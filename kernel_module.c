#include <linux/init.h>             // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>           // Core header for loading LKMs into the kernel
#include <linux/device.h>           // Header to support the kernel Driver Model
#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
#include <linux/fs.h>               // Header for the Linux file system support
#include <linux/uaccess.h>          // Required for the copy to user function
#include <linux/slab.h>             // Required for kmalloc/kfree functions
#include <linux/random.h>           // Required for the get_random_bytes function

#define  DEVICE_NAME "randomness_generator"    ///< The device will appear at /dev/randomness... using this value
#define  CLASS_NAME  "first_project"        ///< The device class -- this is a character device driver

MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Quoc-Cuong TRAN");    // modinfo
MODULE_DESCRIPTION("First Project in Operating system course"); 
MODULE_VERSION("1.0");         

static int    majorNumber;                  ///< Stores the device number -- determined automatically
static int    numberOpens = 0;              ///< Counts the number of times the device is opened
static struct class*  RNGClass  = NULL; ///< The device-driver class struct pointer
static struct device* RNGDevice = NULL; ///< The device-driver device struct pointer

// The prototype functions for the character driver -- must come before the struct definition
static int     my_open(struct inode *, struct file *);
static int     my_release(struct inode *, struct file *);
static ssize_t my_read(struct file *, char *, size_t, loff_t *);

/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
   .open = my_open,
   .read = my_read,
   .release = my_release,
};

static int __init RNG_init(void){
    printk(KERN_INFO "RNG: Initializing the Random number generator LKM\n");

    // Try to dynamically allocate a major number for the device -- more difficult but worth it
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber<0){
        printk(KERN_ALERT "RNG failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "RNG: registered correctly with major number %d\n", majorNumber);

    // Register the device class
    RNGClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(RNGClass)){                  // Check for error and clean up if there is
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(RNGClass);           // Correct way to return an error on a pointer
    }
    printk(KERN_INFO "RNG: device class registered correctly\n");

    // Register the device driver
    RNGDevice = device_create(RNGClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(RNGDevice)){                 // Clean up if there is an error
        class_destroy(RNGClass);            // Repeated code but the alternative is goto statements
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(RNGDevice);
    }
    printk(KERN_INFO "RNG: device class created correctly\n");  // Made it! device was initialized
    return 0;
}

static void __exit RNG_exit(void){
   device_destroy(RNGClass, MKDEV(majorNumber, 0));         // remove the device
   class_unregister(RNGClass);                              // unregister the device class
   class_destroy(RNGClass);                                 // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
   printk(KERN_INFO "RNG: Goodbye from the LKM!\n");
}

static int my_open(struct inode *inodep, struct file *filep) {
   numberOpens++;
   printk(KERN_INFO "RNG: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

static ssize_t my_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int error_count;

    char* arr = kmalloc(len, GFP_KERNEL);
    get_random_bytes(arr, len);
    // copy_to_user has the format ( * to, *from, size) and returns 0 on success
    error_count = copy_to_user(buffer, arr, len);
    kfree(arr);

    if (error_count == 0){            // if true then have success
        printk(KERN_INFO "RNG: Sent random numbers to the user\n");
        return 0;
    }
    else {
        printk(KERN_INFO "RNG: Failed to to send random numbers to the user\n");
        return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
    }
}

static int my_release(struct inode *inodep, struct file *filep){
    printk(KERN_INFO "RNG: Device successfully closed\n");
    return 0;
}

module_init(RNG_init);
module_exit(RNG_exit);