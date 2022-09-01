// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright 2022 - Patrick Blass <patrickblass@mailbox.org>
 *
 * Control keyboard backlight of Apple silicon laptops
 */
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/types.h>

/*
#define LED_FRONT_LEFT	0x01
#define LED_FRONT_RIGHT	0x02
*/

static void __iomem *led_port;
//static u8 led_value;

static void apple_silicon_led_set(struct led_classdev *led_cdev,
			       enum led_brightness brightness)
{
	/*if (brightness)
		led_value = LED_FRONT_LEFT | LED_FRONT_RIGHT;
	else
		led_value = ~(LED_FRONT_LEFT | LED_FRONT_RIGHT);
	writeb(led_value, led_port);
	*/
}

static struct led_classdev apple_silicon_led = {
	.name			= "apple::kbd_backlight",
	.brightness		= LED_FULL,
	.brightness_set		= apple_silicon_led_set,
	//.default_trigger	= "default-on",
};

static int apple_silicon_led_probe(struct platform_device *pdev)
{
	printk(KERN_EMERG "apple m1 led driver probed!\n");

	struct resource *res;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -EBUSY;

	led_port = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (!led_port)
		return -ENOMEM;

	/*led_value = LED_FRONT_LEFT | LED_FRONT_RIGHT;
	writeb(led_value, led_port);
	*/

	return devm_led_classdev_register(&pdev->dev, &apple_silicon_led);
}

static struct platform_driver apple_silicon_led_driver = {
	.probe	= apple_silicon_led_probe,
	.driver	= {
		.name	= "apple-silicon-leds",
	},
};

module_platform_driver(apple_silicon_led_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Keyboard Backlight LED support for Apple silicon laptopss");
MODULE_AUTHOR("Patrick Blass <patrickblass@mailbox.org>");
MODULE_ALIAS("platform:apple-silicon-leds");
