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
	dev = MKDEV(1337, 0);
	int res_devnum = register_chrdev_region(dev, 1, "LEDS");
	
	// Check for errors
	if (res_devnum < 0) return res_devnum;
	
	// Request I/O ports
	request_region(MEM_ADDR_START, MEM_ADDR_LEN, "LEDS");
	
	// Init hardware
	AVR32_PIOB.per = elements_all;
	AVR32_PIOB.oer = elements_all;
	AVR32_PIOB.codr = elements_all;
	AVR32_PIOB.sodr = elements[1] + elements[3] + elements[5];
	
	// Register Device
	struct cdev *cdev = cdev_alloc();
	cdev_init(cdev, &driver_fops);
	
	printk(KERN_INFO "LED driver loaded!");
	
	return 0;
}

/*****************************************************************************/

static void __exit driver_exit(void) {
	// TODO?
	
	// Release device number
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
	return 0;
}

static ssize_t driver_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
	return 0;
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

