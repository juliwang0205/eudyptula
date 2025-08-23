#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/list.h>
#include <linux/jiffies.h>

#define ID_SIZE 13

static struct dentry *eudyptula_root;
static LIST_HEAD(eudyptula_list);

struct rw_semaphore foo_sem;

struct eudyptula_debugfs_attr {
    const char *name;
    umode_t mode;
    void *data;
    int data_size;
    struct file_operations *fops;
    struct list_head list;
};

static int id_open(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "eudyptula task08: id open device\n");
    // allocate data to  store data
    struct eudyptula_debugfs_attr *attr;

    list_for_each_entry(attr, &eudyptula_list, list) {
        if(strcmp(attr->name, "id") == 0) {
            //char data[] = "7c1caf2f50d1";
            attr->data_size = ID_SIZE;
            attr->data = kmalloc(attr->data_size, GFP_KERNEL);
            break;
        }
    }
    return 0;
}

static int id_release(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "eudyptula task08: id release device\n");
    // free data
    struct eudyptula_debugfs_attr *attr;
    list_for_each_entry(attr, &eudyptula_list, list) {
        if(strcmp(attr->name, "id") == 0) {
            kfree(attr->data); 
            break;     
        }         
    }
    return 0;
}

static ssize_t id_write(struct file *filp, const char *buf, size_t len, loff_t *ppos) {
    printk(KERN_INFO "eudyptula task08: id write %ld bytes to device\n", len);
    //copy_from_user(attr->data, buf, len);     
    struct eudyptula_debugfs_attr *attr;
    list_for_each_entry(attr, &eudyptula_list, list) {
        if (strcmp(attr->name, "id") == 0) {
            len = attr->data_size;

            if(copy_from_user(attr->data, buf, len) != 0) {
    		//printk(KERN_INFO "eudyptula task08: id write %s data to device\n", attr->data);
                return -EFAULT;         
            };
            break;   
        }
    }
    *ppos += len;
    return len;
}

static ssize_t id_read(struct file *filp, char *buf, size_t len, loff_t *ppos) {
    printk(KERN_INFO "eudyptula task08: id read %ld bytes from device\n", len);
    //copy_to_user(buf, attr->data, len);
    struct eudyptula_debugfs_attr *attr;
    list_for_each_entry(attr, &eudyptula_list, list) {
        if(strcmp(attr->name, "id") == 0) {
            len = attr->data_size;

            if (copy_to_user(buf, attr->data, len) != 0) {
                return -EFAULT;        
            };
            break;
        }
    }
    *ppos += len;
    return len;
}

static int jiffies_open(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "eudyptula task08: jiffies open device\n");
    // allocate data to  store data
    struct eudyptula_debugfs_attr *attr;

    list_for_each_entry(attr, &eudyptula_list, list) {
        if(strcmp(attr->name, "jiffies") == 0) {
            // here should set the length of jiffies
            attr->data_size = sizeof(unsigned long);
            attr->data = kmalloc(attr->data_size, GFP_KERNEL);
            break;
        }
    }
    return 0;
}

static int jiffies_release(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "eudyptula task08: jiffies release device\n");
    // free data
    struct eudyptula_debugfs_attr *attr;
    list_for_each_entry(attr, &eudyptula_list, list) {
        if(strcmp(attr->name, "jiffies") == 0) {
            kfree(attr->data); 
            break;     
        }         
    }
    return 0;
}

static ssize_t jiffies_read(struct file *filp, char *buf, size_t len, loff_t *ppos) {
    printk(KERN_INFO "eudyptula task08: jiffies read %ld bytes from device\n", len);
    //copy_to_user(buf, attr->data, len);
	unsigned long j = jiffies;
    struct eudyptula_debugfs_attr *attr;
    list_for_each_entry(attr, &eudyptula_list, list) {
        if(strcmp(attr->name, "jiffies") == 0) {
            len = attr->data_size;
	        memcpy(attr->data, &j, len);
	        printk(KERN_INFO "eudyptula task08: jiffies read jiffies %ld \n", j);
            if (copy_to_user(buf, attr->data, len) != 0) {
                return -EFAULT;        
            };
            break;
        }
    }
    *ppos += len;
    return len;
}

static int foo_open(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "eudyptula task08: foo open device\n");
    // allocate data to  store data
    struct eudyptula_debugfs_attr *attr;

    list_for_each_entry(attr, &eudyptula_list, list) {
        if(strcmp(attr->name, "foo") == 0) {
            // here should set the length of one page size in OS
            attr->data_size = PAGE_SIZE;
            attr->data = kmalloc(attr->data_size , GFP_KERNEL);
            break;
        }
    }
    return 0;
}

static int foo_release(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "eudyptula task08: foo release device\n");
    // free data
    struct eudyptula_debugfs_attr *attr;
    list_for_each_entry(attr, &eudyptula_list, list) {
        if(strcmp(attr->name, "foo") == 0) {
            kfree(attr->data); 
            break;     
        }         
    }
    return 0;
}

static ssize_t foo_read(struct file *filp, char *buf, size_t len, loff_t *ppos) {
    printk(KERN_INFO "eudyptula task08: foo read %ld bytes from device\n", len);
    //copy_to_user(buf, attr->data, len);
    struct eudyptula_debugfs_attr *attr;
    list_for_each_entry(attr, &eudyptula_list, list) {
        if(strcmp(attr->name, "foo") == 0) {
            if (len > attr->data_size) {
	      	len = attr->data_size;
	    }
    	    printk(KERN_INFO "eudyptula task08: foo read %s", attr->data);
	    
	    down_read(&foo_sem);
            if (copy_to_user(buf, attr->data, len) != 0) {
                return -EFAULT;        
            }
	    up_read(&foo_sem);

            break;
        }
    }
    *ppos += len;
    return len;
}

static ssize_t foo_write(struct file *filp, const char *buf, size_t len, loff_t *ppos) {
    printk(KERN_INFO "eudyptula task08: foo write %ld bytes to device\n", len);
    //copy_from_user(attr->data, buf, len);     
    struct eudyptula_debugfs_attr *attr;
    list_for_each_entry(attr, &eudyptula_list, list) {
        if (strcmp(attr->name, "foo") == 0) {
            if (len > attr->data_size) {
                len = attr->data_size;
            }
           
	    down_write(&foo_sem); 
            if(copy_from_user(attr->data, buf, len) != 0) {
                return -EFAULT;         
            };
	    up_write(&foo_sem);

    	    printk(KERN_INFO "eudyptula task08: foo write %s", attr->data);
            break;   
        }
    }
    *ppos += len;
    return len;
}

struct file_operations task08_fops[] = {
    [0] = {
        .owner = THIS_MODULE,
        .open = id_open,
        .read = id_read,
        .write = id_write,
        .release = id_release
    },
    [1] = {
        .owner = THIS_MODULE,
        .open = jiffies_open,
        .read = jiffies_read,
        .release = jiffies_release
    },
    [2] = {
        .owner = THIS_MODULE,
        .open = foo_open,
        .read = foo_read,
        .write = foo_write,
        .release = foo_release
    }
};

static void init_debugfs_attr(void) {
    printk(KERN_INFO "eudyptula task08: init debugfs attr\n");
    char *name[] = {
        "id",
        "jiffies",
        "foo"
    };
    mode_t mode[] = {
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
        S_IRUSR | S_IRGRP | S_IROTH,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
    };

    int name_length = sizeof(name) / sizeof(name[0]);
    
    init_rwsem(&foo_sem);
    for (int i = 0; i < name_length; i++) {
        struct eudyptula_debugfs_attr *attr = 
            kmalloc(sizeof(struct eudyptula_debugfs_attr), GFP_KERNEL);
        attr->name = name[i];
        attr->fops = &task08_fops[i];
        attr->mode = mode[i];
        list_add(&attr->list, &eudyptula_list);
    }
}

static void delete_debugfs_attr(void) {
    struct eudyptula_debugfs_attr *attr, *tmp;
    list_for_each_entry_safe(attr, tmp, &eudyptula_list, list) {
        list_del(&attr->list);
        kfree(attr);
    }
}

static void debugfs_create_files(struct dentry *parent) {
    printk(KERN_INFO "eudyptula task08: debugfs create files\n");
    if (IS_ERR_OR_NULL(parent))
        return;

    struct eudyptula_debugfs_attr *attr;
    list_for_each_entry(attr, &eudyptula_list, list) {
        debugfs_create_file(attr->name, attr->mode, parent, (void *)attr, attr->fops);        
    } 
}

static int __init task08_init(void)
{
    printk(KERN_INFO "eudyptula task08");
    eudyptula_root = debugfs_create_dir("eudyptula", NULL);
    init_debugfs_attr(); 
    debugfs_create_files(eudyptula_root);         
    return 0;
}

static void __exit task08_exit(void)
{
    delete_debugfs_attr();
    debugfs_remove(eudyptula_root);
}

module_init(task08_init);
module_exit(task08_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wangf23");
MODULE_DESCRIPTION("eudyptula task08");

