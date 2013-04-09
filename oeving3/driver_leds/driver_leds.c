/*****************************************************************************
 *
 * Øving 3 uCSysDes, driverkoden
 *
 *****************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "ap7000.h"

// Prototypes

static int __init driver_init(void);
static void __exit driver_exit(void);
static int driver_open(struct inode *inode, struct file *filp);
static int driver_release(struct inode *inode, struct file *filp);
static ssize_t driver_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

// File Operations Struct

static struct file_operations driver_fops = {
  .owner = THIS_MODULE,
  .read = driver_read,
  .write = driver_write,
  .open = driver_open,
  .release = driver_release
};

// Constants

static dev_t dev;
static struct cdev *cdev;
static unsigned long MEM_ADDR_START = AVR32_PIOB_ADDRESS;
static unsigned long MEM_ADDR_LEN = AVR32_PIOC_ADDRESS - AVR32_PIOB_ADDRESS;

static int elements[] = {1<<8, 1<<9, 1<<10, 1<<13, 1<<14, 1<<15, 1<<16, 1<<30};
static int elements_all;

/*****************************************************************************/

static int __init driver_init(void) {
	// Setup vars
	elements_all = elements[0] + elements[1] + elements[2] + elements[3]
		+ elements[4] + elements[5] + elements[6] + elements[7];
	
	// Allocate device number
	//dev = MKDEV(1337, 0);
	int res_devnum = alloc_chrdev_region(&dev, 0, 1, "LEDS");
	
	// Check for errors
	if (res_devnum < 0) return res_devnum;
	
	// Request I/O ports
	request_region(MEM_ADDR_START, MEM_ADDR_LEN, "LEDS");
	
	// Init hardware
	AVR32_PIOB.per = elements_all;
	AVR32_PIOB.oer = elements_all;
	AVR32_PIOB.codr = elements_all;
	
	// Register Device
	cdev = cdev_alloc();
	cdev_init(cdev, &driver_fops);
	cdev_add(cdev, dev, 1);
	
	printk(KERN_INFO "LED driver loaded! (%d, %d)", MAJOR(dev), MINOR(dev));
	
	return 0;
}

/*****************************************************************************/

static void __exit driver_exit(void) {
	// TODO?
	
	// Release device number
	cdev_del(cdev);
	unregister_chrdev_region(dev, 1);
	
	// Release hardware
	AVR32_PIOB.codr = elements_all;
	AVR32_PIOB.pdr = elements_all;
	
	// Release I/O ports
	release_region(MEM_ADDR_START, MEM_ADDR_LEN);
	
	printk(KERN_INFO "LED driver killed!");
}

/*****************************************************************************/

static int driver_open(struct inode *inode, struct file *filp) {
	return 0;
}

static int driver_release(struct inode *inode, struct file *filp) {
	return 0;
}

static ssize_t driver_read(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	int pindata = AVR32_PIOB.pdsr;
	char data[8];
	
	data[0] = (pindata & elements[0]) ? 'A' : 'a';
	data[1] = (pindata & elements[1]) ? 'B' : 'b';
	data[2] = (pindata & elements[2]) ? 'C' : 'c';
	data[3] = (pindata & elements[3]) ? 'D' : 'd';
	data[4] = (pindata & elements[4]) ? 'E' : 'e';
	data[5] = (pindata & elements[5]) ? 'F' : 'f';
	data[6] = (pindata & elements[6]) ? 'G' : 'g';
	data[7] = (pindata & elements[7]) ? 'H' : 'h';
	
	/*
	int i = count;
	while (i--) {
		
	}*/
	
	if(count > 8) count = 8;  //Do not allow reads outside the array.
	copy_to_user(buff, data, count);
	
	return count;
}

static ssize_t driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
	char data[count];
	copy_from_user(data, buff, count);
	printk(KERN_INFO "LED driver recieving data!");
	int i = 0;
	while (i < count) {
		switch (data[i]) {
			case 'A': AVR32_PIOB.SODR.p8 = 1; break;
			case 'B': AVR32_PIOB.SODR.p9 = 1; break;
			case 'C': AVR32_PIOB.SODR.p10 = 1; break;
			case 'D': AVR32_PIOB.SODR.p13 = 1; break;
			case 'E': AVR32_PIOB.SODR.p14 = 1; break;
			case 'F': AVR32_PIOB.SODR.p15 = 1; break;
			case 'G': AVR32_PIOB.SODR.p16 = 1; break;
			case 'H': AVR32_PIOB.SODR.p30 = 1; break;
			
			case 'a': AVR32_PIOB.CODR.p0 = 1; break;
			case 'b': AVR32_PIOB.CODR.p9 = 1; break;
			case 'c': AVR32_PIOB.CODR.p10 = 1; break;
			case 'd': AVR32_PIOB.CODR.p13 = 1; break;
			case 'e': AVR32_PIOB.CODR.p14 = 1; break;
			case 'f': AVR32_PIOB.CODR.p15 = 1; break;
			case 'g': AVR32_PIOB.CODR.p16 = 1; break;
			case 'h': AVR32_PIOB.CODR.p30 = 1; break;
		}
		i++;
	}
	
	return count;
}

/*****************************************************************************/

// Set init and exit functions
module_init(driver_init);
module_exit(driver_exit);

// Descrption
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LED driver");
MODULE_VERSION("blarg");
MODULE_AUTHOR("PTL");

