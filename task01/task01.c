
/*
 *  @brief task01
 *  @date 2025-07-21
 *  @auth wangf23
 *  @version 1.0
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int task01_init(void)
{
    printk("hello world\n");
    return 0;
}

static void task01_exit(void)
{
    printk("goodbye world\n");
}

module_init(task01_init);
module_exit(task01_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wangf23");
MODULE_DESCRIPTION("eudyptula task01");
