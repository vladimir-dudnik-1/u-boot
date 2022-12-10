// SPDX-License-Identifier: GPL-2.0+

#include <dm.h>
#include <reset-uclass.h>
#include <asm/io.h>
#include <clk/bl808.h>
#include <bl808/glb_reg.h>
#include <dt-bindings/reset/bl808-glb.h>
#include <dt-bindings/reset/bl808-mm-glb.h>
#include <linux/bitops.h>

static const struct bl808_reset_data *
bl808_reset_get_data(const struct bl808_clk_plat *plat,
		     const struct reset_ctl *reset_ctl)
{
	const struct bl808_clk_desc *desc = plat->desc;

	if (reset_ctl->id >= desc->num_resets)
		return NULL;

	return &desc->resets[reset_ctl->id];
}

static int bl808_reset_request(struct reset_ctl *reset_ctl)
{
	const struct bl808_clk_plat *plat = dev_get_plat(reset_ctl->dev);

	return bl808_reset_get_data(plat, reset_ctl) ? 0 : -EINVAL;
}

static int bl808_reset_set(struct reset_ctl *reset_ctl, bool assert)
{
	const struct bl808_clk_plat *plat = dev_get_plat(reset_ctl->dev);
	const struct bl808_reset_data *data = bl808_reset_get_data(plat, reset_ctl);
	u32 mask = BIT(data->bit);

	clrsetbits_le32(plat->base + data->reg, mask, assert ? mask : 0);

	return 0;
}

static int bl808_reset_assert(struct reset_ctl *reset_ctl)
{
	return bl808_reset_set(reset_ctl, true);
}

static int bl808_reset_deassert(struct reset_ctl *reset_ctl)
{
	return bl808_reset_set(reset_ctl, false);
}

static int bl808_reset_status(struct reset_ctl *reset_ctl)
{
	const struct bl808_clk_plat *plat = dev_get_plat(reset_ctl->dev);
	const struct bl808_reset_data *data = bl808_reset_get_data(plat, reset_ctl);
	u32 mask = BIT(data->bit);

	return !!(readl(plat->base + data->reg) & mask);
}

static const struct reset_ops bl808_reset_ops = {
	.request	= bl808_reset_request,
	.rst_assert	= bl808_reset_assert,
	.rst_deassert	= bl808_reset_deassert,
	.rst_status	= bl808_reset_status,
};

U_BOOT_DRIVER(bl808_reset) = {
	.name		= "bl808_reset",
	.id		= UCLASS_RESET,
	.ops		= &bl808_reset_ops,
};
