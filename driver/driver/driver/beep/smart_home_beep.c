#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include "smart_home_beep.h"

static int major = 0;
static int devno = 0;


static volatile unsigned  int  *gpd0con;
static volatile unsigned  int  *tcfg0;
static volatile unsigned  int  *tcfg1;
static volatile unsigned  int  *tcon;
static volatile unsigned  int  *tcntb0;
static volatile unsigned  int  *tcmpb0;


static struct class *cls;

static int smart_home_beep_open (struct inode *inode, struct file *file)
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
static long smart_home_beep_unlocked_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{

	switch (cmd)
	{
		case BEEP_ON:
			 *tcon  = (*tcon &~(0xf <<0))|(0x9 <<0);
			break;
		case BEEP_OFF:
			 *tcon  = (*tcon &~(0xf <<0))&~(0x9 <<0);
			break;
	}
	
#ifdef DEBUG
	printk("smart_home_beep_unlocked_ioctl\n");
#endif
	return 0;
}


static int smart_home_beep_release (struct inode *inode, struct file *file)
{

	printk("smart_home_beep_release\n");
	return 0;
}

struct file_operations beep_fops =  {
	.owner          = THIS_MODULE,
	.open           = smart_home_beep_open,
	.release        = smart_home_beep_release,
	.unlocked_ioctl = smart_home_beep_unlocked_ioctl,
};


static int smart_home_beep_probe(struct platform_device *pdev)

{
	int minor = 0;

	/*register chrdev*/
	major = register_chrdev(0,"smart_home_beep",&beep_fops);
	if(major < 0)
	{
		printk("fail to register_chrdev\n");
		return -ENOMEM;
	}
	devno = MKDEV(major,minor);

	/*gpio remap build virtual memory*/
	gpd0con = ioremap(pdev->resource[0].start,pdev->resource[0].end - pdev->resource[0].start);

	if(NULL == gpd0con)
	{
		goto err1;
	}
	
	tcfg0   = ioremap(pdev->resource[1].start,pdev->resource[1].end - pdev->resource[1].start);
	if(NULL == tcfg0)
	{
		goto err2;
	}
	
	tcfg1   = tcfg0 + 1;
	tcon    = tcfg0 + 2;
	tcntb0  = tcfg0 + 3;
	tcmpb0  = tcfg0 + 4;
	
	writel((readl(gpd0con )&~(0xf<<0))|(0x2<<0),gpd0con);
	writel ((readl(tcfg0)&~(0xff<<0))|(0xff<<0),tcfg0);
	writel((readl(tcfg1)&~(0xf<<0))|(0x2<<0),tcfg1);
	writel((readl(tcntb0)&0x0)|500,tcntb0);
	writel((readl(tcmpb0)&0x0)|250,tcmpb0);
	writel((readl(tcon)&~(0xf << 0))|(0x2<<0),tcon);
	writel((readl(tcon)&~(0xf<< 0))&~(0x9 << 0),tcon);
	
	
	/*device model create class*/
	
	cls = class_create(THIS_MODULE,"smart_home_beep");
	if (IS_ERR(cls)) {
		printk(KERN_WARNING "Unable to create smart_home_beep class; errno = %ld\n", PTR_ERR(cls));
		cls = NULL;
	}

	/*create device node*/
	device_create(cls,NULL,devno,NULL,"smart_home_beep");
	
	printk("smart_home_beep_init\n");
	return 0;

	/*deal with error */
err2:
	iounmap(gpd0con);
err1:
	unregister_chrdev(major,"smart_home_beep");
	return -ENOMEM;
	
}

static int smart_home_beep_remove(struct platform_device *pdev)
{	
	/*free all the resource*/
	device_destroy(cls,devno);
	class_destroy(cls);
	iounmap(gpd0con);
	iounmap(tcfg0);
	unregister_chrdev(major,"smart_home_beep");
	printk("smart_home_beep_exit\n");

	return 0;
}

struct of_device_id  beep_table[] = {

	{.compatible = "fs4412,beep"},
	
};


struct platform_driver   beep_drv = {

	.probe  = smart_home_beep_probe,
	.remove = smart_home_beep_remove,
	.driver = {
			.name = "fs4412-beep",
			.of_match_table = beep_table,
	},
	
}; 

static int smart_home_beep_init(void)
{
	platform_driver_register(&beep_drv);
	printk("smart_home_beep_init\n");
	return 0;
}

static void smart_home_beep_exit(void)
{
	platform_driver_unregister(&beep_drv);
	printk("smart_home_beep_exit\n");
}



MODULE_LICENSE("GPL");
module_init(smart_home_beep_init);
module_exit(smart_home_beep_exit);
