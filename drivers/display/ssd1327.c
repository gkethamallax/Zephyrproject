/*
 * Copyright (c) 2024 Savoir-faire Linux
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ssd1327, CONFIG_DISPLAY_LOG_LEVEL);

#include <string.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/kernel.h>

#include "ssd1327_regs.h"

#define SSD1327_ENABLE_VDD				0x01
#define SSD1327_ENABLE_SECOND_PRECHARGE			0x62
#define SSD1327_VCOMH_VOLTAGE				0x0f
#define SSD1327_PHASES_VALUE				0xf1
#define SSD1327_DEFAULT_PRECHARGE_V			0x08
#define SSD1327_UNLOCK_COMMAND				0x12

union ssd1327_bus {
	struct spi_dt_spec spi;
};

typedef bool (*ssd1327_bus_ready_fn)(const struct device *dev);
typedef int (*ssd1327_write_bus_fn)(const struct device *dev, uint8_t *buf, size_t len,
				    bool command);
typedef const char *(*ssd1327_bus_name_fn)(const struct device *dev);

struct ssd1327_config {
	union ssd1327_bus bus;
	struct gpio_dt_spec data_cmd;
	struct gpio_dt_spec reset;
	ssd1327_bus_ready_fn bus_ready;
	ssd1327_write_bus_fn write_bus;
	ssd1327_bus_name_fn bus_name;
	uint16_t height;
	uint16_t width;
	uint8_t oscillator_freq;
	uint8_t start_line;
	uint8_t display_offset;
	uint8_t multiplex_ratio;
	uint8_t prechargep;
	uint8_t remap_value;
	bool color_inversion;
};

struct ssd1327_data {
	uint8_t contrast;
	uint8_t scan_mode;
};

#if (DT_HAS_COMPAT_ON_BUS_STATUS_OKAY(solomon_ssd1327fb, spi))
static bool ssd1327_bus_ready_spi(const struct device *dev)
{
	const struct ssd1327_config *config = dev->config;

	if (gpio_pin_configure_dt(&config->data_cmd, GPIO_OUTPUT_INACTIVE) < 0) {
		return false;
	}

	return spi_is_ready_dt(&config->bus.spi);
}

static int ssd1327_write_bus_spi(const struct device *dev, uint8_t *buf, size_t len, bool command)
{
	const struct ssd1327_config *config = dev->config;
	int errno;

	gpio_pin_set_dt(&config->data_cmd, command ? 0 : 1);
	struct spi_buf tx_buf = {
		.buf = buf,
		.len = len
	};

	struct spi_buf_set tx_bufs = {
		.buffers = &tx_buf,
		.count = 1
	};

	errno = spi_write_dt(&config->bus.spi, &tx_bufs);

	return errno;
}

static const char *ssd1327_bus_name_spi(const struct device *dev)
{
	const struct ssd1327_config *config = dev->config;

	return config->bus.spi.bus->name;
}
#endif

static inline bool ssd1327_bus_ready(const struct device *dev)
{
	const struct ssd1327_config *config = dev->config;

	return config->bus_ready(dev);
}

static inline int ssd1327_write_bus(const struct device *dev, uint8_t *buf, size_t len,
				    bool command)
{
	const struct ssd1327_config *config = dev->config;

	return config->write_bus(dev, buf, len, command);
}

static inline int ssd1327_set_timing_setting(const struct device *dev)
{
	const struct ssd1327_config *config = dev->config;

	uint8_t cmd_buf[] = {
		SSD1327_SET_PHASE_LENGTH,
		SSD1327_PHASES_VALUE,
		SSD1327_SET_OSC_FREQ,
		config->oscillator_freq,
		SSD1327_SET_PRECHARGE_PERIOD,
		config->prechargep,
		SSD1327_LINEAR_LUT,
		SSD1327_SET_PRECHARGE_VOLTAGE,
		SSD1327_DEFAULT_PRECHARGE_V,
		SSD1327_SET_VCOMH,
		SSD1327_VCOMH_VOLTAGE,
		SSD1327_FUNCTION_SELECTION_B,
		SSD1327_ENABLE_SECOND_PRECHARGE,
		SSD1327_SET_COMMAND_LOCK,
		SSD1327_UNLOCK_COMMAND,
	};

	return ssd1327_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static inline int ssd1327_set_hardware_config(const struct device *dev)
{
	const struct ssd1327_config *config = dev->config;

	uint8_t cmd_buf[] = {
		SSD1327_SET_DISPLAY_START_LINE,
		config->start_line,
		SSD1327_SET_DISPLAY_OFFSET,
		config->display_offset,
		SSD1327_SET_NORMAL_DISPLAY,
		SSD1327_SET_MULTIPLEX_RATIO,
		config->multiplex_ratio,
		SSD1327_SET_FUNCTION_A,
		SSD1327_ENABLE_VDD,
	};

	return ssd1327_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static int ssd1327_resume(const struct device *dev)
{
	uint8_t cmd_buf[] = {
		SSD1327_DISPLAY_ON,
	};

	return ssd1327_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static int ssd1327_suspend(const struct device *dev)
{
	uint8_t cmd_buf[] = {
		SSD1327_DISPLAY_OFF,
	};

	return ssd1327_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static int ssd1327_set_display(const struct device *dev)
{
	const struct ssd1327_config *config = dev->config;

	uint8_t cmd_buf[] = {
		SSD1327_SET_COLUMN_ADDR,
		0,
		config->width - 1,
		SSD1327_SET_ROW_ADDR,
		0,
		config->height - 1,
		SSD1327_SET_SEGMENT_MAP_REMAPED,
		config->remap_value,
	};

	return ssd1327_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static int ssd1327_write_default(const struct device *dev, const uint16_t x, const uint16_t y,
				 const struct display_buffer_descriptor *desc, const void *buf,
				 const size_t buf_len)
{
	uint8_t cmd_buf[] = {
		SSD1327_SET_COLUMN_ADDR,
		x,
		x + desc->width - 1,
		SSD1327_SET_ROW_ADDR,
		y,
		y + desc->height - 1
	};

	if (ssd1327_write_bus(dev, cmd_buf, sizeof(cmd_buf), true)) {
		LOG_ERR("Failed to write command");
		return -1;
	}

	return ssd1327_write_bus(dev, (uint8_t *)buf, buf_len, false);
}

static int ssd1327_write(const struct device *dev, const uint16_t x, const uint16_t y,
			 const struct display_buffer_descriptor *desc, const void *buf)
{
	size_t buf_len;

	if (desc->pitch < desc->width) {
		LOG_ERR("Pitch is smaller then width");
		return -1;
	}
	/* Following the datasheet, in the GDDRAM, two segment are split in one register */
	buf_len = MIN(desc->buf_size, desc->height * desc->width / 2);
	if (buf == NULL || buf_len == 0U) {
		LOG_ERR("Display buffer is not available");
		return -1;
	}

	if (desc->pitch > desc->width) {
		LOG_ERR("Unsupported mode");
		return -1;
	}

	if ((y & 0x7) != 0U) {
		LOG_ERR("Unsupported origin");
		return -1;
	}

	LOG_DBG("x %u, y %u, pitch %u, width %u, height %u, buf_len %u", x, y, desc->pitch,
		desc->width, desc->height, buf_len);

	return ssd1327_write_default(dev, x, y, desc, buf, buf_len);
}

static int ssd1327_set_contrast(const struct device *dev, const uint8_t contrast)
{
	uint8_t cmd_buf[] = {
		SSD1327_SET_CONTRAST_CTRL,
		contrast,
	};

	return ssd1327_write_bus(dev, cmd_buf, sizeof(cmd_buf), true);
}

static void ssd1327_get_capabilities(const struct device *dev,
				     struct display_capabilities *caps)
{
	const struct ssd1327_config *config = dev->config;

	memset(caps, 0, sizeof(struct display_capabilities));
	caps->x_resolution = config->width;
	caps->y_resolution = config->height;
	caps->supported_pixel_formats = PIXEL_FORMAT_MONO10;
	caps->current_pixel_format = PIXEL_FORMAT_MONO10;
	caps->screen_info = SCREEN_INFO_MONO_VTILED;
}

static int ssd1327_set_pixel_format(const struct device *dev,
				    const enum display_pixel_format pf)
{
	if (pf == PIXEL_FORMAT_MONO10) {
		return 0;
	}
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static int ssd1327_init_device(const struct device *dev)
{
	const struct ssd1327_config *config = dev->config;

	uint8_t cmd_buf[] = {
		SSD1327_SET_ENTIRE_DISPLAY_OFF,
		(config->color_inversion ? SSD1327_SET_REVERSE_DISPLAY
					 : SSD1327_SET_NORMAL_DISPLAY),
	};

	/* Reset if pin connected */
	if (config->reset.port) {
		k_sleep(K_USEC(SSD1327_RESET_DELAY_1000US));
		gpio_pin_set_dt(&config->reset, 1);
		k_sleep(K_USEC(SSD1327_RESET_DELAY_100US));
		gpio_pin_set_dt(&config->reset, 0);
		k_sleep(K_MSEC(SSD1327_RESET_DELAY_300MS));
	}

	/* Turn display off */
	if (ssd1327_suspend(dev)) {
		return -EIO;
	}

	if (ssd1327_set_display(dev)) {
		return -EIO;
	}

	if (ssd1327_set_contrast(dev, CONFIG_SSD1327_DEFAULT_CONTRAST)) {
		return -EIO;
	}

	if (ssd1327_set_hardware_config(dev)) {
		return -EIO;
	}

	if (ssd1327_write_bus(dev, cmd_buf, sizeof(cmd_buf), true)) {
		return -EIO;
	}

	if (ssd1327_set_timing_setting(dev)) {
		return -EIO;
	}

	ssd1327_resume(dev);

	return 0;
}

static int ssd1327_init(const struct device *dev)
{
	const struct ssd1327_config *config = dev->config;

	LOG_DBG("Initializing device %s", config->bus_name(dev));

	if (!ssd1327_bus_ready(dev)) {
		LOG_ERR("Bus device %s not ready!", config->bus_name(dev));
		return -EINVAL;
	}

	if (config->reset.port) {
		int ret;

		ret = gpio_pin_configure_dt(&config->reset,
					    GPIO_OUTPUT_INACTIVE);
		if (ret < 0) {
			return ret;
		}
	}

	if (ssd1327_init_device(dev)) {
		LOG_ERR("Failed to initialize device!");
		return -EIO;
	}

	return 0;
}

static struct display_driver_api ssd1327_driver_api = {
	.blanking_on = ssd1327_suspend,
	.blanking_off = ssd1327_resume,
	.write = ssd1327_write,
	.set_contrast = ssd1327_set_contrast,
	.get_capabilities = ssd1327_get_capabilities,
	.set_pixel_format = ssd1327_set_pixel_format,
};

#define SSD1327_CONFIG_SPI(node_id)                                                                \
	.bus = {.spi = SPI_DT_SPEC_GET(                                                            \
			node_id, SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8), 0)},     \
	.bus_ready = ssd1327_bus_ready_spi,                                                        \
	.write_bus = ssd1327_write_bus_spi,                                                        \
	.bus_name = ssd1327_bus_name_spi,                                                          \
	.data_cmd = GPIO_DT_SPEC_GET(node_id, data_cmd_gpios),

    #define SSD1327_DEFINE(node_id)                                                                \
	static struct ssd1327_data data##node_id;                                                  \
	static const struct ssd1327_config config##node_id = {                                     \
		.reset = GPIO_DT_SPEC_GET_OR(node_id, reset_gpios, {0}),                           \
		.height = DT_PROP(node_id, height),                                                \
		.width = DT_PROP(node_id, width),                                                  \
		.oscillator_freq = DT_PROP(node_id, oscillator_freq),                              \
		.display_offset = DT_PROP(node_id, display_offset),                                \
		.start_line = DT_PROP(node_id, start_line),                                        \
		.multiplex_ratio = DT_PROP(node_id, multiplex_ratio),                              \
		.prechargep = DT_PROP(node_id, prechargep),                                        \
		.remap_value = DT_PROP(node_id, remap_value),                                      \
		.color_inversion = DT_PROP(node_id, inversion_on),                                 \
		COND_CODE_1(DT_ON_BUS(node_id, spi), (SSD1327_CONFIG_SPI(node_id)),                \
			    ())									\
	};                                                                                         \
                                                                                                   \
	DEVICE_DT_DEFINE(node_id, ssd1327_init, NULL, &data##node_id, &config##node_id,            \
			 POST_KERNEL, CONFIG_DISPLAY_INIT_PRIORITY, &ssd1327_driver_api);

DT_FOREACH_STATUS_OKAY(solomon_ssd1327fb, SSD1327_DEFINE)
