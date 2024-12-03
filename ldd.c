#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ACHREF999");
MODULE_DESCRIPTION("yo oy");

static struct proc_dir_entry *dpd_proc_node;

struct file_operations dpd_proc_ops={};



static int dp_module_init(void){
    
    dpd_proc_node = proc_create("dpd",0,NULL,&dpd_proc_ops);

    return 0;
    
}


static void dp_module_exit(void){
        proc_remove(dpd_proc_node);
        printk("Leaving daemon");
}




module_init(dp_module_init)
module_exit(dp_module_exit)