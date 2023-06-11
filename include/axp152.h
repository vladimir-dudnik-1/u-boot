/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2012 Henrik Nordstrom <henrik@henriknordstrom.net>
 */

enum axp152_reg {
	AXP152_CHIP_VERSION = 0x3,
	AXP152_DCDC2_VOLTAGE = 0x23,
	AXP152_DCDC3_VOLTAGE = 0x27,
	AXP152_DCDC4_VOLTAGE = 0x2B,
	AXP152_LDO2_VOLTAGE = 0x2A,
	AXP152_SHUTDOWN = 0x32,
};

#define AXP152_POWEROFF			(1 << 7)

#ifdef CONFIG_AXP152_POWER
#define AXP_POWER_STATUS		0x00
#define AXP_POWER_STATUS_ALDO_IN		BIT(0)
#endif
