#include <dm.h>
#include <ram.h>

#include "bl808_ef_cfg.h"
#include "bl808_psram_uhs.h"
#include "bl808_uhs_phy.h"

#define WB_4MB_PSRAM   (1)
#define UHS_32MB_PSRAM (2)
#define UHS_64MB_PSRAM (3)
#define WB_32MB_PSRAM  (4)
#define NONE_UHS_PSRAM (-1)

static PSRAM_UHS_Cfg_Type psramDefaultCfg = {
	2000,
	PSRAM_MEM_SIZE_32MB,
	PSRAM_PAGE_SIZE_2KB,
	PSRAM_UHS_NORMAL_TEMP,
};

static int uhs_psram_init(void)
{
	Efuse_Chip_Info_Type chip_info;
	EF_Ctrl_Get_Chip_Info(&chip_info);
	if (chip_info.psramInfo == UHS_32MB_PSRAM) {
		psramDefaultCfg.psramMemSize = PSRAM_MEM_SIZE_32MB;
	} else if (chip_info.psramInfo == UHS_64MB_PSRAM) {
		psramDefaultCfg.psramMemSize = PSRAM_MEM_SIZE_64MB;
	} else {
		return -1;
	}

	Efuse_Psram_Trim_Type uhs_psram_trim;
	EF_Ctrl_Read_Psram_Trim(&uhs_psram_trim);

	//init uhs PLL; Must open uhs pll first, and then initialize uhs psram
	GLB_Config_UHS_PLL(GLB_XTAL_40M, uhsPllCfg_2000M);
	//init uhs psram ;
	// Psram_UHS_x16_Init(Clock_Peripheral_Clock_Get(BL_PERIPHERAL_CLOCK_PSRAMA) / 1000000);
	Psram_UHS_x16_Init_Override(&psramDefaultCfg);

	// example: 2000Mbps typical cal values
	uhs_phy_cal_res->rl = 39;
	uhs_phy_cal_res->rdqs = 3;
	uhs_phy_cal_res->rdq = 0;
	uhs_phy_cal_res->wl = 13;
	uhs_phy_cal_res->wdqs = 4;
	uhs_phy_cal_res->wdq = 5;
	uhs_phy_cal_res->ck = 9;
	/* TODO: use uhs psram trim update */
	set_uhs_latency_r(uhs_phy_cal_res->rl);
	cfg_dqs_rx(uhs_phy_cal_res->rdqs);
	cfg_dq_rx(uhs_phy_cal_res->rdq);
	set_uhs_latency_w(uhs_phy_cal_res->wl);
	cfg_dq_drv(uhs_phy_cal_res->wdq);
	cfg_ck_cen_drv(uhs_phy_cal_res->wdq + 4, uhs_phy_cal_res->wdq + 1);
	cfg_dqs_drv(uhs_phy_cal_res->wdqs);
	// set_odt_en();
	mr_read_back();

	return 0;
}

static int bflb_psram_get_info(struct udevice *dev, struct ram_info *info)
{
	info->base = 0x50000000;
	info->size = (psramDefaultCfg.psramMemSize + 1) * 1024 * 1024;

	return 0;
}

static const struct ram_ops bflb_psram_ops = {
	.get_info = bflb_psram_get_info,
};

static int bflb_psram_probe(struct udevice *dev)
{
	return uhs_psram_init();
}

static const struct udevice_id bflb_psram_ids[] = {
	{ .compatible = "bflb,bl808-psram-uhs" },
	{ }
};

U_BOOT_DRIVER(bflb_psram) = {
	.name		= "bflb_psram",
	.id		= UCLASS_RAM,
	.of_match	= bflb_psram_ids,
	.probe		= bflb_psram_probe,
	.ops		= &bflb_psram_ops,
	.flags		= DM_FLAG_PRE_RELOC,
};
