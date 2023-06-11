// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2015 Hans de Goede <hdegoede@redhat.com>
 *
 * X-Powers AXP Power Management ICs gpio driver
 */

#include <common.h>
#include <asm/gpio.h>
#include <dm.h>
#include <dm/device-internal.h>
#include <errno.h>
#include <power/pmic.h>

#define AXP_GPIO_PREFIX			"AXP0-"
#define AXP_GPIO_COUNT			4

#define AXP_GPIO_CTRL_MASK		0x7
#define AXP_GPIO_CTRL_OUTPUT_LOW	0
#define AXP_GPIO_CTRL_OUTPUT_HIGH	1

struct axp_gpio_desc {
	const u8	*pins;
	u8		npins;
	u8		status_reg;
	u8		status_offset;
	u8		pull_reg;
	u8		input_mux;
};

static int axp_gpio_get_value(struct udevice *dev, unsigned pin)
{
	const struct axp_gpio_desc *desc = dev_get_priv(dev);
	int ret;

	ret = pmic_reg_read(dev->parent, desc->pins[pin]);
	if (ret < 0)
		return ret;

	if (ret == AXP_GPIO_CTRL_OUTPUT_LOW)
		return 0;
	if (ret == AXP_GPIO_CTRL_OUTPUT_HIGH)
		return 1;

	ret = pmic_reg_read(dev->parent, desc->status_reg);
	if (ret < 0)
		return ret;

	return !!(ret & BIT(desc->status_offset + pin));
}

static int axp_gpio_get_function(struct udevice *dev, unsigned pin)
{
	const struct axp_gpio_desc *desc = dev_get_priv(dev);
	int ret;

	ret = pmic_reg_read(dev->parent, desc->pins[pin]);
	if (ret < 0)
		return ret;

	ret &= AXP_GPIO_CTRL_MASK;
	if (ret == desc->input_mux)
		return GPIOF_INPUT;
	if (ret == AXP_GPIO_CTRL_OUTPUT_HIGH || ret == AXP_GPIO_CTRL_OUTPUT_LOW)
		return GPIOF_OUTPUT;

	return GPIOF_UNKNOWN;
}

static int axp_gpio_set_flags(struct udevice *dev, unsigned pin, ulong flags)
{
	const struct axp_gpio_desc *desc = dev_get_priv(dev);
	bool pull_down = flags & GPIOD_PULL_DOWN;
	int ret;
	u8 mux;

	if (flags & (GPIOD_MASK_DSTYPE | GPIOD_PULL_UP))
		return -EINVAL;
	if (pull_down && !desc->pull_reg)
		return -EINVAL;

	if (desc->pull_reg) {
		ret = pmic_clrsetbits(dev->parent, desc->pull_reg,
				      BIT(pin), pull_down ? BIT(pin) : 0);
		if (ret)
			return ret;
	}

	if (flags & GPIOD_IS_IN)
		mux = desc->input_mux;
	else if (flags & GPIOD_IS_OUT_ACTIVE)
		mux = AXP_GPIO_CTRL_OUTPUT_HIGH;
	else
		mux = AXP_GPIO_CTRL_OUTPUT_LOW;

	return pmic_clrsetbits(dev->parent, desc->pins[pin],
			       AXP_GPIO_CTRL_MASK, mux);
}

static const struct dm_gpio_ops axp_gpio_ops = {
	.get_value		= axp_gpio_get_value,
	.get_function		= axp_gpio_get_function,
	.xlate			= gpio_xlate_offs_flags,
	.set_flags		= axp_gpio_set_flags,
};

static int axp_gpio_probe(struct udevice *dev)
{
	struct axp_gpio_desc *desc = (void *)dev_get_driver_data(dev);
	struct gpio_dev_priv *uc_priv = dev_get_uclass_priv(dev);

	dev_set_priv(dev, desc);

	/* Tell the uclass how many GPIOs we have */
	uc_priv->bank_name = AXP_GPIO_PREFIX;
	uc_priv->gpio_count = desc->npins;

	return 0;
}

static const u8 axp152_gpio_pins[] = {
	0x90, 0x91, 0x92, 0x93,
};

static const struct axp_gpio_desc axp152_gpio_desc = {
	.pins		= axp152_gpio_pins,
	.npins		= ARRAY_SIZE(axp152_gpio_pins),
	.status_reg	= 0x97,
	.status_offset	= 4,
	.input_mux	= 3,
};

static const u8 axp209_gpio_pins[] = {
	0x90, 0x92, 0x93,
};

static const struct axp_gpio_desc axp209_gpio_desc = {
	.pins		= axp209_gpio_pins,
	.npins		= ARRAY_SIZE(axp209_gpio_pins),
	.status_reg	= 0x94,
	.status_offset	= 4,
	.input_mux	= 2,
};

static const u8 axp221_gpio_pins[] = {
	0x90, 0x92,
};

static const struct axp_gpio_desc axp221_gpio_desc = {
	.pins		= axp221_gpio_pins,
	.npins		= ARRAY_SIZE(axp221_gpio_pins),
	.status_reg	= 0x94,
	.pull_reg	= 0x97,
	.input_mux	= 2,
};

static const struct udevice_id axp_gpio_ids[] = {
	{ .compatible = "x-powers,axp152-gpio", .data = (ulong)&axp152_gpio_desc },
	{ .compatible = "x-powers,axp209-gpio", .data = (ulong)&axp209_gpio_desc },
	{ .compatible = "x-powers,axp221-gpio", .data = (ulong)&axp221_gpio_desc },
	{ .compatible = "x-powers,axp813-gpio", .data = (ulong)&axp221_gpio_desc },
	{ }
};

U_BOOT_DRIVER(axp_gpio) = {
	.name		= "axp_gpio",
	.id		= UCLASS_GPIO,
	.of_match	= axp_gpio_ids,
	.probe		= axp_gpio_probe,
	.ops		= &axp_gpio_ops,
};
