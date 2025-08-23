
/*
 *  @brief task05
 *  @date 2025-08-04
 *  @auth wangf23
 *  @version 1.0
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define USB_EUDYPATULA_VENDOR_ID 0x0627
#define USB_EUDYPATULA_PRODUCT_ID 0x0001

static struct usb_device_id eudyptula_table[] = {
    { USB_DEVICE(USB_EUDYPATULA_VENDOR_ID, USB_EUDYPATULA_PRODUCT_ID) },
    { } 
};

static int eudyptula_probe(struct usb_interface *interface, 
        const struct usb_device_id *id)
{
    printk(KERN_INFO "USB Eudyptula device connected\n");
    return 0;
}

static void eudyptula_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "USB Eudyptula device disconnected\n");
}
static struct usb_driver eudyptula_driver = {
    .name = "eudyptula",
    .id_table = eudyptula_table,
    .probe = eudyptula_probe,
    .disconnect = eudyptula_disconnect,
}; 

static int task05_init(void)
{
    usb_register(&eudyptula_driver);
    printk("hello eudyptula task05 driver\n");
    return 0;
}

static void task05_exit(void)
{
    usb_deregister(&eudyptula_driver);
    printk("goodbye eudyptula task05 driver\n");
}

module_init(task05_init);
module_exit(task05_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wangf23");
MODULE_DESCRIPTION("eudyptula task05");
