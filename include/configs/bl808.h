/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <config_distro_bootcmd.h>

#ifdef CONFIG_BL808_CPU_M0
#define CFG_SYS_INIT_RAM_ADDR	0x22020000
#define CFG_SYS_INIT_RAM_SIZE	0x00034000 // see GLB_Set_EM_Sel()
#else
#define CFG_SYS_INIT_RAM_ADDR	0x50100000
#define CFG_SYS_INIT_RAM_SIZE	0x00100000
#endif

#endif /* __CONFIG_H */
