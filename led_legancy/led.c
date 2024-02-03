#include <linux/module.h>   /* This module defines functions such as module_init/module_exit */
#include <linux/io.h>       /* This module defines functions such as ioremap/iounmap */
#include "led.h"           /* Gpio modules */

#define DRIVER_AUTHOR "abcxyz@gmail.com"
#define DRIVER_DESC   "Turn on and off led"

uint32_t __iomem *gpio0_base_addr;

static int __init led_init(void)
{
    gpio0_base_addr = _ioremap(GPIO_Start_addr, GPIO_Size);
        if(!gpio0_base_addr)
        return -ENOMEM;

    *(gpio0_base_addr + GPIO_OE_OFFSET/4) &= (0<<30);
    *(gpio0_base_addr + GPIO_DATAOUT_OFFSET/4) |=(1<<30);

    pr_info("Hello from led 30 GPIO0\n");
    return 0;
}

static void __exit led_exit(void)
{
    *(gpio0_base_addr + GPIO_DATAOUT_OFFSET/4) &= (0<<30);
    iounmap(gpio0_base_addr);
    pr_info("Goodbye from GPIO0 PIN30\n");

}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 
