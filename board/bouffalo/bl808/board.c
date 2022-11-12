// SPDX-License-Identifier: GPL-2.0+

#include <asm/io.h>
#include <bl808/glb_reg.h>
#include <linux/bitops.h>

#define GLB_BASE			(void *)0x20000000

void board_debug_uart_init(void)
{
#ifdef CONFIG_DEBUG_UART
	// Set GPIO12 function to UART0, output enable
	writel((7 << 8) | BIT(6),
	       GLB_BASE + GLB_GPIO_CFG12_OFFSET);

	// Set GPIO13 function to UART0, input and pull up enable
	writel((7 << 8) | BIT(4) | BIT(0),
	       GLB_BASE + GLB_GPIO_CFG13_OFFSET);

	// Enable UART clock
	writel(readl(GLB_BASE + GLB_UART_CFG0_OFFSET) | BIT(4),
	       GLB_BASE + GLB_UART_CFG0_OFFSET);
#endif

	// Mux GPIO12 to UART0 TXD, GPIO13 to UART0 RXD
	writel((3 << 4) | (2 << 0),
	       GLB_BASE + GLB_UART_CFG1_OFFSET);
}

int board_early_init_f(void)
{
	board_debug_uart_init();

	return 0;
}

int board_init(void)
{
	return 0;
}
