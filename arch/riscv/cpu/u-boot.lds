/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2017 Andes Technology Corporation
 * Rick Chen, Andes Technology Corporation <rick@andestech.com>
 */

#include <config.h>

OUTPUT_ARCH("riscv")
ENTRY(_start)

#ifdef CONFIG_XIP
MEMORY { .xip : ORIGIN = CONFIG_TEXT_BASE, LENGTH = CONFIG_SYS_MONITOR_LEN }
MEMORY { .ram : ORIGIN = CFG_SYS_INIT_RAM_ADDR, LENGTH = CFG_SYS_INIT_RAM_SIZE }
#define TO_XIP >.xip
#define TO_RAM >.ram AT>.xip
#else
#define TO_XIP
#define TO_RAM
#endif

SECTIONS
{
	. = ALIGN(4);
	.text : {
		arch/riscv/cpu/start.o	(.text)
	} TO_XIP

	/* This needs to come before *(.text*) */
	.efi_runtime : {
		__efi_runtime_start = .;
		*(.text.efi_runtime*)
		*(.rodata.efi_runtime*)
		*(.data.efi_runtime*)
		__efi_runtime_stop = .;
	} TO_XIP

	.text_rest : {
		*(.text*)
	} TO_XIP

	. = ALIGN(4);
	.rodata : { *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) } TO_XIP

	. = ALIGN(4);

	.got : {
		__got_start = .;
		*(.got.plt) *(.got)
		__got_end = .;
	} TO_XIP

	. = ALIGN(4);

	.data : {
		__data_start = .;
		*(.data*)
	} TO_RAM
	__data_load_start = LOADADDR(.data);

	. = ALIGN(4);

	__u_boot_list : {
		KEEP(*(SORT(__u_boot_list*)));
	} TO_RAM

	. = ALIGN(4);

	.efi_runtime_rel : {
		__efi_runtime_rel_start = .;
		*(.rel*.efi_runtime)
		*(.rel*.efi_runtime.*)
		__efi_runtime_rel_stop = .;
	} TO_RAM

	. = ALIGN(4);

	/DISCARD/ : { *(.rela.plt*) }
	.rela.dyn : {
		__rel_dyn_start = .;
		*(.rela*)
		__rel_dyn_end = .;
	} TO_RAM

	. = ALIGN(4);

	.dynsym : {
		__dyn_sym_start = .;
		*(.dynsym)
		__dyn_sym_end = .;
	} TO_RAM

	. = ALIGN(4);

	_end = .;

	.bss : {
		__bss_start = .;
		*(.bss*)
		. = ALIGN(8);
		__bss_end = .;
	} TO_RAM
}
