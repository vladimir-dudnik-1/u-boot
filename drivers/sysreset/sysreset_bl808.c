// SPDX-License-Identifier: GPL-2.0+

#include <dm.h>
#include <sysreset.h>
#include <asm/io.h>
#include <bl808/glb_reg.h>

struct bl808_sysreset_plat {
	void *base;
};

static int bl808_sysreset_request(struct udevice *dev, enum sysreset_t type)
{
	struct bl808_sysreset_plat *plat = dev_get_plat(dev);

	setbits_le32(plat->base + GLB_SWRST_CFG2_OFFSET,
		     GLB_REG_CTRL_PWRON_RST_MSK);

	return -EINPROGRESS;
}

static struct sysreset_ops bl808_sysreset_ops = {
	.request = bl808_sysreset_request,
};

static int bl808_sysreset_of_to_plat(struct udevice *dev)
{
	struct bl808_sysreset_plat *plat = dev_get_plat(dev);

	plat->base = dev_read_addr_ptr(dev);
	if (!plat->base)
		return -ENOMEM;

	return 0;
}

static const struct udevice_id bl808_sysreset_ids[] = {
	{ .compatible = "bflb,bl808-glb-sysreset" },
	{ }
};

U_BOOT_DRIVER(bl808_sysreset) = {
	.name		= "bl808_sysreset",
	.id		= UCLASS_SYSRESET,
	.of_match	= bl808_sysreset_ids,
	.of_to_plat	= bl808_sysreset_of_to_plat,
	.plat_auto	= sizeof(struct bl808_sysreset_plat),
	.ops		= &bl808_sysreset_ops,
};
