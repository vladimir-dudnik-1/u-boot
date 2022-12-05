// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2018, Bin Meng <bmeng.cn@gmail.com>
 */

#include <common.h>
#include <fdtdec.h>
#include <init.h>
#include <ram.h>
#include <asm/global_data.h>
#include <dm/uclass.h>
#include <linux/sizes.h>

DECLARE_GLOBAL_DATA_PTR;

#if IS_ENABLED(CONFIG_RAM) && !IS_ENABLED(CONFIG_SPL)
int dram_init(void)
{
	struct ram_info ram;
	struct udevice *dev;
	int ret;

	ret = uclass_get_device(UCLASS_RAM, 0, &dev);
	if (ret) {
		debug("DRAM init failed: %d\n", ret);
		return ret;
	}

	ret = ram_get_info(dev, &ram);
	if (ret) {
		debug("Cannot get DRAM size: %d\n", ret);
		return ret;
	}

	gd->ram_base = ram.base;
	gd->ram_size = ram.size;

	return 0;
}
#else
int dram_init(void)
{
	return fdtdec_setup_mem_size_base();
}

int dram_init_banksize(void)
{
	return fdtdec_setup_memory_banksize();
}
#endif

phys_size_t board_get_usable_ram_top(phys_size_t total_size)
{
	/*
	 * Ensure that we run from first 4GB so that all
	 * addresses used by U-Boot are 32bit addresses.
	 *
	 * This in-turn ensures that 32bit DMA capable
	 * devices work fine because DMA mapping APIs will
	 * provide 32bit DMA addresses only.
	 */
	if (gd->ram_top >= SZ_4G)
		return SZ_4G - 1;

	return gd->ram_top;
}
