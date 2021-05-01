#include <linux/module.h>
#include <linux/kernel.h> // для макроопределений KERN_...
#include <linux/init.h> // для смены названий init и cleanup
#include <linux/cdev.h> // для cdev
#include <linux/types.h> 
#include <linux/kdev_t.h> // для dev_t
#include <linux/fs.h> // для file_operations

#define M_AUTHOR "Andrey Afanasyev"
#define M_DESCRIPTION "A sample driver"
#define M_LICENSE "GPL"
#define DEVICE_NAME "my_dummy_device"

MODULE_LICENSE(M_LICENSE);
MODULE_AUTHOR(M_AUTHOR);
MODULE_DESCRIPTION(M_DESCRIPTION);

static const int minor = 0;
static struct cdev my_dummy_cdev;
static dev_t my_dummy_dev = 0;

static struct file_operations my_dummy_fops = // определение операций с файлом устройства
{
    .owner = THIS_MODULE
};

static int __init my_dummy_init_module(void)
{
    int error = alloc_chrdev_region(&my_dummy_dev, minor, 1, DEVICE_NAME); // выделяем major динамически
    printk(KERN_INFO "my_dummy_device_driver: trying to register char device...\n");
    if (error != 0) // если ошибка, то выводим соответствующее сообщение в dmesg
    {
        printk(KERN_ALERT "my_dummy_device_driver: alloc_chrdev_region() error: %d\n", error);
        return error;
    }

    cdev_init(&my_dummy_cdev, &my_dummy_fops); // регистрация операций
    if (cdev_add(&my_dummy_cdev, my_dummy_dev, 1) != 0)  // если ошибка, то выводим соответствующее сообщение в dmesg
    {
        printk(KERN_ALERT "my_dummy_device: cdev registering error. Unregistering chardev\n");
        unregister_chrdev_region(my_dummy_dev, 1); // удаляем выделенный major
        return -1;
    }

    printk(KERN_INFO "my_dummy_device_driver: success. Major:Minor = %d:%d\n", MAJOR(my_dummy_dev), MINOR(my_dummy_dev));
    return 0;
}

static void __exit my_dummy_exit_module(void)
{
    printk(KERN_INFO "my_dummy_device_driver: trying to unregister device...\n");
    cdev_del(&my_dummy_cdev); // удаляем регистрацию операций
    unregister_chrdev_region(my_dummy_dev, 1); // удаляем выделенный major
    printk(KERN_INFO "my_dummy_device_driver: success\n");
}

module_init(my_dummy_init_module);
module_exit(my_dummy_exit_module);