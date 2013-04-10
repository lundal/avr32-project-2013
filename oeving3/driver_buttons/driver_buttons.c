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

/* prototyper */

static int __init driver_init(void);
static void __exit driver_exit(void);
static int driver_open (struct inode *inode, struct file *filp);
static int driver_release (struct inode *inode, struct file *filp);
static ssize_t driver_read (struct file *filp, char __user *buff,
                     size_t count, loff_t *offp);
static ssize_t driver_write (struct file *filp, const char __user *buff,
                      size_t count, loff_t *offp);

/* fops-struct */

static struct file_operations driver_fops = {
  .owner = THIS_MODULE,
  .read = driver_read,
  .write = driver_write,
  .open = driver_open,
  .release = driver_release
};


static dev_t dev;
static struct cdev *cdev;
static unsigned long MEM_ADDR_START = AVR32_PIOB_ADDRESS;
static unsigned long MEM_ADDR_LEN = AVR32_PIOC_ADDRESS - AVR32_PIOB_ADDRESS; 

static int elements[] = {1, 2, 4, 8, 16, 32, 64, 128};
static int elements_all = 255;

/*****************************************************************************/
/* init-funksjon (kalles når modul lastes) */

static int __init driver_init (void) {
	// Setup vars

	int res_devnum = alloc_chrdev_region(&dev, 0, 1, "BUTTONS");
	
	// Check for errors
	if (res_devnum < 0) return res_devnum;
	
	// Request I/O ports
	request_region(MEM_ADDR_START, MEM_ADDR_LEN, "BUTTONS");
	
	// Init hardware
	AVR32_PIOB.per = elements_all;
	AVR32_PIOB.puer = elements_all;
	
	// Register Device
	cdev = cdev_alloc();
	cdev_init(cdev, &driver_fops);
	cdev_add(cdev, dev, 1);
	
	printk(KERN_INFO "BUTTON driver loaded! (%d, %d)", MAJOR(dev), MINOR(dev));
	
	return 0;
}

/*****************************************************************************/
/* exit-funksjon (kalles når modul fjernes fra systemet) */

static void __exit driver_exit (void) {
	// Release device number
	cdev_del(cdev);
	unregister_chrdev_region(dev, 1);
	
	// Release hardware
	AVR32_PIOB.pdr = elements_all;
	
	// Release I/O ports
	release_region(MEM_ADDR_START, MEM_ADDR_LEN);
	
	printk(KERN_INFO "BUTTON driver brutally killed!");
}

/*****************************************************************************/
/* fops-funksjoner */

static int driver_open (struct inode *inode, struct file *filp) {
  return 0;
}

/*---------------------------------------------------------------------------*/

static int driver_release (struct inode *inode, struct file *filp) {
  return 0;
}

/*---------------------------------------------------------------------------*/

static ssize_t driver_read (struct file *filp, char __user *buff,
              size_t count, loff_t *offp) {


 	int pindata = AVR32_PIOB.pdsr;

	/* This is just a first version which makes it easy to test using cat. 
	In the future it might suffice to simply write the first 8 bits of the pdsr data directly?*/
       
	char data[9]; //TODO remove magic number
	

	// Remember that pindata is 0 when button is pushed.
	data[0] = (pindata & elements[0]) ? 'a' : 'A';
	data[1] = (pindata & elements[1]) ? 'b' : 'B';
	data[2] = (pindata & elements[2]) ? 'c' : 'C';
	data[3] = (pindata & elements[3]) ? 'd' : 'D';
	data[4] = (pindata & elements[4]) ? 'e' : 'E';
	data[5] = (pindata & elements[5]) ? 'f' : 'F';
	data[6] = (pindata & elements[6]) ? 'g' : 'G';
	data[7] = (pindata & elements[7]) ? 'h' : 'H';

	//An alternative way of printing the data which we might use later. 
	//Added to the end of the array for testing.
	data[8] = pindata & elements_all;
	
	if(count > 9) count = 9;  //Do not allow reads outside the array.
	copy_to_user(buff, data, count);
	
	return count;
}

/*---------------------------------------------------------------------------*/

static ssize_t driver_write (struct file *filp, const char __user *buff,
               size_t count, loff_t *offp) {
  return 0;
}

/*****************************************************************************/
/* modul-beskrivelsesmakroer */

module_init (driver_init);  /* angir hva som er init-funksjon */
module_exit (driver_exit);  /* angir hva som er exit-funksjon */

MODULE_LICENSE ("GPL");     /* programlisens for modulen */
MODULE_DESCRIPTION ("");    /* tekstlig beskrivelse */
MODULE_VERSION ("");        /* versjonsnummer */
MODULE_AUTHOR ("Kristian F. Normann, Per Thomas Lundal, Andreas L. Selvik");         /* forfatter(e) */

