/*
 * Copyright (c) 2016 Wind River Systems, Inc.
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 *
 * System workqueue.
 */

#include <kernel.h>
#include <init.h>

static K_KERNEL_STACK_DEFINE(sys_work_q_stack,
			     CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE);

struct k_work_q k_sys_work_q;

static int k_sys_work_q_init(const struct device *dev)
{
	ARG_UNUSED(dev);
	struct k_work_queue_config cfg = {
		.name = "sysworkq",
		.no_yield = IS_ENABLED(CONFIG_SYSTEM_WORKQUEUE_NO_YIELD),
	};

#ifdef CONFIG_KERNEL_WORK1
	k_work_q_start(&k_sys_work_q,
		       sys_work_q_stack,
		       K_KERNEL_STACK_SIZEOF(sys_work_q_stack),
		       CONFIG_SYSTEM_WORKQUEUE_PRIORITY);
	k_thread_name_set(&k_sys_work_q.thread, "sysworkq");
#else /* CONFIG_KERNEL_WORK1 */
	k_work_queue_start(&k_sys_work_q,
			    sys_work_q_stack,
			    K_KERNEL_STACK_SIZEOF(sys_work_q_stack),
			    CONFIG_SYSTEM_WORKQUEUE_PRIORITY, &cfg);
#endif /* CONFIG_KERNEL_WORK1 */

	return 0;
}

SYS_INIT(k_sys_work_q_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
