/*
 * Copyright (c) 2018 - 2019 Antmicro <www.antmicro.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __RISCV32_LITEX_VEXRISCV_SOC_H_
#define __RISCV32_LITEX_VEXRISCV_SOC_H_

#include "../riscv-privilege/common/soc_common.h"
#include <generated_dts_board.h>

/* lib-c hooks required RAM defined variables */
#define RISCV_RAM_BASE              DT_INST_0_MMIO_SRAM_BASE_ADDRESS
#define RISCV_RAM_SIZE              DT_INST_0_MMIO_SRAM_SIZE

#ifndef _ASMLANGUAGE
/* CSR access helpers */

static inline unsigned char litex_read8(unsigned long addr)
{
	return sys_read8(addr);
}

static inline unsigned short litex_read16(unsigned long addr)
{
	return (sys_read8(addr) << 8)
		| sys_read8(addr + 0x4);
}

static inline unsigned int litex_read32(unsigned long addr)
{
	return (sys_read8(addr) << 24)
		| (sys_read8(addr + 0x4) << 16)
		| (sys_read8(addr + 0x8) << 8)
		| sys_read8(addr + 0xc);
}

static inline void litex_write8(unsigned char value, unsigned long addr)
{
	sys_write8(value, addr);
}

static inline void litex_write16(unsigned short value, unsigned long addr)
{
	sys_write8(value >> 8, addr);
	sys_write8(value, addr + 0x4);
}

static inline void litex_write32(unsigned int value, unsigned long addr)
{
	sys_write8(value >> 24, addr);
	sys_write8(value >> 16, addr + 0x4);
	sys_write8(value >> 8, addr + 0x8);
	sys_write8(value, addr + 0xC);
}

/* `reg_size` is assumed to be a multiple of 4 */
static inline void litex_write(volatile u32_t *reg, u32_t reg_size, u32_t val)
{
	u32_t shifted_data;
	volatile u32_t *reg_addr;
	u32_t subregs = reg_size / 4;

	for (int i = 0; i < subregs; ++i) {
		shifted_data = val >> ((subregs - i - 1) * 8);
		reg_addr = ((volatile u32_t *) reg) + i;
		*(reg_addr) = shifted_data;
	}
}

/* `reg_size` is assumed to be a multiple of 4 */
static inline u32_t litex_read(volatile u32_t *reg, u32_t reg_size)
{
	u32_t shifted_data;
	volatile u32_t *reg_addr;
	u32_t result = 0;
	u32_t subregs = reg_size / 4;

	for (int i = 0; i < subregs; ++i) {
		reg_addr = ((volatile u32_t *) reg) + i;
		shifted_data = *(reg_addr);
		result |= (shifted_data << ((subregs - i - 1) * 8));
	}

	return result;
}

#endif /* _ASMLANGUAGE */

#endif /* __RISCV32_LITEX_VEXRISCV_SOC_H_ */
