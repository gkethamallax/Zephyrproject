/*
 * Copyright (c) 2021 Murali Tejeshwar Janaswami
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SI1133_H
#define _SI1133_H

#define SI1133_VAL_STARTUP_TIME_MS		25
#define SI1133_VAL_PART_ID			0x33

#define SI1133_I2C_REG_PART_ID			0x00
#define SI1133_I2C_REG_COMMAND			0x0B
#define SI1133_I2C_REG_RESPONSE0		0x11
#define SI1133_I2C_REG_RESPONSE1		0x10
#define SI1133_I2C_REG_HOSTIN0			0x0A
#define SI1133_I2C_REG_IRQ_STATUS		0x12
#define SI1133_I2C_REG_HOSTOUT_BASE		0x13

#define SI1133_CMD_REG_PRM_SET_PRFX		0b10000000
#define SI1133_CMD_REG_PRM_QRY_PRFX		0b01000000
#define SI1133_CMD_REG_RST_CMD_CTR		0x00
#define SI1133_CMD_REG_FORCE			0x11

#define SI1133_PRM_TBL_CHAN_LIST		0x01
#define SI1133_PRM_TBL_ADCCONFIG0		0x02
#define SI1133_PRM_TBL_ADCCONFIG1		0x06
#define SI1133_PRM_TBL_ADCCONFIG2		0x0A
#define SI1133_PRM_TBL_ADCSENS0			0x03
#define SI1133_PRM_TBL_ADCSENS1			0x07
#define SI1133_PRM_TBL_ADCSENS2			0x0B
#define SI1133_PRM_TBL_ADCPOST0			0x04
#define SI1133_PRM_TBL_ADCPOST1			0x08
#define SI1133_PRM_TBL_ADCPOST2			0x0C

#define SI1133_RESPONSE0_BIT_CMD_ERR		1 << 4
#define SI1133_RESPONSE0_BITS_CMD_CTR		0b1111
#define SI1133_RESPONSE0_BITS_CMD_ERR		0b11111

#define SI1133_CFG_ADCCONFIG0_ADCMUX_WHITE	0b01011
#define SI1133_CFG_ADCCONFIG1_ADCMUX_SMALL_IR	0b00000
#define SI1133_CFG_ADCCONFIG2_ADCMUX_UV		0b11000
#define SI1133_CFG_ENABLE_CHANNEL_0		1 << 0
#define SI1133_CFG_ENABLE_CHANNEL_1		1 << 1
#define SI1133_CFG_ENABLE_CHANNEL_2		1 << 2
#define SI1133_CFG_ADCCONFIG0_DECIM_RATE	0
#define SI1133_CFG_ADCCONFIG1_DECIM_RATE	0
#define SI1133_CFG_ADCCONFIG2_DECIM_RATE	3 << 5
#define SI1133_CFG_ADCSENS0_HW_GAIN		0
#define SI1133_CFG_ADCSENS1_HW_GAIN		0
#define SI1133_CFG_ADCSENS2_HW_GAIN		9

#ifdef CONFIG_SI1133_OPERATING_MODE_BL
#define SI1133_CFG_ADCPOST0_24BIT_OUT		1 << 6
#define SI1133_CFG_ADCPOST1_24BIT_OUT		1 << 6
#define SI1133_CFG_ADCPOST2_24BIT_OUT		0 << 6
#else
#define SI1133_CFG_ADCPOST0_24BIT_OUT		0
#define SI1133_CFG_ADCPOST1_24BIT_OUT		0
#define SI1133_CFG_ADCPOST2_24BIT_OUT		0
#endif

#ifdef CONFIG_SI1133_OPERATING_MODE_BL
#define SI1133_CFG_CHANNEL_0_DATA_BYTES		3
#define SI1133_CFG_CHANNEL_1_DATA_BYTES		3
#define SI1133_CFG_CHANNEL_2_DATA_BYTES		2
#else
#define SI1133_CFG_CHANNEL_0_DATA_BYTES		2
#define SI1133_CFG_CHANNEL_1_DATA_BYTES		2
#define SI1133_CFG_CHANNEL_2_DATA_BYTES		2
#endif

#define SI1133_CFG_TOTAL_OUTPUT_BYTES		SI1133_CFG_CHANNEL_0_DATA_BYTES + \
						SI1133_CFG_CHANNEL_1_DATA_BYTES + \
						SI1133_CFG_CHANNEL_2_DATA_BYTES

struct si1133_chan_config {
	uint8_t enable_chan;
	uint8_t adcconfig_addr;
	uint8_t adcconfig_val;
	uint8_t adcsens_addr;
	uint8_t adcsens_val;
	uint8_t adcpost_addr;
	uint8_t adcpost_val;
};

struct si1133_output {
#ifdef CONFIG_SI1133_OPERATING_MODE_BL
	int32_t visible_light;
	int32_t infrared_light;
#else
	uint16_t visible_light;
	uint16_t infrared_light;
#endif
	uint16_t ultraviolet_light;
};

struct si1133_data {
	uint16_t i2c_addr;
	const struct device *i2c_master;
	struct si1133_output data;
};

struct si1133_dev_config {
	const char *i2c_master_name;
	uint16_t i2c_addr;
};

#endif /* _SI1133_H */
