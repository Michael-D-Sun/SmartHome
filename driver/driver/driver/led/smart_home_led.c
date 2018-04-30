#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#include "smart_home_led.h"


static int major = 0;
static int devno = 0;


static volatile unsigned int *gpx2con;
static volatile unsigned int *gpx1con;
static volatile unsigned int *gpf3con;
static volatile unsigned int *gpx2dat;
static volatile unsigned int *gpx1dat;
static volatile unsigned int *gpf3dat;

static struct class *cls;

static int smart_home_led_open (struct inode *inode, struct file *file)
{

#ifdef DEBUG
	printk("smart_home_led_open\n");
#endif
	return 0;
}


/*
*deal with the command sent by the application
*
*/

static long smart_home_led_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch (cmd)
	{
		case LED1_ON:
			writel(readl(gpx2dat)|(1<<7),gpx2dat);
			break;
		case LED1_OFF:
			writel(readl(gpx2dat)&~(1<<7),gpx2dat);
			break;
		case LED2_ON:
			writel(readl(gpx1dat)|(1<<0),gpx1dat);
			break;
		case LED2_OFF:
			writel(readl(gpx1dat)&~(1<<0),gpx1dat);
			break;
		case LED3_ON:
			writel(readl(gpf3dat)|(1<<4),gpf3dat);
			break;
		case LED3_OFF:
			writel(readl(gpf3dat)&~(1<<4),gpf3dat);
			break;
		case LED4_ON:
			writel(readl(gpf3dat)|(1<<5),gpf3dat);
			break;
		case LED4_OFF:
			writel(readl(gpf3dat)&~(1<<5),gpf3dat);
			break;
		case LEDALL_ON:
			writel(readl(gpx2dat)|(1<<7),gpx2dat);
			writel(readl(gpx1dat)|(1<<0),gpx1dat);
			writel(readl(gpf3dat)|(0x3<<4),gpf3dat);
			break;
		case LEDALL_OFF:
			writel(readl(gpx2dat)&~(1<<7),gpx2dat);
			writel(readl(gpx1dat)&~(1<<0),gpx1dat);
			writel(readl(gpf3dat)&~(0x3<<4),gpf3dat);
			break;
	}
	
#ifdef DEBUG
	printk("smart_home_led_unlocked_ioctl\n");
#endif
	return 0;
}

static int smart_home_led_release (struct inode *inode, struct file *file)
{

	printk("smart_home_led_release\n");
	return 0;
}

struct file_operations led_fops =  {
	.owner = THIS_MODULE,
	.unlocked_ioctl = smart_home_led_unlocked_ioctl,
	.open = smart_home_led_open,
	.release = smart_home_led_release,	
};


static int smart_home_led_probe(struct platform_device *pdev)
{
	int minor = 0;

	/*register chrdev*/
	major = register_chrdev(0,"smart_home_led",&led_fops);
	if(major < 0)
	{
		printk("fail to register chardev\n");
		return -EBUSY;
	}
	devno = MKDEV(major,minor);

	/*gpio remap build virtual memory*/

	gpx2con = ioremap(pdev->resource[0].start,pdev->resource[0].end - pdev->resource[0].start);
	if(NULL == gpx2con)
	{
		printk("Unable to remap gpx2con");
		goto err1;
	}
	gpx1con = ioremap(pdev->resource[1].start,pdev->resource[1].end - pdev->resource[1].start);
	if(NULL == gpx1con)
	{
		printk("Unable to remap gpx1con");
		goto err2;
	}
	gpf3con = ioremap(pdev->resource[2].start,pdev->resource[2].end - pdev->resource[2].start);
	if(NULL == gpf3con)
	{
		printk("Unable to remap gpx1con");
		goto err3;
	}

	gpx2dat = gpx2con + 1;
	gpx1dat = gpx1con + 1;
	gpf3dat = gpf3con + 1;

	writel((readl(gpx2con)&~(0xf<<28))|(0x1<<28),gpx2con);
	writel((readl(gpx1con)&~(0xf<<0))|(0x1<<0),gpx1con);
	writel((readl(gpf3con)&~(0xff<<16))|(0x11<<16),gpf3con);

	writel(readl(gpx2dat)&~(0x1<<7),gpx2dat);
	writel(readl(gpx1dat)&~(0x1<<0),gpx1dat);
	writel(readl(gpf3dat)&~(0x3<<4),gpf3dat);

	
	/*device model create class*/
	
	cls = class_create(THIS_MODULE,"smart_home_led");
	if (IS_ERR(cls)) {
		printk(KERN_WARNING "Unable to create smart_home_led class; errno = %ld\n", PTR_ERR(cls));
		cls = NULL;
	}

	/*create device node*/
	device_create(cls,NULL,devno,NULL,"smart_home_led");
	
	printk("match ok\n");
	return 0;

	/*deal with error */
	
err3:
	iounmap(gpx1con);
err2:
	iounmap(gpx2con);
err1:
	unregister_chrdev(major,"smart_home_led");
	return -ENOMEM;
}

static int smart_home_led_remove(struct platform_device *pdev)
{
	/*free all the resource*/
	device_destroy(cls,devno);
	class_destroy(cls);
	iounmap(gpf3con);
	iounmap(gpx1con);
	iounmap(gpx2con);
	unregister_chrdev(major,"smart_home_led");

	printk("smart_home led_remove\n");
	return 0;
}

struct of_device_id led_table[] = {

	{.compatible = "fs4412,led",},
};

struct platform_driver   led_drv = {
	
	
	.probe  = smart_home_led_probe,
	.remove = smart_home_led_remove,
	.driver = {
		.name = "fs4412-led",
		.of_match_table = led_table,	
	},
};

static int smart_home_led_init(void)
{
	platform_driver_register(&led_drv);

	printk("smart_home_led_init\n");
	return 0;
}
static  void smart_home_led_exit(void)
{	
	platform_driver_unregister(&led_drv);
	printk("smart_home_led_exit\n");
}

MODULE_LICENSE("GPL");
module_init(smart_home_led_init);
module_exit(smart_home_led_exit);
