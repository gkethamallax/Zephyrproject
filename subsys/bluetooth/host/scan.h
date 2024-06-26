/*
 * Copyright (c) 2017-2021 Nordic Semiconductor ASA
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SUBSYS_BLUETOOTH_HOST_SCAN_H_
#define SUBSYS_BLUETOOTH_HOST_SCAN_H_

#include <stdint.h>

#include <zephyr/sys/atomic.h>
#include <zephyr/bluetooth/bluetooth.h>

/**
 * Reasons why a scanner can be running.
 * Used as input to @ref bt_le_scan_user_add
 * and @ref bt_le_scan_user_remove.
 */
enum bt_le_scan_enabled_flag {
	/** The application explicitly instructed the stack to scan for advertisers
	 * using the API @ref bt_le_scan_start().
	 * May not be used explicitly.
	 */
	BT_LE_SCAN_USER_EXPLICIT_SCAN,

	/**
	 * Syncing to a periodic advertiser.
	 */
	BT_LE_SCAN_USER_SYNC_SYNCING,

	/**
	 * Scanning to find devices to connect to.
	 */
	BT_LE_SCAN_USER_SCAN_BEFORE_INITIATE,

	/**
	 * Special state for a NOP for @ref bt_le_scan_update_and_reconfigure to not add/remove any
	 * flag why the scanner should be running.
	 */
	BT_LE_SCAN_USER_NONE,
	BT_LE_SCAN_USER_NUM_FLAGS,
};

void bt_scan_reset(void);

bool bt_id_scan_random_addr_check(void);
bool bt_le_scan_active_scanner_running(void);

int bt_le_scan_set_enable(uint8_t enable);

struct bt_le_per_adv_sync *bt_hci_get_per_adv_sync(uint16_t handle);

void bt_periodic_sync_disable(void);

/**
 * Start / update the scanner.
 *
 * This API updates the users of the scanner.
 * Multiple users can be enabled at the same time.
 * Depending on all the users, scan parameters are selected
 * and the scanner is started or updated, if needed.
 * This API may update the scan parameters, for example if the scanner is already running
 * when another user that demands higher duty-cycle is being added.
 *
 * Every SW module that informs the scanner that it should run, needs to eventually remove
 * the flag again using @ref bt_le_scan_user_remove once it does not require
 * the scanner to run, anymore.
 *
 * @param flag user requesting the scanner
 *
 * @return 0 in case of success, or a negative error code on failure.
 */
int bt_le_scan_user_add(enum bt_le_scan_enabled_flag flag);

/**
 * Stop / update the scanner.
 *
 * This API updates the users of the scanner.
 * Depending on all enabled users, scan parameters are selected
 * and the scanner is stopped or updated, if needed.
 * This API may update the scan parameters, for example if the scanner is already running
 * when a user that demands higher duty-cycle is being removed.
 *
 * This API allows removing the user why the scanner is running.
 * If all users for the scanner to run are removed, this API will stop the scanner.
 *
 * @param flag user releasing the scanner
 *
 * @return 0 in case of success, or a negative error code on failure.
 */
int bt_le_scan_user_remove(enum bt_le_scan_enabled_flag flag);

/**
 * Check if the explicit scanner was enabled.
 */
bool bt_le_explicit_scanner_running(void);
#endif /* defined SUBSYS_BLUETOOTH_HOST_SCAN_H_ */
