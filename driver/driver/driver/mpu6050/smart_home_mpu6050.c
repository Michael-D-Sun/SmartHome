#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include "smart_home_mpu6050.h"
#include <asm/uaccess.h>
#include <linux/device.h>



MODULE_LICENSE("GPL");



struct  fs_mpu6050  {

	dev_t  devno;
	int major;

}mpu6050_t;

struct  i2c_client  *  myclient;
static struct  class  *  cls;



void   mpu6050_write_byte(unsigned  char  reg,unsigned  char  val)
{

	char  txbuf[2] = {reg,val};
	struct  i2c_msg  msgs[] = {
	
		{myclient->addr,0,2,txbuf},
	
	
	};

	i2c_transfer(myclient->adapter,msgs,ARRAY_SIZE(msgs));


}
unsigned  char   mpu6050_read_byte(unsigned  char  reg)
{

	char  txbuf[1] = {reg};
	char   rxbuf[1] = {0};
	struct  i2c_msg  msgs[]  = {
	
		{myclient->addr,0,1,txbuf},
		{myclient->addr,1,1,rxbuf},
	
	
	};

	
	i2c_transfer(myclient->adapter,msgs,ARRAY_SIZE(msgs));
	return  rxbuf[0];



}
static int mpu6050_open (struct inode *inode, struct file *file)
{

	printk("mpu6050_open  \n");
	return   0;

}
static ssize_t mpu6050_read (struct file *file, char __user *buf, size_t  size, loff_t *loff)
{

	printk("mpu6050_read  \n");

	return   0;

}
static long   mpu6050_ioctl (struct file *file, unsigned int  cmd, unsigned long  arg)
{

	struct  mpu6050  data;

	switch(cmd)
	{
	
	case SMART_HOME_MPU6050_GYRO:
         data.x  = mpu6050_read_byte(GYRO_XOUT_L);
		 data.x |=mpu6050_read_byte(GYRO_XOUT_H) <<8;
         data.y  = mpu6050_read_byte(GYRO_YOUT_L);
		 data.y |=mpu6050_read_byte(GYRO_YOUT_H) <<8;
         data.z  = mpu6050_read_byte(GYRO_ZOUT_L);
		 data.z |=mpu6050_read_byte(GYRO_ZOUT_H) <<8;

		break;
	case SMART_HOME_MPU6050_ACCEL:
		data.x   = mpu6050_read_byte(ACCEL_XOUT_L);
		data.x   |= mpu6050_read_byte(ACCEL_XOUT_H) << 8;
		data.y   = mpu6050_read_byte(ACCEL_YOUT_L);
		data.y   |= mpu6050_read_byte(ACCEL_YOUT_H) << 8;
		data.z   = mpu6050_read_byte(ACCEL_ZOUT_L);
		data.z   |= mpu6050_read_byte(ACCEL_ZOUT_H) << 8;

		break;
	case SMART_HOME_MPU6050_TEMP:
		data.temp  = mpu6050_read_byte(TEMP_OUT_L);
		data.temp  |= mpu6050_read_byte(TEMP_OUT_H) << 8;

		break;
	
	}

	if(copy_to_user((void  *)arg,&data,sizeof(struct  mpu6050)))
			return  -EINVAL;
			

	return  0;

}
struct  file_operations    mpu6050_fops  = {

	.owner   =  THIS_MODULE,
	.open  =  mpu6050_open,
	.read  =  mpu6050_read,
	.unlocked_ioctl  =  mpu6050_ioctl,

};

static int mpu6050_probe(struct i2c_client *client, const struct i2c_device_id *id)
{

	int minor = 0;
	myclient  = client;
	mpu6050_t.major  =  register_chrdev(0,"mpu6050",&mpu6050_fops);
	if(mpu6050_t.major  <  0)
	{
	
		printk("register_chrdev   fail  \n");
		return  -ENOMEM;

	
	}

	mpu6050_t.devno  =MKDEV(mpu6050_t.major,minor);


	mpu6050_write_byte(PWR_MGMT_1,0x00);
	mpu6050_write_byte(SMPRT_DIV,0x07);
	mpu6050_write_byte(CONFIG,0x06);
	mpu6050_write_byte(GYRO_CONFIG,0x18);
	mpu6050_write_byte(ACCEL_CONFIG,0x01);


	cls  =  class_create(THIS_MODULE,"mpu6050");
	device_create(cls,NULL,mpu6050_t.devno,NULL,"smart_home_mpu6050");

	printk("match  ok  \n");
	return   0;

}	

static int mpu6050_remove(struct i2c_client *client)
{

	device_destroy(cls,mpu6050_t.devno);
	class_destroy(cls);
	unregister_chrdev(mpu6050_t.major,"mpu6050");
	printk("mpu6050_remove  \n");
	return   0;

}

struct   of_device_id   mpu6050_table[]  = {

	{
	
		.compatible = "invensense,mpu6050",
	
	},


};

struct  i2c_device_id   mpu6050_id_table []  = {


	{"mpu6050",0},

};

struct i2c_driver    mpu6050_driver  = {
	.driver   = {
	
		.name = "mpu6050",
		.of_match_table  = mpu6050_table,
	
	},

	.id_table  = mpu6050_id_table,
	.probe  = mpu6050_probe,
	.remove = mpu6050_remove,
};
int  mpu6050_init(void)
{

	i2c_add_driver(&mpu6050_driver);
	printk("mpu6050_init  \n");
	return  0;
}
void  mpu6050_exit(void)
{

	i2c_del_driver(&mpu6050_driver);
	printk("mpu6050_exit  \n");
}
module_init(mpu6050_init);
module_exit(mpu6050_exit);
