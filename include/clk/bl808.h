// SPDX-License-Identifier: GPL-2.0+

#ifndef _CLK_BL808_H_
#define _CLK_BL808_H_

struct bl808_clk_data {
	const char	*name;
	const u8 	*parents;
	ulong		rate;
	u32		sel_mask;
	u32		div_mask;
	u32		en_mask;
	u16		sel_reg;
	u16		div_reg;
	u16		en_reg;
	u16 		fixed_div;
};

struct bl808_clk_desc {
	const struct bl808_clk_data	*clks;
	u8				num_clks;
};

struct bl808_clk_plat {
	void __iomem			*base;
	const struct bl808_clk_desc	*desc;
};

#endif /* _CLK_BL808_H_ */
