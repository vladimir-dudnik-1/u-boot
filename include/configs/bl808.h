/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <config_distro_bootcmd.h>

#define CFG_SYS_INIT_RAM_ADDR	0x22020000
#define CFG_SYS_INIT_RAM_SIZE	0x00034000 // see GLB_Set_EM_Sel()

#endif /* __CONFIG_H */
