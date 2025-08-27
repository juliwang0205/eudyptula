// SPDX-License-Identifier: GPL-2.0
/*
 * Sample kobject implementation
 *
 * Copyright (C) 2004-2007 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (C) 2007 Novell Inc.
 */
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/jiffies.h>

/*
 * This module shows how to create a simple subdirectory in sysfs called
 * /sys/kernel/eudyptula.  In that directory, 3 files are created:
 * "id", "jeffies", and "foo".  If an integer is written to these files, it can be
 * later read out of it.
 */

#define ID_SIZE 13

struct eudyptula_sysfs_data{
    void *data;
    int data_size;
};

struct rw_semaphore foo_sem;
struct eudyptula_sysfs_data id_data;
struct eudyptula_sysfs_data foo_data;
/*
 * The "id" file store the id.
 */
static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
    printk(KERN_INFO "eudyptula task09: id show %s\n", (char *)id_data.data);
    return sysfs_emit(buf, "%s\n", (char *)id_data.data); 
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
    printk(KERN_INFO "eudyptula task09: id store %s\n", buf);
    char *tmp_data = id_data.data;
    memcpy(tmp_data, buf, id_data.data_size);
    tmp_data[id_data.data_size - 1] = '\0';
    printk(KERN_INFO "eudyptula task09: id data store %s\n", (char *)id_data.data);
    return id_data.data_size; 
}

static struct kobj_attribute id_attribute =
    // kenrel 6.16 cannot support with S_IWOTH in sysfs
	__ATTR(id, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH, id_show, id_store);

/*
 * "jeffies" to store the jiffies
 */
static ssize_t jeffies_show(struct kobject *kobj, struct kobj_attribute *attr,
		      char *buf)
{
    unsigned long j = jiffies;
    printk(KERN_INFO "eudyptula task09: jiffies read jiffies %lu \n", j);
    return sysfs_emit(buf, "%lu\n", j);
}

static struct kobj_attribute jeffies_attribute =
	__ATTR(jeffies, S_IRUSR | S_IRGRP | S_IROTH,  jeffies_show, NULL);

/*
 * "foo" to store one page data
 */
static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
		      char *buf)
{
    down_read(&foo_sem);
    printk(KERN_INFO "eudyptula task09: foo show %s\n", (char *)foo_data.data);
    ssize_t ret = sysfs_emit(buf, "%s\n", (char *)foo_data.data); 
    up_read(&foo_sem);
    return ret;
}
static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
		       const char *buf, size_t count)
{
	down_write(&foo_sem);
    printk(KERN_INFO "eudyptula task09: foo store %s count %d\n", buf, count);
    char *tmp_data = foo_data.data;
    memcpy(tmp_data, buf, foo_data.data_size);
    if(count < foo_data.data_size)
        tmp_data[count - 1] = '\0';
    else
        tmp_data[foo_data.data_size - 1] = '\0';
    printk(KERN_INFO "eudyptula task09: foo data store %s\n", (char *)foo_data.data);
	up_write(&foo_sem);
    return foo_data.data_size;
}
static struct kobj_attribute foo_attribute =
	__ATTR(foo, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, foo_show, foo_store);


/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *attrs[] = {
	&id_attribute.attr,
	&jeffies_attribute.attr,
	&foo_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static const struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *eudyptula_kobj;

static int __init task09_init(void)
{
	int retval;
    id_data.data_size = ID_SIZE;
    id_data.data = kmalloc(id_data.data_size, GFP_KERNEL);

    init_rwsem(&foo_sem);
    foo_data.data_size = PAGE_SIZE;
    foo_data.data = kmalloc(foo_data.data_size , GFP_KERNEL);
 
    /*
	 * Create a simple kobject with the name of "eudyptula",
	 * located under /sys/kernel/
	 *
	 * As this is a simple directory, no uevent will be sent to
	 * userspace.  That is why this function should not be used for
	 * any type of dynamic kobjects, where the name and number are
	 * not known ahead of time.
	 */
	eudyptula_kobj = kobject_create_and_add("eudyptula", kernel_kobj);
	if (!eudyptula_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(eudyptula_kobj, &attr_group);
	if (retval)
		kobject_put(eudyptula_kobj);

	return retval;
}

static void __exit task09_exit(void)
{
	kobject_put(eudyptula_kobj);
    kfree(id_data.data);
    kfree(foo_data.data);
}

module_init(task09_init);
module_exit(task09_exit);
MODULE_DESCRIPTION("Sample kobject implementation");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Greg Kroah-Hartman <greg@kroah.com>");
