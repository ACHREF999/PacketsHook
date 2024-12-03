#include <linux/kernel.h>

#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>


#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/inet.h>



#define PROC_NAME "dpd"
#define LOG_BUFFER_SIZE 2048


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ACHREF999");
MODULE_DESCRIPTION("yo oy");

static struct proc_dir_entry *dpd_proc_node;
static char log_buffer[LOG_BUFFER_SIZE];
static int log_pos = 0;

// now we define the proc_read / proc_write ....

static ssize_t dpd_proc_read(struct file *file,char __user *buf,size_t count , loff_t *ppos){
    int len ; 

    if(*ppos > 0 || log_pos ==0) {
        return 0;
    }


    len = min(log_pos,(int) count);

    if(copy_to_user(buf,log_buffer,len)){
        return -EFAULT;
    }

    *ppos += len;
    return len;
}


struct file_operations dpd_proc_ops={

    .read = dpd_proc_read,

};


// now after defined driver operations , we need to subscribe to a net `request` hook that lets us manipulate our state when an event happen 

static unsigned int net_hook_func(void *priv , struct sk_buff *skb, const struct nf_hook_state *state){
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;
    char src_ip[16] , dest_ip[16];


    if(!skb) return NF_ACCEPT;

    ip_header = ip_hdr(skb);
    if(!ip_header || ip_header->version != 4){
        return NF_ACCEPT;
    }


    snprintf(src_ip,16,"%pI4",&ip_header->saddr);
    snprintf(dest_ip,16,"%pI4\n",&ip_header->daddr);

    log_pos += snprintf(log_buffer + log_pos , LOG_BUFFER_SIZE - log_pos,"SRC: %s , DST : %s", src_ip,dest_ip);

    if(log_pos >= LOG_BUFFER_SIZE - 1){
        printk("LOG BUFFER DATA:%s",log_buffer);
        log_pos = 0;
    }

    return NF_ACCEPT;
}



// now we also have a netfilter hook operations ;
static struct nf_hook_ops net_hook={};







static int dp_module_init(void){
    
    dpd_proc_node = proc_create(PROC_NAME,0,NULL,&dpd_proc_ops);
    if(!dpd_proc_node){
        pr_err("Failed to create proc entry \n");
        return -ENOMEM;
    }

    // we assign the nethook operation and register it (just like the proc)
    net_hook.hook = net_hook_func;
    net_hook.pf = PF_INET;
    net_hook.hooknum = NF_INET_PRE_ROUTING; // this is where the hook is doing the interception  ; we want it before it goes down to routing 
    net_hook.priority = NF_IP_PRI_FIRST;


    if(nf_register_net_hook(&init_net ,  &net_hook)){
        pr_err("Failed to regitser the net filter hook");
        proc_remove(dpd_proc_node);
        return -1;
    }

    pr_info("Moule Loaded\n");
    return 0;
    
}


static void dp_module_exit(void){
        // now we have to remove the netfilter entry and the proc file entry;
        nf_unregister_net_hook(&init_net,&net_hook);
        proc_remove(dpd_proc_node);
        pr_info("Module Leaving \n");
}




module_init(dp_module_init)
module_exit(dp_module_exit)
