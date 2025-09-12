
/*
 *  @brief task12
 *  @date 2025-09-12
 *  @auth wangf23
 *  @version 1.0
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head list;
};

static LIST_HEAD(eudyptula_list);

static int identity_create(char *name, int id) {
	struct identity *node = kmalloc(sizeof(struct identity), GFP_KERNEL);
	if (!node)
		return -ENOMEM;

	strncpy(node->name, name, 20);
	node->id = id;
	node->busy = false;

	list_add_tail(&node->list, &eudyptula_list);

	return 0;
}

static struct identity *identity_find(int id) {
	struct identity *node;

	list_for_each_entry(node, &eudyptula_list, list) {
		if (id == node->id)
			return node;
	}

	return NULL;
}

static void identity_destroy(int id) {
	struct identity *node, *tmp;

	list_for_each_entry_safe(node, tmp, &eudyptula_list, list) {
		if (id == node->id) {
			list_del(&node->list);
			kfree(node);
		}
	}
}

static int task12_init(void)
{
	struct identity *temp;

	identity_create("Alice", 1);
	identity_create("Bob", 2);
	identity_create("Dave", 3);
	identity_create("Gena", 10);

	temp = identity_find(3);
	if (NULL == temp)
		printk("id 3 not found\n");
	printk("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (NULL == temp)
		printk("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);
	return 0;
}

static void task12_exit(void)
{
	printk("goodbye task12\n");
}

module_init(task12_init);
module_exit(task12_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wangf23");
MODULE_DESCRIPTION("eudyptula task12");
