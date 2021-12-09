/*
 * Copyright (c) 2017, NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT maxim_max30101

#include <logging/log.h>

#include "max30101.h"

LOG_MODULE_REGISTER(MAX30101, CONFIG_SENSOR_LOG_LEVEL);

static int max30101_sample_fetch(const struct device *dev,
				 enum sensor_channel chan)
{
	struct max30101_data *data = dev->data;
	const struct max30101_config *config = dev->config;
	uint8_t buffer[MAX30101_MAX_BYTES_PER_SAMPLE];
	uint32_t fifo_data;
	int fifo_chan;
	int num_bytes;
	int i;

	/* Read all the active channels for one sample */
	num_bytes = data->num_channels * MAX30101_BYTES_PER_CHANNEL;
	if (i2c_burst_read_dt(&config->bus,
			   MAX30101_REG_FIFO_DATA, buffer, num_bytes)) {
		LOG_ERR("Could not fetch sample");
		return -EIO;
	}

	fifo_chan = 0;
	for (i = 0; i < num_bytes; i += 3) {
		/* Each channel is 18-bits */
		fifo_data = (buffer[i] << 16) | (buffer[i + 1] << 8) |
			    (buffer[i + 2]);
		fifo_data &= MAX30101_FIFO_DATA_MASK;

		/* Save the raw data */
		data->raw[fifo_chan++] = fifo_data;
	}

	return 0;
}

static int max30101_channel_get(const struct device *dev,
				enum sensor_channel chan,
				struct sensor_value *val)
{
	struct max30101_data *data = dev->data;
	enum max30101_led_channel led_chan;
	int fifo_chan;

	switch (chan) {
	case SENSOR_CHAN_RED:
		led_chan = MAX30101_LED_CHANNEL_RED;
		break;

	case SENSOR_CHAN_IR:
		led_chan = MAX30101_LED_CHANNEL_IR;
		break;

	case SENSOR_CHAN_GREEN:
		led_chan = MAX30101_LED_CHANNEL_GREEN;
		break;

	default:
		LOG_ERR("Unsupported sensor channel");
		return -ENOTSUP;
	}

	/* Check if the led channel is active by looking up the associated fifo
	 * channel. If the fifo channel isn't valid, then the led channel
	 * isn't active.
	 */
	fifo_chan = data->map[led_chan];
	if (fifo_chan >= MAX30101_MAX_NUM_CHANNELS) {
		LOG_ERR("Inactive sensor channel");
		return -ENOTSUP;
	}

	/* TODO: Scale the raw data to standard units */
	val->val1 = data->raw[fifo_chan];
	val->val2 = 0;

	return 0;
}

static const struct sensor_driver_api max30101_driver_api = {
	.sample_fetch = max30101_sample_fetch,
	.channel_get = max30101_channel_get,
};

static int max30101_init(const struct device *dev)
{
	const struct max30101_config *config = dev->config;
	struct max30101_data *data = dev->data;
	uint8_t part_id;
	uint8_t mode_cfg;
	uint32_t led_chan;
	int fifo_chan;

	/* Wait for I2C bus */
	if (!device_is_ready(config->bus.bus)) {
		LOG_ERR("I2C dev %s not ready", config->bus.bus->name);
		return -ENODEV;
	}

	/* Check the part id to make sure this is MAX30101 */
	if (i2c_reg_read_byte_dt(&config->bus,
			      MAX30101_REG_PART_ID, &part_id)) {
		LOG_ERR("Could not get Part ID");
		return -EIO;
	}
	if (part_id != MAX30101_PART_ID) {
		LOG_ERR("Got Part ID 0x%02x, expected 0x%02x",
			    part_id, MAX30101_PART_ID);
		return -EIO;
	}

	/* Reset the sensor */
	if (i2c_reg_write_byte_dt(&config->bus,
			       MAX30101_REG_MODE_CFG,
			       MAX30101_MODE_CFG_RESET_MASK)) {
		return -EIO;
	}

	/* Wait for reset to be cleared */
	do {
		if (i2c_reg_read_byte_dt(&config->bus,
				      MAX30101_REG_MODE_CFG, &mode_cfg)) {
			LOG_ERR("Could read mode cfg after reset");
			return -EIO;
		}
	} while (mode_cfg & MAX30101_MODE_CFG_RESET_MASK);

	/* Write the FIFO configuration register */
	if (i2c_reg_write_byte_dt(&config->bus,
			       MAX30101_REG_FIFO_CFG, config->fifo)) {
		return -EIO;
	}

	/* Write the mode configuration register */
	if (i2c_reg_write_byte_dt(&config->bus,
			       MAX30101_REG_MODE_CFG, config->mode)) {
		return -EIO;
	}

	/* Write the SpO2 configuration register */
	if (i2c_reg_write_byte_dt(&config->bus,
			       MAX30101_REG_SPO2_CFG, config->spo2)) {
		return -EIO;
	}

	/* Write the LED pulse amplitude registers */
	if (i2c_reg_write_byte_dt(&config->bus,
			       MAX30101_REG_LED1_PA, config->led_pa[0])) {
		return -EIO;
	}
	if (i2c_reg_write_byte_dt(&config->bus,
			       MAX30101_REG_LED2_PA, config->led_pa[1])) {
		return -EIO;
	}
	if (i2c_reg_write_byte_dt(&config->bus,
			       MAX30101_REG_LED3_PA, config->led_pa[2])) {
		return -EIO;
	}

	if (config->mode == MAX30101_MODE_MULTI_LED) {
		uint8_t multi_led[2];

	    /* Write the multi-LED mode control registers */
	    multi_led[0] = (config->slot[1] << 4) | (config->slot[0]);
	    multi_led[1] = (config->slot[3] << 4) | (config->slot[2]);

		if (i2c_reg_write_byte_dt(&config->bus,
			      MAX30101_REG_MULTI_LED, multi_led[0])) {
			return -EIO;
		}
		if (i2c_reg_write_byte_dt(&config->bus,
			      MAX30101_REG_MULTI_LED + 1, multi_led[1])) {
			return -EIO;
		}
	}

	/* Initialize the channel map and active channel count */
	data->num_channels = 0U;
	for (led_chan = 0U; led_chan < MAX30101_MAX_NUM_CHANNELS; led_chan++) {
		data->map[led_chan] = MAX30101_MAX_NUM_CHANNELS;
	}

	/* Count the number of active channels and build a map that translates
	 * the LED channel number (red/ir/green) to the fifo channel number.
	 */
	for (fifo_chan = 0; fifo_chan < MAX30101_MAX_NUM_CHANNELS;
	     fifo_chan++) {
		led_chan = (config->slot[fifo_chan] & MAX30101_SLOT_LED_MASK)-1;
		if (led_chan < MAX30101_MAX_NUM_CHANNELS) {
			data->map[led_chan] = fifo_chan;
			data->num_channels++;
		}
	}

	return 0;
}

#define MAX30101_INIT(n)									\
	static struct max30101_config max30101_config_##n = {					\
		.bus = I2C_DT_SPEC_INST_GET(n),							\
		.fifo = (DT_INST_PROP(n, smp_ave) << MAX30101_FIFO_CFG_SMP_AVE_SHIFT) |		\
				(DT_INST_PROP(n, fifo_rollover_en) <<				\
				MAX30101_FIFO_CFG_ROLLOVER_EN_SHIFT) |				\
				(DT_INST_PROP(n, fifo_a_full) <<				\
				MAX30101_FIFO_CFG_FIFO_FULL_SHIFT),				\
		.mode = DT_INST_PROP(n, led_mode),						\
		.slot[0] = (DT_INST_PROP(n, led_mode) != MAX30101_MODE_MULTI_LED) ?		\
				MAX30101_SLOT_RED_LED1_PA : DT_INST_PROP(n, led_slot1),		\
		.slot[1] = (DT_INST_PROP(n, led_mode) == MAX30101_MODE_SPO2) ?			\
				MAX30101_SLOT_IR_LED2_PA : DT_INST_PROP(n, led_slot2),		\
		.slot[2] = DT_INST_PROP(n, led_slot3),						\
		.slot[3] = DT_INST_PROP(n, led_slot4),						\
		.spo2 = (DT_INST_PROP(n, adc_rge) << MAX30101_SPO2_ADC_RGE_SHIFT) |		\
				(DT_INST_PROP(n, smp_rate_ctrl) << MAX30101_SPO2_SR_SHIFT) |	\
				(MAX30101_PW_18BITS << MAX30101_SPO2_PW_SHIFT),			\
		.led_pa[0] = DT_INST_PROP(n, led1_pa),						\
		.led_pa[1] = DT_INST_PROP(n, led2_pa),						\
		.led_pa[2] = DT_INST_PROP(n, led3_pa),						\
	};											\
												\
	static struct max30101_data max30101_data_##n;						\
												\
	DEVICE_DT_INST_DEFINE(n, max30101_init, NULL,						\
				&max30101_data_##n, &max30101_config_##n,			\
				POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,			\
				&max30101_driver_api);
DT_INST_FOREACH_STATUS_OKAY(MAX30101_INIT)
