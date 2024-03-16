/**
 * @file
 * @brief Bluetooth Basic Audio Profile shell USB extension
 *
 * This files handles all the USB related functionality to audio in/out for the BAP shell
 *
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>

#include <zephyr/bluetooth/audio/audio.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/class/usb_audio.h>
#include <nrfx_clock.h>

#include "shell/bt.h"
#include "audio.h"

LOG_MODULE_REGISTER(bap_usb, CONFIG_BT_BAP_STREAM_LOG_LEVEL);

#define USB_ENQUEUE_COUNT     20U
#define USB_FRAME_DURATION_US 1000U
#define USB_SAMPLE_CNT         ((USB_FRAME_DURATION_US * USB_SAMPLE_RATE) / USEC_PER_SEC)
#define USB_BYTES_PER_SAMPLE   sizeof(int16_t)
#define USB_MONO_FRAME_SIZE    (USB_SAMPLE_CNT * USB_BYTES_PER_SAMPLE)
#define USB_CHANNELS           2U
#define USB_STEREO_FRAME_SIZE  (USB_MONO_FRAME_SIZE * USB_CHANNELS)
#define USB_OUT_RING_BUF_SIZE  (CONFIG_BT_ISO_RX_BUF_COUNT * LC3_MAX_NUM_SAMPLES_STEREO)
#define USB_IN_RING_BUF_SIZE   (CONFIG_BT_ISO_TX_BUF_COUNT * LC3_MAX_NUM_SAMPLES_STEREO)

#if defined CONFIG_BT_AUDIO_RX
struct decoded_sdu {
	int16_t right_frames[MAX_CODEC_FRAMES_PER_SDU][LC3_MAX_NUM_SAMPLES_MONO];
	int16_t left_frames[MAX_CODEC_FRAMES_PER_SDU][LC3_MAX_NUM_SAMPLES_MONO];
	size_t right_frames_cnt;
	size_t left_frames_cnt;
	size_t mono_frames_cnt;
	uint32_t ts;
} decoded_sdu;

RING_BUF_DECLARE(usb_out_ring_buf, USB_OUT_RING_BUF_SIZE);
NET_BUF_POOL_DEFINE(usb_out_buf_pool, USB_ENQUEUE_COUNT, USB_STEREO_FRAME_SIZE, 0, net_buf_destroy);

/* USB consumer callback, called every 1ms, consumes data from ring-buffer */
static void usb_data_request_cb(const struct device *dev)
{
	uint8_t usb_audio_data[USB_STEREO_FRAME_SIZE] = {0};
	struct net_buf *pcm_buf;
	uint32_t size;
	int err;

	if (bap_get_rx_streaming_cnt() == 0) {
		/* no-op */
		return;
	}

	pcm_buf = net_buf_alloc(&usb_out_buf_pool, K_NO_WAIT);
	if (pcm_buf == NULL) {
		LOG_WRN("Could not allocate pcm_buf");
		return;
	}

	/* This may fail without causing issues since usb_audio_data is 0-initialized */
	size = ring_buf_get(&usb_out_ring_buf, usb_audio_data, sizeof(usb_audio_data));

	net_buf_add_mem(pcm_buf, usb_audio_data, sizeof(usb_audio_data));

	if (size != 0) {
		static size_t cnt;

		if ((++cnt % bap_get_recv_stats_interval()) == 0U) {
			LOG_INF("[%zu]: Sending USB audio", cnt);
		}
	} else {
		static size_t cnt;

		if ((++cnt % bap_get_recv_stats_interval()) == 0U) {
			LOG_INF("[%zu]: Sending empty USB audio", cnt);
		}
	}

	err = usb_audio_send(dev, pcm_buf, sizeof(usb_audio_data));
	if (err != 0) {
		static size_t cnt;

		cnt++;
		if ((cnt % 1000) == 0) {
			LOG_ERR("Failed to send USB audio: %d (%zu)", err, cnt);
		}

		net_buf_unref(pcm_buf);
	}
}

static void usb_data_written_cb(const struct device *dev, struct net_buf *buf, size_t size)
{
	/* Unreference the buffer now that the USB is done with it */
	net_buf_unref(buf);
}

static void bap_usb_send_frames_to_usb(void)
{
	const bool is_left_only =
		decoded_sdu.right_frames_cnt == 0U && decoded_sdu.mono_frames_cnt == 0U;
	const bool is_right_only =
		decoded_sdu.left_frames_cnt == 0U && decoded_sdu.mono_frames_cnt == 0U;
	const bool is_mono_only =
		decoded_sdu.left_frames_cnt == 0U && decoded_sdu.right_frames_cnt == 0U;
	const bool is_single_channel = is_left_only || is_right_only || is_mono_only;
	const size_t frame_cnt =
		MAX(decoded_sdu.mono_frames_cnt,
		    MAX(decoded_sdu.left_frames_cnt, decoded_sdu.right_frames_cnt));
	static size_t cnt;

	/* Send frames to USB - If we only have a single channel we mix it to stereo */
	for (size_t i = 0U; i < frame_cnt; i++) {
		static int16_t stereo_frame[LC3_MAX_NUM_SAMPLES_STEREO];
		const int16_t *right_frame = decoded_sdu.right_frames[i];
		const int16_t *left_frame = decoded_sdu.left_frames[i];
		const int16_t *mono_frame = decoded_sdu.left_frames[i]; /* use left as mono */
		static size_t fail_cnt;
		uint32_t rb_size;

		/* Not enough space to store data */
		if (ring_buf_space_get(&usb_out_ring_buf) < sizeof(stereo_frame)) {
			if ((fail_cnt % bap_get_recv_stats_interval()) == 0U) {
				LOG_WRN("[%zu] Could not send more than %zu frames to USB",
					fail_cnt, i);
			}

			fail_cnt++;

			break;
		}

		fail_cnt = 0U;

		/* Generate the stereo frame
		 *
		 * If we only have single channel then we mix that to stereo
		 */
		for (int j = 0; j < LC3_MAX_NUM_SAMPLES_MONO; j++) {
			if (is_single_channel) {
				int16_t sample = 0;

				/* Mix to stereo as LRLRLRLR */
				if (is_left_only) {
					sample = left_frame[j];
				} else if (is_right_only) {
					sample = right_frame[j];
				} else if (is_mono_only) {
					sample = mono_frame[j];
				}

				stereo_frame[j * 2] = sample;
				stereo_frame[j * 2 + 1] = sample;
			} else {
				stereo_frame[j * 2] = left_frame[j];
				stereo_frame[j * 2 + 1] = right_frame[j];
			}
		}

		rb_size = ring_buf_put(&usb_out_ring_buf, (uint8_t *)stereo_frame,
				       sizeof(stereo_frame));
		if (rb_size != sizeof(stereo_frame)) {
			LOG_WRN("Failed to put frame on USB ring buf");

			break;
		}
	}

	if ((++cnt % bap_get_recv_stats_interval()) == 0U) {
		LOG_INF("[%zu]: Sending %u USB audio frame", cnt, frame_cnt);
	}

	bap_usb_clear_frames_to_usb();
}

static bool ts_overflowed(uint32_t ts)
{
	/* If the timestamp is a factor of 10 in difference, then we assume that TS overflowed */
	return ((uint64_t)ts * 10 < decoded_sdu.ts);
}

int bap_usb_add_frame_to_usb(enum bt_audio_location chan_allocation, const int16_t *frame,
			     size_t frame_size, uint32_t ts)
{
	const bool is_left = (chan_allocation & BT_AUDIO_LOCATION_FRONT_LEFT) != 0;
	const bool is_right = (chan_allocation & BT_AUDIO_LOCATION_FRONT_RIGHT) != 0;
	const bool is_mono = chan_allocation == BT_AUDIO_LOCATION_MONO_AUDIO;
	const uint8_t ts_jitter_us = 100; /* timestamps may have jitter */

	static size_t cnt;

	if ((++cnt % bap_get_recv_stats_interval()) == 0U) {
		LOG_INF("[%zu]: Adding USB audio frame", cnt);
	}

	if (frame_size > LC3_MAX_NUM_SAMPLES_MONO * sizeof(int16_t) || frame_size == 0U) {
		LOG_DBG("Invalid frame of size %zu", frame_size);

		return -EINVAL;
	}

	if (get_chan_cnt(chan_allocation) != 1) {
		LOG_DBG("Invalid channel allocation %d", chan_allocation);

		return -EINVAL;
	}

	if (((is_left || is_right) && decoded_sdu.mono_frames_cnt != 0) ||
	    (is_mono &&
	     (decoded_sdu.left_frames_cnt != 0U || decoded_sdu.right_frames_cnt != 0U))) {
		LOG_DBG("Cannot mix and match mono with left or right");

		return -EINVAL;
	}

	/* Check if the frame can be combined with a previous frame from another channel, of if
	 * we have to send previous data to USB and then store the current frame
	 *
	 * This is done by comparing the timestamps of the frames, and in the case that they are the
	 * same, there are additional checks to see if we have received more left than right frames,
	 * in which case we also send existing data
	 */

	if (ts + ts_jitter_us < decoded_sdu.ts && !ts_overflowed(ts)) {
		/* Old data, discard */
		return -ENOEXEC;
	} else if (ts > decoded_sdu.ts + ts_jitter_us || ts_overflowed(ts)) {
		/* We are getting new data - Send existing data to ring buffer */
		bap_usb_send_frames_to_usb();
	} else { /* same timestamp */
		bool send = false;

		if (is_left && decoded_sdu.left_frames_cnt > decoded_sdu.right_frames_cnt) {
			/* We are receiving left again before a right, send to USB */
			send = true;
		} else if (is_right && decoded_sdu.right_frames_cnt > decoded_sdu.left_frames_cnt) {
			/* We are receiving right again before a left, send to USB */
			send = true;
		} else if (is_mono) {
			/* always send mono as it comes */
			send = true;
		}

		if (true || send) {
			bap_usb_send_frames_to_usb();
		}
	}

	if (is_left) {
		if (decoded_sdu.left_frames_cnt >= ARRAY_SIZE(decoded_sdu.left_frames)) {
			LOG_WRN("Could not add more left frames");

			return -ENOMEM;
		}

		memcpy(decoded_sdu.left_frames[decoded_sdu.left_frames_cnt++], frame, frame_size);
	} else if (is_right) {
		if (decoded_sdu.right_frames_cnt >= ARRAY_SIZE(decoded_sdu.right_frames)) {
			LOG_WRN("Could not add more right frames");

			return -ENOMEM;
		}

		memcpy(decoded_sdu.right_frames[decoded_sdu.right_frames_cnt++], frame, frame_size);
	} else if (is_mono) {
		/* Use left as mono*/
		if (decoded_sdu.mono_frames_cnt >= ARRAY_SIZE(decoded_sdu.left_frames)) {
			LOG_WRN("Could not add more mono frames");

			return -ENOMEM;
		}

		memcpy(decoded_sdu.left_frames[decoded_sdu.mono_frames_cnt++], frame, frame_size);
	} else {
		/* Unsupported channel */
		LOG_DBG("Unsupported channel %d", chan_allocation);

		return -EINVAL;
	}

	decoded_sdu.ts = ts;

	return 0;
}

void bap_usb_clear_frames_to_usb(void)
{
	decoded_sdu.mono_frames_cnt = 0U;
	decoded_sdu.right_frames_cnt = 0U;
	decoded_sdu.left_frames_cnt = 0U;
	decoded_sdu.ts = 0U;
}
#endif /* CONFIG_BT_AUDIO_RX */

#if defined(CONFIG_BT_AUDIO_TX)

static void store_data_in_stream(struct shell_stream *sh_stream, int16_t left_data[USB_SAMPLE_CNT],
				 int16_t right_data[USB_SAMPLE_CNT])
{
	const bool has_right =
		(sh_stream->lc3_chan_allocation & BT_AUDIO_LOCATION_FRONT_RIGHT) != 0;
	const bool has_left = (sh_stream->lc3_chan_allocation & BT_AUDIO_LOCATION_FRONT_LEFT) != 0;

	if (!has_right && !has_left) {
		/* Not suited for USB stereo data */
		return;
	}

	if (has_left) {
		const uint32_t size_put = ring_buf_put(&sh_stream->tx.tx_left_ring_buf,
						       (uint8_t *)left_data, USB_MONO_FRAME_SIZE);
		if (size_put != USB_MONO_FRAME_SIZE) {
			if ((sh_stream->tx.left_ring_buf_fail_cnt %
			     bap_get_recv_stats_interval()) == 0U) {
				LOG_ERR("[%zu]: %p Failed to put left data on ring buffer "
					"(%u != %u) - %u / %u used",
					sh_stream->tx.left_ring_buf_fail_cnt, sh_stream, size_put,
					USB_MONO_FRAME_SIZE,
					ring_buf_size_get(&sh_stream->tx.tx_left_ring_buf),
					ring_buf_capacity_get(&sh_stream->tx.tx_left_ring_buf));
			}

			sh_stream->tx.left_ring_buf_fail_cnt++;
		} else {
			sh_stream->tx.left_ring_buf_fail_cnt = 0U;
		}
	}

	if (has_right) {
		const uint32_t size_put = ring_buf_put(&sh_stream->tx.tx_right_ring_buf,
						       (uint8_t *)right_data, USB_MONO_FRAME_SIZE);
		if (size_put != USB_MONO_FRAME_SIZE) {
			if ((sh_stream->tx.right_ring_buf_fail_cnt %
			     bap_get_recv_stats_interval()) == 0U) {
				LOG_ERR("[%zu]: %p Failed to put right data on ring buffer "
					"(%u != %u) - %u / %u used",
					sh_stream->tx.right_ring_buf_fail_cnt, sh_stream, size_put,
					USB_MONO_FRAME_SIZE,
					ring_buf_size_get(&sh_stream->tx.tx_right_ring_buf),
					ring_buf_capacity_get(&sh_stream->tx.tx_right_ring_buf));
			}

			sh_stream->tx.right_ring_buf_fail_cnt++;
		} else {
			sh_stream->tx.right_ring_buf_fail_cnt = 0U;
		}
	}

	/* Schedule the send work ASAP if not already scheduled */
	if (!sh_stream->tx.tx_active) {
		int err;

		/* TODO: Only trigger when we have store enough data for an entire SDU */

		sh_stream->tx.tx_active = true;
		sh_stream->tx.seq_num = get_next_seq_num(bap_stream_from_shell_stream(sh_stream));
		err = k_work_schedule(&sh_stream->tx.audio_send_work, K_NO_WAIT);
		if (err < 0) {
			LOG_ERR("Failed to trigger send work for stream %p: %d", sh_stream, err);
		}
	}
}

static void usb_data_received_cb(const struct device *dev, struct net_buf *buf, size_t size)
{
	int16_t right_data[USB_SAMPLE_CNT];
	int16_t left_data[USB_SAMPLE_CNT];
	static size_t cnt;
	int16_t *pcm;

	if (buf == NULL) {
		return;
	}

	if (size != USB_STEREO_FRAME_SIZE) {
		net_buf_unref(buf);

		return;
	}

	pcm = (int16_t *)buf->data;

	/* Split the data into left and right as LC3 uses LLLLRRRR instead of LRLRLRLR as USB */
	for (size_t i = 0U; i < USB_SAMPLE_CNT; i++) {
		left_data[i] = pcm[i];
		right_data[i] = pcm[i + 1];
	}

	/* Store the data from USB for each stream that supports TX. Since frames for USB are
	 *generally smaller than the frames sent over ISO, we need to store multiple USB frames
	 *before we can send over air
	 */
#if defined(CONFIG_BT_BAP_UNICAST)
	for (size_t i = 0; i < ARRAY_SIZE(unicast_streams); i++) {
		struct shell_stream *sh_stream = &unicast_streams[i];

		if (sh_stream->is_tx) {
			store_data_in_stream(sh_stream, left_data, right_data);
		}
	}
#endif /* CONFIG_BT_BAP_UNICAST */

#if defined(CONFIG_BT_BAP_BROADCAST_SOURCE)
	for (size_t i = 0; i < ARRAY_SIZE(broadcast_source_streams); i++) {
		struct shell_stream *sh_stream = &broadcast_source_streams[i];

		if (sh_stream->is_tx) {
			store_data_in_stream(sh_stream, left_data, right_data);
		}
	}
#endif /* CONFIG_BT_BAP_BROADCAST_SOURCE */

	if ((++cnt % bap_get_recv_stats_interval()) == 0U) {
		printk("USB Data received (count = %d)\n", cnt);
	}

	net_buf_unref(buf);
}

size_t bap_usb_get_frame_size(const struct shell_stream *sh_stream)
{
	return (sizeof(int16_t) * sh_stream->lc3_frame_duration_us * sh_stream->lc3_freq_hz) /
	       USEC_PER_SEC;
}

bool bap_usb_can_get_full_sdu(struct shell_stream *sh_stream)
{
	const bool has_left = (sh_stream->lc3_chan_allocation & BT_AUDIO_LOCATION_FRONT_LEFT) != 0;
	const bool has_right =
		(sh_stream->lc3_chan_allocation & BT_AUDIO_LOCATION_FRONT_RIGHT) != 0;
	const bool has_stereo = has_right && has_left;
	const uint32_t frame_size = bap_usb_get_frame_size(sh_stream);
	const uint32_t frame_block_size = frame_size * (has_stereo ? 2U : 1U);
	const uint32_t retrieve_size = frame_block_size * sh_stream->lc3_frame_blocks_per_sdu;
	const uint32_t rb_size = ring_buf_size_get(&sh_stream->tx.tx_left_ring_buf);

	if (rb_size < retrieve_size) {
		/* Not enough for a frame yet */
		LOG_WRN("Ring buffer (%u) does not contain enough for an entire SDU %u", rb_size,
			retrieve_size);

		return false;
	}

	return true;
}

bool bap_usb_get_full_frame(struct shell_stream *sh_stream, uint8_t index, uint8_t buffer[])
{
	const bool has_left = (sh_stream->lc3_chan_allocation & BT_AUDIO_LOCATION_FRONT_LEFT) != 0;
	const bool has_right =
		(sh_stream->lc3_chan_allocation & BT_AUDIO_LOCATION_FRONT_RIGHT) != 0;
	const bool is_left = index == 0U && has_left;
	const bool is_right = has_right && (index == 0U || (index == 1U && has_left));
	const uint32_t frame_size = bap_usb_get_frame_size(sh_stream);
	uint32_t rb_size = 0;

	if (is_left) {
		rb_size = ring_buf_get(&sh_stream->tx.tx_left_ring_buf, buffer, frame_size);
		if (rb_size != frame_size) {
			shell_error(ctx_shell, "Failed to get left frame of size %u, got %u",
				    frame_size, rb_size);

			/* What to do here? Pad? Memset? Nothing */
			rb_size = 0;
		}
	} else if (is_right) {
		rb_size = ring_buf_get(&sh_stream->tx.tx_right_ring_buf, buffer, frame_size);
		if (rb_size != frame_size) {
			shell_error(ctx_shell, "Failed to get right frame of size %u, got %u",
				    frame_size, rb_size);

			/* What to do here? Pad? Memset? Nothing */
			rb_size = 0;
		}
	}

	return rb_size != 0;
}
#endif /* CONFIG_BT_AUDIO_TX */

int bap_usb_init(void)
{
	const struct device *hs_dev = DEVICE_DT_GET(DT_NODELABEL(hs_0));
	static const struct usb_audio_ops usb_ops = {
#if defined(CONFIG_BT_AUDIO_RX)
		.data_request_cb = usb_data_request_cb,
		.data_written_cb = usb_data_written_cb,
#endif /* CONFIG_BT_AUDIO_RX */
#if defined(CONFIG_BT_AUDIO_TX)
		.data_received_cb = usb_data_received_cb,
#endif /* CONFIG_BT_AUDIO_TX */
	};
	int err;

	if (!device_is_ready(hs_dev)) {
		LOG_ERR("Cannot get USB Headset Device");
		return -EIO;
	}

	usb_audio_register(hs_dev, &usb_ops);
	err = usb_enable(NULL);
	if (err != 0) {
		LOG_ERR("Failed to enable USB");
		return err;
	}

	if (IS_ENABLED(CONFIG_SOC_NRF5340_CPUAPP)) {
		/* Use this to turn on 128 MHz clock for cpu_app */
		err = nrfx_clock_divider_set(NRF_CLOCK_DOMAIN_HFCLK, NRF_CLOCK_HFCLK_DIV_1);

		err -= NRFX_ERROR_BASE_NUM;
		if (err != 0) {
			LOG_WRN("Failed to set 128 MHz: %d", err);
		}
	}

	return 0;
}
