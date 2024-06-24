/*
 * Copyright (c) 2023 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This very simple hello world C code can be used as a test case for building
 * probably the simplest loadable extension. It requires a single symbol be
 * linked, section relocation support, and the ability to export and call out
 * to a function.
 *
 * Note that this is also used in the llext_find_section test case, so it is
 * important that number is the first symbol in the .data section.
 */

#include <stdint.h>
#include <zephyr/llext/symbol.h>
#include <zephyr/sys/printk.h>
#include <zephyr/ztest_assert.h>

uint32_t number = 42;

void test_entry(void)
{
	printk("hello world\n");
	printk("A number is %u\n", number);
	zassert_equal(number, 42);
}
LL_EXTENSION_SYMBOL(test_entry);

/* for llext_find_section test case */
LL_EXTENSION_SYMBOL(number);
