/*
 * Copyright (c) 2018 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifndef __ZEPHYR__
/* Include Zephyr autogenerated autoconf.h settings: */
#define CONFIG_MQTT_MSG_MAX_SIZE 128
#define CONFIG_MQTT_SUBSCRIBE_MAX_TOPICS 1
#define CONFIG_NET_IPV4 1
/* Most likely, if running Linux on a PC: */
#define CONFIG_X86 1
#endif

#endif /* _CONFIG_H_ */
