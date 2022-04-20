#include <common.h>
#include <cpu.h>
#include <dm.h>
#include <fdt_support.h>
#include <ram.h>
#include <spl.h>
#include <asm/csr.h>
#include <asm/io.h>

#define CSR_MXSTATUS		0x7c0
#define CSR_MHCR		0x7c1
#define CSR_MCOR		0x7c2
#define CSR_MHINT		0x7c5

#define SUNXI_CCU_BASE		((void *)0x2001000)
#define SUNXI_RISCV_CFG_BGR_REG			0xd0c

#define SUNXI_SID_BASE		((void *)0x3006000)
#define SUNXI_AUDIO_CODEC	((void *)0x2030000)

#define SUNXI_RISCV_CFG_BASE	((void *)0x6010000)
#define SUNXI_RESET_ENTRY_LOW			0x04
#define SUNXI_RESET_ENTRY_HIGH			0x08
#define SUNXI_WAKEUP_EN_REG			0x20
#define SUNXI_WAKEUP_MASK_REG(i)		(0x24 + 4 * (i))

#define SUNXI_PPU_BASE		((void *)0x7001000)
#define SUNXI_PD_ACTIVE_CTRL			0x2c

#define SUNXI_RPRCM_BASE	((void *)0x7010000)
#define SUNXI_RISCV_PPU_BUS_GATING		0x1ac

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	/* https://lore.kernel.org/u-boot/31587574-4cd1-02da-9761-0134ac82b94b@sholland.org/ */
	return cpu_probe_all();
}

void *board_spl_fit_buffer_addr(ulong fit_size, int sectors, int bl_len)
{
	return (void *)0x40200000;
}


int spl_board_init_f(void)
{
	int ret;
	struct udevice *dev;

	/* Trim bandgap reference voltage. */
	u32 bg_trim = (readl(SUNXI_SID_BASE + 0x228) >> 16) & 0xff;
	if (!bg_trim)
		bg_trim = 0x19;
	clrsetbits_le32(SUNXI_AUDIO_CODEC + 0x348, 0xff, bg_trim);

	/* DDR init */
	ret = uclass_get_device(UCLASS_RAM, 0, &dev);
	if (ret) {
		debug("DRAM init failed: %d\n", ret);
		return ret;
	}

	/* Initialize extension CSRs. */
	printf("mxstatus=0x%08lx mhcr=0x%08lx mcor=0x%08lx mhint=0x%08lx\n",
	       csr_read(CSR_MXSTATUS),
	       csr_read(CSR_MHCR),
	       csr_read(CSR_MCOR),
	       csr_read(CSR_MHINT));

	csr_set(CSR_MXSTATUS, 0x638000);
	csr_write(CSR_MCOR, 0x70013);
	csr_write(CSR_MHCR, 0x11ff);
	csr_write(CSR_MHINT, 0x16e30c);

	/* Initialize RISCV_CFG. */
	writel(0x10001, SUNXI_CCU_BASE + SUNXI_RISCV_CFG_BGR_REG);
	for (int i = 0; i < 5; ++i)
		writel(0xffffffff, SUNXI_RISCV_CFG_BASE + SUNXI_WAKEUP_MASK_REG(i));

	return 0;
}

uint32_t spl_boot_device(void)
{
	return BOOT_DEVICE_MMC1;
}

#ifdef CONFIG_SPL_BUILD
void spl_perform_fixups(struct spl_image_info *spl_image)
{
	struct ram_info info;
	struct udevice *dev;
	int ret;

	ret = uclass_get_device(UCLASS_RAM, 0, &dev);
	if (ret)
		panic("No RAM device");

	ret = ram_get_info(dev, &info);
	if (ret)
		panic("No RAM info");

	ret = fdt_fixup_memory(spl_image->fdt_addr, info.base, info.size);
	if (ret)
		panic("Failed to update DTB");
}
#endif
