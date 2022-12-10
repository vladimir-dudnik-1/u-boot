// SPDX-License-Identifier: GPL-2.0+

#include <clk-uclass.h>
#include <dm.h>
#include <asm/io.h>
#include <bl808/glb_reg.h>
#include <bl808/hbn_reg.h>
#include <bl808/mm_glb_reg.h>
#include <bl808/pds_reg.h>
#include <clk/bl808.h>
#include <dm/device-internal.h>
#include <dm/lists.h>
#include <dt-bindings/clock/bl808-glb.h>
#include <dt-bindings/clock/bl808-hbn.h>
#include <dt-bindings/clock/bl808-mm-glb.h>
#include <dt-bindings/clock/bl808-pds.h>
#include <dt-bindings/reset/bl808-mm-glb.h>

#define PRNTS(...) (const u8[]) { __VA_ARGS__ }

enum {
	FW_PARENT_BASE		= 0x80,
	// Fixed clocks
	FW_EXT_XTAL		= FW_PARENT_BASE,
	FW_EXT_XTAL32K,
	// GLB
	FW_BCLK,
	FW_DIG_32K,
	FW_AUPLL_DIV1,
	FW_CPUPLL_400M,
	FW_DSPPLL,
	FW_MUXPLL_160M,
	FW_MUXPLL_240M,
	FW_MUXPLL_320M,
	FW_WIFIPLL_240M,
	FW_WIFIPLL_320M,
	// HBN
	FW_XCLK,
	FW_HBN_ROOT,
	FW_HBN_UART,
	FW_XTAL,
	// PDS
	FW_RC32M,
	FW_PDS_PLL,
	FW_PARENT_MAX,
	NO_PARENT		= 0xff,
};

static const char *const fw_parent_names[FW_PARENT_MAX-FW_PARENT_BASE] = {
	// Fixed clocks
	"ext_xtal",
	"ext_xtal32k",
	// GLB
	"bclk",
	"dig_32k",
	"aupll_div1",
	"cpupll_400m",
	"dsppll",
	"muxpll_160m",
	"muxpll_240m",
	"muxpll_320m",
	"wifipll_240m",
	"wifipll_320m",
	// HBN
	"xclk",
	"hbn_root",
	"hbn_uart",
	"xtal",
	// PDS
	"rc32m",
	"pds_pll",
};

static const struct bl808_clk_data bl808_glb_clks[] = {
	[CLK_CPU] = {
		.name		= "CPU",
		.parents	= PRNTS(FW_HBN_ROOT),
		.div_reg	= GLB_SYS_CFG0_OFFSET,
		.div_mask	= GLB_REG_HCLK_DIV_MSK,
	},
	[CLK_HCLK] = {
		.name		= "HCLK",
		.parents	= PRNTS(CLK_CPU),
		.en_reg		= GLB_SYS_CFG0_OFFSET,
		.en_mask	= GLB_REG_HCLK_EN_MSK,
	},
	[CLK_BCLK] = {
		.name		= "BCLK",
		.parents	= PRNTS(CLK_CPU),
		/* Must set GLB_REG_BCLK_DIV_ACT_PULSE to update */
		.div_reg	= GLB_SYS_CFG0_OFFSET,
		.div_mask	= GLB_REG_BCLK_DIV_MSK,
		.en_reg		= GLB_SYS_CFG0_OFFSET,
		.en_mask	= GLB_REG_BCLK_EN_MSK,
	},
	[CLK_UART] = {
		.name		= "UART",
		.parents	= PRNTS(FW_HBN_UART),
		.div_reg	= GLB_UART_CFG0_OFFSET,
		.div_mask	= GLB_UART_CLK_DIV_MSK,
		.en_reg		= GLB_UART_CFG0_OFFSET,
		.en_mask	= GLB_UART_CLK_EN_MSK,
	},
	[CLK_DSPPLL] = {
		.name		= "DSPPLL",
	},
	[CLK_MM_MUXPLL_160M] = {
		.name		= "MM_MUXPLL_160M",
		.rate		= 160000000,
	},
	[CLK_MM_MUXPLL_240M] = {
		.name		= "MM_MUXPLL_240M",
		.rate		= 240000000,
	},
	[CLK_MM_MUXPLL_320M] = {
		.name		= "MM_MUXPLL_320M",
		.rate		= 320000000,
	},
	[CLK_TOP_MUXPLL_160M] = {
		.name		= "TOP_MUXPLL_160M",
		.rate		= 160000000,
	},
	[CLK_SDH] = {
		.name		= "SDH",
		.parents	= PRNTS(CLK_WIFIPLL_DIV5,
					CLK_TOP_CPUPLL_100M),
		.sel_reg	= GLB_SDH_CFG0_OFFSET,
		.sel_mask	= GLB_REG_SDH_CLK_SEL_MSK,
		.div_reg	= GLB_SDH_CFG0_OFFSET,
		.div_mask	= GLB_REG_SDH_CLK_DIV_MSK,
		.en_reg		= GLB_SDH_CFG0_OFFSET,
		.en_mask	= GLB_REG_SDH_CLK_EN_MSK,
	},
	[CLK_TOP_CPUPLL_100M] = {
		.name		= "TOP_CPUPLL_100M",
		.rate		= 100000000,
	},
	[CLK_TOP_CPUPLL_400M] = {
		.name		= "TOP_CPUPLL_400M",
		.rate		= 400000000,
	},
	[CLK_TOP_WIFIPLL_240M] = {
		.name		= "TOP_WIFIPLL_240M",
		.rate		= 240000000,
	},
	[CLK_TOP_WIFIPLL_320M] = {
		.name		= "TOP_WIFIPLL_320M",
		.rate		= 320000000,
	},
	[CLK_WIFIPLL] = {
		.name		= "WIFIPLL",
		.rate		= 480000000,
	},
	[CLK_WIFIPLL_DIV5] = {
		.name		= "WIFIPLL_DIV5",
		.parents	= PRNTS(CLK_WIFIPLL),
		.en_reg		= GLB_WIFI_PLL_CFG8_OFFSET,
		.en_mask	= GLB_WIFIPLL_EN_DIV5_MSK,
		.fixed_div	= 5,
	},
	[CLK_USB] = {
		.name		= "USB",
		.en_reg		= GLB_CGEN_CFG1_OFFSET,
		.en_mask	= GLB_CGEN_S1_RSVD13_MSK,
	},
};

static const struct bl808_clk_desc bl808_glb_clk_desc = {
	.clks		= bl808_glb_clks,
	.num_clks	= ARRAY_SIZE(bl808_glb_clks),
};

static const struct bl808_clk_data bl808_hbn_clks[] = {
	[CLK_XCLK] = {
		.name		= "XCLK",
		.parents	= PRNTS(FW_RC32M,
					CLK_XTAL),
		.sel_reg	= HBN_GLB_OFFSET,
		.sel_mask	= BIT(0), /* HBN_ROOT_CLK_SEL[0] */
	},
	[CLK_HBN_ROOT] = {
		.name		= "HBN_ROOT",
		.parents	= PRNTS(CLK_XCLK,
					FW_PDS_PLL),
		.sel_reg	= HBN_GLB_OFFSET,
		.sel_mask	= BIT(1), /* HBN_ROOT_CLK_SEL[1] */
	},
	[CLK_HBN_UART_SEL] = {
		.name		= "HBN_UART_SEL",
		.parents	= PRNTS(FW_BCLK,
					FW_MUXPLL_160M),
		.sel_reg	= HBN_GLB_OFFSET,
		.sel_mask	= HBN_UART_CLK_SEL_MSK,
	},
	[CLK_F32K] = {
		.name		= "F32K",
		.parents	= PRNTS(CLK_RC32K,
					CLK_XTAL32K,
					FW_DIG_32K,
					NO_PARENT),
		.sel_reg	= HBN_GLB_OFFSET,
		.sel_mask	= HBN_F32K_SEL_MSK,
	},
	[CLK_HBN_UART] = {
		.name		= "HBN_UART",
		.parents	= PRNTS(CLK_HBN_UART_SEL,
					CLK_XCLK),
		.sel_reg	= HBN_GLB_OFFSET,
		.sel_mask	= HBN_UART_CLK_SEL2_MSK,
	},
	[CLK_RC32K] = {
		.name		= "RC32K",
		.rate		= 32000,
	},
	[CLK_XTAL32K] = {
		.name		= "XTAL32K",
		.parents	= PRNTS(FW_EXT_XTAL32K),
	},
	[CLK_XTAL] = {
		.name		= "XTAL",
		.parents	= PRNTS(FW_EXT_XTAL),
	},
};

static const struct bl808_clk_desc bl808_hbn_clk_desc = {
	.clks		= bl808_hbn_clks,
	.num_clks	= ARRAY_SIZE(bl808_hbn_clks),
};

static const struct bl808_clk_data bl808_mm_glb_clks[] = {
	[CLK_MM_UART] = {
		.name		= "MM_UART",
		.parents	= PRNTS(CLK_MM_BCLK1X,
					FW_MUXPLL_160M,
					CLK_MM_XCLK,
					NO_PARENT),
		.sel_reg	= MM_GLB_MM_CLK_CTRL_CPU_OFFSET,
		.sel_mask	= MM_GLB_REG_UART_CLK_SEL_MSK,
	},
	[CLK_MM_I2C] = {
		.name		= "MM_I2C",
		.parents	= PRNTS(CLK_MM_BCLK1X,
					CLK_MM_XCLK),
		.sel_reg	= MM_GLB_MM_CLK_CTRL_CPU_OFFSET,
		.sel_mask	= MM_GLB_REG_I2C_CLK_SEL_MSK,
	},
	[CLK_MM_SPI] = {
		.name		= "MM_SPI",
		.parents	= PRNTS(FW_MUXPLL_160M,
					CLK_MM_XCLK),
		.sel_reg	= MM_GLB_MM_CLK_CTRL_CPU_OFFSET,
		.sel_mask	= MM_GLB_REG_SPI_CLK_SEL_MSK,
	},
	[CLK_MM_MUXPLL] = {
		.name		= "MM_MUXPLL",
		.parents	= PRNTS(FW_MUXPLL_240M,
					FW_MUXPLL_320M,
					FW_CPUPLL_400M,
					FW_CPUPLL_400M),
		.sel_reg	= MM_GLB_MM_CLK_CTRL_CPU_OFFSET,
		.sel_mask	= MM_GLB_REG_CPU_CLK_SEL_MSK,
	},
	[CLK_MM_XCLK] = {
		.name		= "MM_XCLK",
		.parents	= PRNTS(FW_RC32M,
					FW_XTAL),
		.sel_reg	= MM_GLB_MM_CLK_CTRL_CPU_OFFSET,
		.sel_mask	= MM_GLB_REG_XCLK_CLK_SEL_MSK,
	},
	[CLK_MM_CPU] = {
		.name		= "MM_CPU",
		.parents	= PRNTS(CLK_MM_XCLK,
					CLK_MM_MUXPLL),
		.sel_reg	= MM_GLB_MM_CLK_CTRL_CPU_OFFSET,
		.sel_mask	= MM_GLB_REG_CPU_ROOT_CLK_SEL_MSK,
		.div_reg	= MM_GLB_MM_CLK_CPU_OFFSET,
		.div_mask	= MM_GLB_REG_CPU_CLK_DIV_MSK,
		.en_reg		= MM_GLB_MM_CLK_CTRL_CPU_OFFSET,
		.en_mask	= MM_GLB_REG_MMCPU0_CLK_EN_MSK,
	},
	[CLK_MM_BCLK1X] = {
		.name		= "MM_BCLK1X",
		.parents	= PRNTS(CLK_MM_XCLK,
					CLK_MM_XCLK,
					FW_MUXPLL_160M,
					FW_MUXPLL_240M),
		.sel_reg	= MM_GLB_MM_CLK_CTRL_CPU_OFFSET,
		.sel_mask	= MM_GLB_REG_BCLK1X_SEL_MSK,
		.div_reg	= MM_GLB_MM_CLK_CPU_OFFSET,
		.div_mask	= MM_GLB_REG_BCLK1X_DIV_MSK,
	},
	[CLK_MM_BCLK2X] = {
		.name		= "MM_BCLK2X",
		.parents	= PRNTS(CLK_MM_CPU),
		/* Must set MM_GLB_REG_BCLK2X_DIV_ACT_PULSE to update */
		.div_reg	= MM_GLB_MM_CLK_CPU_OFFSET,
		.div_mask	= MM_GLB_REG_BCLK2X_DIV_MSK,
	},
	[CLK_MM_CNN] = {
		.name		= "MM_CNN",
		.parents	= PRNTS(FW_MUXPLL_160M,
					FW_MUXPLL_240M,
					FW_MUXPLL_320M,
					FW_MUXPLL_320M),
		.sel_reg	= MM_GLB_MM_CLK_CPU_OFFSET,
		.sel_mask	= MM_GLB_REG_CNN_CLK_SEL_MSK,
		.div_reg	= MM_GLB_MM_CLK_CPU_OFFSET,
		.div_mask	= MM_GLB_REG_CNN_CLK_DIV_MSK,
		.en_reg		= MM_GLB_MM_CLK_CPU_OFFSET,
		.en_mask	= MM_GLB_REG_CNN_CLK_DIV_EN_MSK,
	},
	[CLK_MM_DSP] = {
		.name		= "MM_DSP",
		.parents	= PRNTS(FW_MUXPLL_160M,
					FW_MUXPLL_240M,
					FW_CPUPLL_400M,
					CLK_MM_XCLK),
		.sel_reg	= MM_GLB_DP_CLK_OFFSET,
		.sel_mask	= MM_GLB_REG_CLK_SEL_MSK,
		.div_reg	= MM_GLB_DP_CLK_OFFSET,
		.div_mask	= MM_GLB_REG_CLK_DIV_MSK,
		.en_reg		= MM_GLB_DP_CLK_OFFSET,
		.en_mask	= MM_GLB_REG_CLK_DIV_EN_MSK,
	},
	[CLK_MM_DSP_DP] = {
		.name		= "MM_DSP_DP",
		.parents	= PRNTS(FW_DSPPLL,
					CLK_MM_XCLK),
		.sel_reg	= MM_GLB_DP_CLK_OFFSET,
		.sel_mask	= MM_GLB_REG_DP_CLK_SEL_MSK,
		.div_reg	= MM_GLB_DP_CLK_OFFSET,
		.div_mask	= MM_GLB_REG_DP_CLK_DIV_MSK,
		.en_reg		= MM_GLB_DP_CLK_OFFSET,
		.en_mask	= MM_GLB_REG_DP_CLK_DIV_EN_MSK,
	},
	[CLK_MM_H264] = {
		.name		= "MM_H264",
		.parents	= PRNTS(FW_MUXPLL_160M,
					FW_MUXPLL_240M,
					FW_MUXPLL_320M,
					FW_MUXPLL_320M),
		.sel_reg	= MM_GLB_CODEC_CLK_OFFSET,
		.sel_mask	= MM_GLB_REG_H264_CLK_SEL_MSK,
		.div_reg	= MM_GLB_CODEC_CLK_OFFSET,
		.div_mask	= MM_GLB_REG_H264_CLK_DIV_MSK,
		.en_reg		= MM_GLB_CODEC_CLK_OFFSET,
		.en_mask	= MM_GLB_REG_H264_CLK_DIV_EN_MSK,
	},
	[CLK_MM_IC20] = {
		.name		= "MM_IC20",
		.parents	= PRNTS(CLK_MM_I2C),
		.div_reg	= MM_GLB_MM_CLK_CTRL_PERI_OFFSET,
		.div_mask	= MM_GLB_REG_I2C0_CLK_DIV_MSK,
		.en_reg		= MM_GLB_MM_CLK_CTRL_PERI_OFFSET,
		.en_mask	= MM_GLB_REG_I2C0_CLK_DIV_EN_MSK |
				  MM_GLB_REG_I2C0_CLK_EN_MSK,
	},
	[CLK_MM_UART0] = {
		.name		= "MM_UART0",
		.parents	= PRNTS(CLK_MM_UART),
		.div_reg	= MM_GLB_MM_CLK_CTRL_PERI_OFFSET,
		.div_mask	= MM_GLB_REG_UART0_CLK_DIV_MSK,
		.en_reg		= MM_GLB_MM_CLK_CTRL_PERI_OFFSET,
		.en_mask	= MM_GLB_REG_UART0_CLK_DIV_EN_MSK,
	},
	[CLK_MM_SPI0] = {
		.name		= "MM_SPI0",
		.parents	= PRNTS(CLK_MM_SPI),
		.div_reg	= MM_GLB_MM_CLK_CTRL_PERI_OFFSET,
		.div_mask	= MM_GLB_REG_SPI_CLK_DIV_MSK,
		.en_reg		= MM_GLB_MM_CLK_CTRL_PERI_OFFSET,
		.en_mask	= MM_GLB_REG_SPI_CLK_DIV_EN_MSK,
	},
	[CLK_MM_I2C1] = {
		.name		= "MM_I2C1",
		.parents	= PRNTS(CLK_MM_I2C),
		.div_reg	= MM_GLB_MM_CLK_CTRL_PERI3_OFFSET,
		.div_mask	= MM_GLB_REG_I2C1_CLK_DIV_MSK,
		.en_reg		= MM_GLB_MM_CLK_CTRL_PERI3_OFFSET,
		.en_mask	= MM_GLB_REG_I2C1_CLK_DIV_EN_MSK,
	},
	[CLK_MM_UART1] = {
		.name		= "MM_UART1",
		.parents	= PRNTS(CLK_MM_UART),
		.div_reg	= MM_GLB_MM_CLK_CTRL_PERI3_OFFSET,
		.div_mask	= MM_GLB_REG_UART1_CLK_DIV_MSK,
		.en_reg		= MM_GLB_MM_CLK_CTRL_PERI3_OFFSET,
		.en_mask	= MM_GLB_REG_UART1_CLK_DIV_EN_MSK,
	},
};

static const struct bl808_reset_data bl808_mm_glb_resets[] = {
	[RST_MM_CPU]	= { MM_GLB_MM_SW_SYS_RESET_OFFSET,
			    MM_GLB_REG_CTRL_MMCPU0_RESET_POS },
};

static const struct bl808_clk_desc bl808_mm_glb_clk_desc = {
	.clks		= bl808_mm_glb_clks,
	.resets		= bl808_mm_glb_resets,
	.num_clks	= ARRAY_SIZE(bl808_mm_glb_clks),
	.num_resets	= ARRAY_SIZE(bl808_mm_glb_resets),
};

static const struct bl808_clk_data bl808_pds_clks[] = {
	[CLK_PDS_PLL] = {
		.name		= "PDS_PLL",
		.parents	= PRNTS(FW_CPUPLL_400M,
					FW_AUPLL_DIV1,
					FW_WIFIPLL_240M,
					FW_WIFIPLL_320M),
		.sel_reg	= PDS_CPU_CORE_CFG1_OFFSET,
		.sel_mask	= PDS_REG_PLL_SEL_MSK,
		.en_reg		= PDS_CPU_CORE_CFG1_OFFSET,
		.en_mask	= PDS_REG_MCU1_CLK_EN_MSK,
	},
	[CLK_RC32M] = {
		.name		= "RC32M",
		.rate		= 32000000,
	},
};

static const struct bl808_clk_desc bl808_pds_clk_desc = {
	.clks		= bl808_pds_clks,
	.num_clks	= ARRAY_SIZE(bl808_pds_clks),
};

static const struct udevice_id bl808_clk_ids[] = {
	{ .compatible = "bflb,bl808-glb-clk",
	  .data = (ulong)&bl808_glb_clk_desc },
	{ .compatible = "bflb,bl808-hbn-clk",
	  .data = (ulong)&bl808_hbn_clk_desc },
	{ .compatible = "bflb,bl808-mm-glb-clk",
	  .data = (ulong)&bl808_mm_glb_clk_desc },
	{ .compatible = "bflb,bl808-pds-clk",
	  .data = (ulong)&bl808_pds_clk_desc },
	{ }
};

static struct clk *
bl808_clk_get_current_parent(const struct clk *clk)
{
	struct clk *current_parents = dev_get_priv(clk->dev);

	return &current_parents[clk->id];
}

static const struct bl808_clk_data *
bl808_clk_get_data(const struct bl808_clk_plat *plat, const struct clk *clk)
{
	const struct bl808_clk_desc *desc = plat->desc;

	if (clk->id >= desc->num_clks)
		return NULL;

	return &desc->clks[clk->id];
}

static int bl808_clk_request(struct clk *clk)
{
	const struct bl808_clk_plat *plat = dev_get_plat(clk->dev);

	return bl808_clk_get_data(plat, clk) ? 0 : -EINVAL;
}

static u32 bl808_clk_count_parents(const struct bl808_clk_data *data)
{
	if (!data->sel_mask)
		return 1;

	return data->sel_mask / (data->sel_mask & -data->sel_mask) + 1;
}

static int bl808_clk_resolve_parent(struct udevice *dev,
				    const struct bl808_clk_data *data,
				    u32 sel, struct clk *out)
{
	struct clk parent = {};
	u8 id;

	if (!data->parents)
		return -ENOENT;

	id = data->parents[sel];
	if (id < FW_PARENT_BASE) {
		parent.dev = dev;
		parent.id = id;
	} else if (id < FW_PARENT_MAX) {
		const char *name = fw_parent_names[id - FW_PARENT_BASE];
		int ret;

		ret = clk_get_by_name(dev, name, &parent);
		if (ret)
			return ret;
	} else if (id == NO_PARENT) {
		return -ENOENT;
	} else {
		return -EINVAL;
	}

	*out = parent;

	return 0;
}

static ulong bl808_clk_calc_rate(struct clk *clk, ulong req,
				 u32 *best_sel, u32 *best_div)
{
	const struct bl808_clk_plat *plat = dev_get_plat(clk->dev);
	const struct bl808_clk_data *data = bl808_clk_get_data(plat, clk);
	u32 num_sels = bl808_clk_count_parents(data), num_divs = 1;
	ulong best_rate = 0;

	if (data->div_mask) {
		u32 div_lsb = data->div_mask & -data->div_mask;

		num_divs = data->div_mask / div_lsb + 1;
	}

	for (u32 sel = 0; sel < num_sels; ++sel) {
		struct clk parent;
		ulong parent_rate;
		int ret;

		ret = bl808_clk_resolve_parent(clk->dev, data, sel, &parent);
		if (ret)
			continue;

		parent_rate = clk_get_rate(&parent);
		if (IS_ERR_VALUE(parent_rate))
			continue;

		if (data->fixed_div)
			parent_rate /= data->fixed_div;

		for (u32 div = 0; div < num_divs; ++div) {
			ulong rate = parent_rate / (1 + div);

			if (rate > req || rate <= best_rate)
				continue;

			*best_sel = sel;
			*best_div = div;
			best_rate = rate;
		}
	}

	return best_rate;
}

static ulong bl808_clk_round_rate(struct clk *clk, ulong req)
{
	u32 sel, div;

	return bl808_clk_calc_rate(clk, req, &sel, &div);
}

static ulong bl808_clk_get_rate(struct clk *clk)
{
	const struct bl808_clk_plat *plat = dev_get_plat(clk->dev);
	const struct bl808_clk_data *data = bl808_clk_get_data(plat, clk);
	ulong rate;

	if (data->rate)
		return data->rate;

	rate = clk_get_parent_rate(clk);
	if (IS_ERR_VALUE(rate))
		return rate;

	if (data->fixed_div)
		rate /= data->fixed_div;

	if (data->div_mask) {
		u32 div = readl(plat->base + data->div_reg);

		div &= data->div_mask;
		div /= data->div_mask & -data->div_mask;

		rate /= 1 + div;
	}

	return rate;
}

static ulong bl808_clk_set_rate(struct clk *clk, ulong req)
{
	const struct bl808_clk_plat *plat = dev_get_plat(clk->dev);
	const struct bl808_clk_data *data = bl808_clk_get_data(plat, clk);
	u32 sel, div;
	ulong rate;

	rate = bl808_clk_calc_rate(clk, req, &sel, &div);
	if (!rate)
		return rate;

	if (data->sel_mask) {
		u32 sel_lsb = data->sel_mask & -data->sel_mask;

		clrsetbits_le32(plat->base + data->sel_reg,
				data->sel_mask, sel * sel_lsb);
	}

	if (data->div_mask) {
		u32 div_lsb = data->div_mask & -data->div_mask;

		clrsetbits_le32(plat->base + data->div_reg,
				data->div_mask, div * div_lsb);
	}

	return 0;
}

static struct clk *bl808_clk_get_parent(struct clk *clk)
{
	const struct bl808_clk_plat *plat = dev_get_plat(clk->dev);
	const struct bl808_clk_data *data = bl808_clk_get_data(plat, clk);
	struct clk *current_parent = bl808_clk_get_current_parent(clk);

	if (!clk_valid(current_parent)) {
		u32 sel = 0;
		int ret;

		if (data->sel_mask) {
			sel = readl(plat->base + data->sel_reg);
			sel &= data->sel_mask;
			sel /= data->sel_mask & -data->sel_mask;
		}

		ret = bl808_clk_resolve_parent(clk->dev, data, sel,
					       current_parent);
		if (ret)
			return ERR_PTR(ret);
	}

	return current_parent;
}

static int bl808_clk_set_parent(struct clk *clk, struct clk *parent)
{
	const struct bl808_clk_plat *plat = dev_get_plat(clk->dev);
	const struct bl808_clk_data *data = bl808_clk_get_data(plat, clk);
	struct clk *current_parent = bl808_clk_get_current_parent(clk);
	u32 num_sels = bl808_clk_count_parents(data);
	u32 sel, sel_lsb;
	struct clk p;
	int ret;

	for (sel = 0; sel < num_sels; ++sel) {
		ret = bl808_clk_resolve_parent(clk->dev, data, sel, &p);
		if (!ret && p.dev == parent->dev && p.id == parent->id)
			break;
	}
	if (sel == num_sels)
		return -ENOENT;

	if (data->sel_mask)
		clrsetbits_le32(plat->base + data->sel_reg,
				data->sel_mask, sel * sel_lsb);

	*current_parent = p;

	return 0;
}

static int bl808_clk_set_gate(struct clk *clk, bool enable)
{
	const struct bl808_clk_plat *plat = dev_get_plat(clk->dev);
	const struct bl808_clk_data *data = bl808_clk_get_data(plat, clk);

	if (!data)
		return -EINVAL;

	clrsetbits_le32(plat->base + data->en_reg,
			data->en_mask, enable ? data->en_mask : 0);

	return 0;
}

static int bl808_clk_enable(struct clk *clk)
{
	return bl808_clk_set_gate(clk, true);
}

static int bl808_clk_disable(struct clk *clk)
{
	return bl808_clk_set_gate(clk, false);
}

static void bl808_clk_dump(struct udevice *dev)
{
	const struct bl808_clk_plat *plat = dev_get_plat(dev);
	const struct bl808_clk_desc *desc = plat->desc;
	struct clk clk, *parent;

	printf("   %s (%s)\n", dev->name, dev_read_string(dev, "compatible"));
	printf("ID       NAME            PARENT         RATE    SEL DIV EN\n");
	printf("--+----------------+----------------+----------+---+---+--\n");

	clk.dev = dev;
	for (size_t id = 0; id < desc->num_clks; ++id) {
		const struct bl808_clk_data *data = &desc->clks[id];
		const char *parent_name;

		clk.id = id;
		parent = clk_get_parent(&clk);

		if (IS_ERR(parent))
			parent_name = "(none)";
		else if (parent->dev->driver != dev->driver)
			parent_name = parent->dev->name;
		else {
			const struct bl808_clk_plat *pplat = dev_get_plat(parent->dev);
			const struct bl808_clk_data *pdata = bl808_clk_get_data(pplat, parent);

			parent_name = pdata->name;
			if (!parent_name)
				parent_name = "(null)";
		}

		printf("%2zd %16s %16s %10ld",
		       id, data->name, parent_name, clk_get_rate(&clk));
		if (data->sel_mask)
			printf(" %3d", (readl(plat->base + data->sel_reg) & data->sel_mask) / (data->sel_mask & -data->sel_mask));
		else if (data->div_mask || data->en_mask)
			puts("    ");
		if (data->div_mask)
			printf(" %3d", (readl(plat->base + data->div_reg) & data->div_mask) / (data->div_mask & -data->div_mask));
		else if (data->en_mask)
			puts("    ");
		if (data->en_mask)
			printf(" %2d", (readl(plat->base + data->en_reg) & data->en_mask) / (data->en_mask & -data->en_mask));
		puts("\n");
	}
	puts("\n");
}

struct clk_ops bl808_clk_ops = {
	.request	= bl808_clk_request,
	.round_rate	= bl808_clk_round_rate,
	.get_rate	= bl808_clk_get_rate,
	.set_rate	= bl808_clk_set_rate,
	.get_parent	= bl808_clk_get_parent,
	.set_parent	= bl808_clk_set_parent,
	.enable		= bl808_clk_enable,
	.disable	= bl808_clk_disable,
	.dump		= bl808_clk_dump,
};

extern U_BOOT_DRIVER(bl808_reset);

static int bl808_clk_bind(struct udevice *dev)
{
	if (IS_ENABLED(CONFIG_RESET_BL808)) {
		device_bind(dev, DM_DRIVER_REF(bl808_reset), "reset",
			    dev_get_plat(dev), dev_ofnode(dev), NULL);
	}

	return 0;
}

static int bl808_clk_probe(struct udevice *dev)
{
	const struct bl808_clk_plat *plat = dev_get_plat(dev);
	struct clk *current_parents;

	current_parents = calloc(sizeof(struct clk), plat->desc->num_clks);
	if (!current_parents)
		return -ENOMEM;

	dev_set_priv(dev, current_parents);

	return 0;
}

static int bl808_clk_of_to_plat(struct udevice *dev)
{
	struct bl808_clk_plat *plat = dev_get_plat(dev);

	plat->base = dev_remap_addr(dev);
	if (!plat->base)
		return -ENOMEM;

	plat->desc = (const struct bl808_clk_desc *)dev_get_driver_data(dev);
	if (!plat->desc)
		return -EINVAL;

	return 0;
}

U_BOOT_DRIVER(bl808_clk) = {
	.name		= "bl808_clk",
	.id		= UCLASS_CLK,
	.of_match	= bl808_clk_ids,
	.bind		= bl808_clk_bind,
	.probe		= bl808_clk_probe,
	.of_to_plat	= bl808_clk_of_to_plat,
	.plat_auto	= sizeof(struct bl808_clk_plat),
	.ops		= &bl808_clk_ops,
	.flags		= DM_FLAG_PRE_RELOC,
};
