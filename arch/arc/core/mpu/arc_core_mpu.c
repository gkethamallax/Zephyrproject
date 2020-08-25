/*
 * Copyright (c) 2017 Synopsys.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <init.h>
#include <kernel.h>
#include <soc.h>
#include <arch/arc/v2/mpu/arc_core_mpu.h>
#include <kernel_structs.h>

/*
 * @brief Configure MPU for the thread
 *
 * This function configures per thread memory map reprogramming the MPU.
 *
 * @param thread thread info data structure.
 */
void configure_mpu_thread(struct k_thread *thread)
{
	arc_core_mpu_disable();
	arc_core_mpu_configure_thread(thread);
	arc_core_mpu_enable();
}

#if defined(CONFIG_USERSPACE)

int arch_mem_domain_max_partitions_get(void)
{
	return arc_core_mpu_get_max_domain_partition_regions();
}

/*
 * Reset MPU region for a single memory partition
 */
void arch_mem_domain_partition_remove(struct k_mem_domain *domain,
				      uint32_t partition_id)
{
	/* No-op on this architecture */
}

/*
 * Configure MPU memory domain
 */
void arch_mem_domain_thread_add(struct k_thread *thread)
{
	/* No-op on this architecture */
}

/*
 * Destroy MPU regions for the mem domain
 */
void arch_mem_domain_destroy(struct k_mem_domain *domain)
{
	/* No-op on this architecture */
}

void arch_mem_domain_partition_add(struct k_mem_domain *domain,
				   uint32_t partition_id)
{
	/* No-op on this architecture */
}

void arch_mem_domain_thread_remove(struct k_thread *thread)
{
	/* No-op on this architecture */
}

/*
 * Validate the given buffer is user accessible or not
 */
int arch_buffer_validate(void *addr, size_t size, int write)
{
	return arc_core_mpu_buffer_validate(addr, size, write);
}

#endif
