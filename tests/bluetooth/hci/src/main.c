/* main.c - Application main entry point */

/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <stddef.h>
#include <zephyr/ztest.h>

#include <zephyr/bluetooth/hci.h>

ZTEST_SUITE(test_hci, NULL, NULL, NULL, NULL, NULL);

ZTEST(test_hci, test_bt_hci_err_to_str)
{
	/* Test a couple of entries */
	zassert_str_equal(bt_hci_err_to_str(BT_HCI_ERR_CONN_TIMEOUT),
			  "BT_HCI_ERR_CONN_TIMEOUT(0x08)");
	zassert_str_equal(bt_hci_err_to_str(BT_HCI_ERR_REMOTE_USER_TERM_CONN),
			  "BT_HCI_ERR_REMOTE_USER_TERM_CONN(0x13)");
	zassert_str_equal(bt_hci_err_to_str(BT_HCI_ERR_TOO_EARLY),
			  "BT_HCI_ERR_TOO_EARLY(0x47)");

	/* Test a entries that is not used */
	zassert_str_equal(bt_hci_err_to_str(0x2b),
			  "(unknown)(0x2b)");
	zassert_str_equal(bt_hci_err_to_str(0xFF),
			  "(unknown)(0xff)");

	for (uint16_t i = 0; i <= UINT8_MAX; i++) {
		zassert_not_null(bt_hci_err_to_str(i), ": %d", i);
	}
}
