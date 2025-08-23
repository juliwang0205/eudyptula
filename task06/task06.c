#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>

struct task06Cdev {
    struct cdev cdev;
    char *data;
    int size;
};

struct task06Cdev *task06_cdev;

int major = 0;
int minor = 0;

static int task06_open(struct inode *inode, struct file *filp) {
    printk("eudyptula: open device\n");
    // use for compute the size of size to store data

    return 0;
}

static int task06_release(struct inode *inode, struct file *filp) {
    return 0;
}

static ssize_t task06_read(struct file *filp, char *buf, size_t len, loff_t *ppos) {
    printk(KERN_INFO "eudyptula: read %ld bytes from device\n", len);
    if (len > task06_cdev->size) {
        len = task06_cdev->size;
    }
    if (copy_to_user(buf, task06_cdev->data, len))
        return -EFAULT;
    printk(KERN_INFO "eudyptula: read %s from device\n", task06_cdev->data);
    return len;
}

static ssize_t task06_write(struct file *filp, const char *buf, size_t len, loff_t *ppos) {
    printk(KERN_INFO "eudyptula: write %ld bytes to device\n", len);
    if (len >= task06_cdev->size) {
        len = task06_cdev->size;
    }

    if (copy_from_user(task06_cdev->data, buf, len))
        return -EFAULT;

    printk(KERN_INFO "eudyptula: write %s to device \n",task06_cdev->data);
    if (strcmp(task06_cdev->data, "7c1caf2f50d1") != 0) {
        printk(KERN_ERR "wrong eudyptula: write %s to device \n",task06_cdev->data);
        return -EINVAL; 
    }
     
    return len;
}

struct file_operations task06_fops = {
    .owner = THIS_MODULE,
    .open = task06_open,
    .read = task06_read,
    .write = task06_write,
    .release = task06_release,
};

static int __init task06_init(void)
{
    int result;
    dev_t dev = 0;
    dev_t devno = 0;
     
    result = alloc_chrdev_region(&dev,0, 1, "eudyptula");
    if (result < 0) {
        printk(KERN_WARNING "eudyptula: can't get major %d\n", result);
        return result;
    }
    major = MAJOR(dev);
    
    task06_cdev = kmalloc(sizeof(struct task06Cdev), GFP_KERNEL);
    if(task06_cdev ==NULL) {
        printk(KERN_ERR "eudyptula: kmalloc failed for task06_cdev\n");
        return -ENOMEM;
    }
    char data[] = "7c1caf2f50d1";
    task06_cdev->size = sizeof(data)/sizeof(char); 
    task06_cdev->data = kmalloc(task06_cdev->size, GFP_KERNEL);
    if(task06_cdev->data == NULL){
        printk(KERN_ERR "eudyptula: kmalloc failed for task06_cdev->data\n");
        return -ENOMEM;
    }
    memset(task06_cdev->data,0 , task06_cdev->size);
    
    cdev_init(&task06_cdev->cdev, &task06_fops);
    task06_cdev->cdev.owner = THIS_MODULE;
    devno = MKDEV(major, minor);
    result = cdev_add(&task06_cdev->cdev, devno, 1);
    if (result < 0) {
        printk(KERN_WARNING "eudyptula: can't add device %d\n", result);
    }
    return 0;
}

static void __exit task06_exit(void)
{
    dev_t devno = MKDEV(major, minor);
    cdev_del(&task06_cdev->cdev);
    kfree(task06_cdev->data);
    kfree(task06_cdev);
    unregister_chrdev_region(devno, 1);
    
}
module_init(task06_init);
module_exit(task06_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wangf23");
MODULE_DESCRIPTION("eudyptula task06");

