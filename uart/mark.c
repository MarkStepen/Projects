#include <asm/cacheflush.h>  
#include <linux/fdtable.h>  
#include <linux/file.h>  
#include <linux/fs.h>  
#include <linux/list.h>  
#include <linux/miscdevice.h>  
#include <linux/mm.h>  
#include <linux/module.h>  
#include <linux/mutex.h>  
#include <linux/nsproxy.h>  
#include <linux/poll.h>  
#include <linux/debugfs.h>  
#include <linux/rbtree.h>  
#include <linux/sched.h>  
#include <linux/seq_file.h>  
#include <linux/uaccess.h>  
#include <linux/vmalloc.h>  
#include <linux/slab.h>  
#include <linux/delay.h>  
  
#include <asm/io.h>  
#include <asm/irq.h>  
#include <asm/gpio.h>  
  
/* 定义DHT11接入的GPIO口 */  
#define DHT11_PIN   EXYNOS4_GPX1(7)  
  
/* 定义检测标志，用来判断接收到的温湿度数据是否正确 */  
static unsigned char check_flag;   
static unsigned char dht11_data_buf[6];   
  
/*  
 * 读取一个bit的数据 
 */  
static int dht11_read_one_bit(void)            
{    
    gpio_direction_input(DHT11_PIN);    
    return gpio_get_value(DHT11_PIN);    
}    
  
/*set pin is out and set value of this pin*/  
static void dht11_gpio_out(int value)     
{    
    gpio_direction_output(DHT11_PIN, value);    
}    
  
/*read a byte data from dht11*/  
static unsigned char dht11_read_byte(void)    
{        
    int i = 0;    
    int num;     
    unsigned char flag = 0;    
    unsigned char data = 0;    
    
    for(num = 0; num < 8; num++)    
    {                  
        i = 0;    
        /* 等待DHT11的引脚变为高电平 */  
        while(!gpio_get_value(DHT11_PIN))    
        {    
            udelay(10);    
            i++;    
            if(i > 10)    
                break;    
        }    
        flag = 0x00;    
        udelay(28);                
        if(gpio_get_value(DHT11_PIN))    
        {    
            flag = 0x01;              
        }    
        i = 0;    
        while(gpio_get_value(DHT11_PIN))    
        {    
            udelay(10);    
            i++;    
            if(i > 12)    
                break;    
        }    
        data <<= 1;    
        data |= flag;    
    }      
    return data;    
}    
  
/** read data format : 
  *     8bit : humidity integer 
  *     8bit : humidity decimal 
  *     8bit : temperature integer 
  *     8bit : temperature decimal 
  *     8bit : check code 
  * buf store data : 
  *     dht11_data_buf[0]   : humidity integer 
  *     dht11_data_buf[1]   : humidity decimal 
  *     dht11_data_buf[2]   : temperature integer 
  *     dht11_data_buf[3]   : temperature decimal 
  *     dht11_data_buf[4]   : check code 
  *     dht11_data_buf[5]   : self calculate check code 
  */  
static void dht11_read_data(void)    
{    
    int i = 0;    
      
    dht11_gpio_out(0);    
    mdelay(30);    
    dht11_gpio_out(1);    
    udelay(20);    
      
    if(dht11_read_one_bit() == 0)    
    {     
        while(!gpio_get_value(DHT11_PIN))  /* 等待IO口变为高电平 */  
        {    
           udelay(5);    
           i++;    
           if(i > 20)    
           {    
                printk("dht11_read_data %d err!\n",  __LINE__);    
                break;    
           }    
        }    
        i = 0;    
        while(gpio_get_value(DHT11_PIN))  /* 等待IO口变为低电平 */  
        {    
           udelay(5);    
           i++;    
           if(i > 20)    
           {    
              printk("dht11_read_data %d err!\n", __LINE__);    
              break;    
           }    
        }    
        for(i = 0; i < 5; i++)  /* 读取温湿度数据 */  
            dht11_data_buf[i] = dht11_read_byte();      
  
        /* 对读取到的数据进行校验 */  
        dht11_data_buf[5] = dht11_data_buf[0]+dht11_data_buf[1]+dht11_data_buf[2]+dht11_data_buf[3];    
  
        /* 判断读到的校验值和计算的校验值是否相同 */  
        if(dht11_data_buf[4] == dht11_data_buf[5])  /* 相同则把标志值设为0xff */    
        {    
           check_flag = 0xff;     
        }    
        else if(dht11_data_buf[4] != dht11_data_buf[5]) /* 不相同则把标志值设为0 */  
        {    
           check_flag = 0x00;    
           printk("dht11 check fail\n");               
        }                       
    }    
}    
  
/* 打开杂项设备，这里写为空函数 */  
static int dht11_open (struct inode *inode, struct file *file)  
{  
    return 0;  
}   
  
/* 杂项设备的读取函数 */  
static ssize_t dht11_read (struct file *file, char __user *buffer, size_t size, loff_t *offset)  
{  
    int ret;   
    unsigned long flags;  
  
    /* 因为DHT11的时序要求很高，所以在读温湿度的时候要让代码进入临界区，防止内核调度和抢占 */  
    local_irq_save(flags);    
    dht11_read_data();    
    local_irq_restore(flags);    
      
    if(check_flag == 0xff)    
    {    
        /* 将读取的温湿度数据拷贝到用户空间 */  
        ret = copy_to_user(buffer, dht11_data_buf, sizeof(dht11_data_buf));    
        if(ret < 0)  
        {    
            printk("copy to user err\n");    
            return -EAGAIN;    
        }    
        else  
        {  
            return 0;    
        }  
    }    
    else   
    {  
        return -EAGAIN;  
    }  
}  
  
/* 释放函数，这里写为空函数 */  
static int dht11_release (struct inode *inode, struct file *file)  
{  
    return 0;  
}  
  
/* 定义杂项设备操作的结构体并初始化成员函数指针 */  
static const struct file_operations dht11_fops = {  
    .owner    = THIS_MODULE,  
    .open     = dht11_open,  
    .read     = dht11_read,  
    .release  = dht11_release,  
};  
  
/* 定义设备杂项设备结构体 */  
static struct miscdevice dht11_miscdev = {  
    .minor = MISC_DYNAMIC_MINOR,    /* 自动分配次设备号 */  
    .name = "dht11",    /* 杂项设备的名字 */  
    .fops = &dht11_fops /* 杂项设备操作的结构体 */  
};  
  
/* 入口函数 */  
static int yl_dht11_init(void)  
{  
    int ret;  
  
    /* 申请对应的GPIO引脚资源，只有引脚资源没有被使用才可以申请成功 */  
    ret = gpio_request(DHT11_PIN , "dht11");    
    if(ret)  
    {  
        printk("gpio_request for dht11 is failed!\n");  
        return ret;  
    }  
  
    /* 注册杂项设备 */  
    ret = misc_register(&dht11_miscdev);  
    return ret;  
}  
  
/* 函数出口 */  
static void yl_dht11_exit(void)  
{  
    /* 释放注册的杂项设备和GPIO资源 */  
    misc_deregister(&dht11_miscdev);  
    gpio_free(DHT11_PIN);  
}  
  
module_init(yl_dht11_init);  
module_exit(yl_dht11_exit);  
  
MODULE_LICENSE("GPL");
