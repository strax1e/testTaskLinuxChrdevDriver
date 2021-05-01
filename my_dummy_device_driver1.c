#include <linux/module.h>
#include <linux/kernel.h> // для макроопределений KERN_...
#include <linux/init.h> // для смены названий init и cleanup
#include <linux/usb.h> // для usb

#define M_AUTHOR "Andrey Afanasyev"
#define M_DESCRIPTION "A sample driver"
#define M_LICENSE "GPL"
#define DEVICE_NAME "my_dummy_device"

#define PRODUCT_ID 0x1666 // ID USB устройства, для которого драйвер
#define VENDOR_ID 0x0951 //

MODULE_LICENSE(M_LICENSE);
MODULE_AUTHOR(M_AUTHOR);
MODULE_DESCRIPTION(M_DESCRIPTION);

static struct usb_device_id my_dummy_table[] = {
    { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
    { } // терминирующий элемент
};

static int my_dummy_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    printk(KERN_INFO "my_dummy_device: connected. VendorId=%d, ProductId=%d\n", VENDOR_ID, PRODUCT_ID);
    return 0;
}

static void my_dummy_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "my_dummy_device: disconnected. VendorId=%d, ProductId=%d\n", VENDOR_ID, PRODUCT_ID);
}

static struct usb_driver my_dummy_driver = { // определяем драйвер
    .name = DEVICE_NAME,
    .id_table = my_dummy_table,
    .probe = my_dummy_probe,
    .disconnect = my_dummy_disconnect,
};

static int __init my_dummy_init_module(void)
{
    int ret = usb_register(&my_dummy_driver); // регистрируем драйвер
    if (ret != 0)
        printk(KERN_ALERT "my_dummy_device: registering error: %d\n", ret);

    printk(KERN_INFO "my_dummy_device: registering complete: %d\n", ret);
    return ret;
}

static void __exit my_dummy_exit_module(void)
{
    usb_deregister(&my_dummy_driver); // выгружаем драйвер
    printk(KERN_INFO "my_dummy_device: deregistering complete\n");
}

module_init(my_dummy_init_module);
module_exit(my_dummy_exit_module);