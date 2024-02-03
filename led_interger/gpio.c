#include <linux/module.h>   /* Defines functions such as module_init/module_exit */
#include <linux/gpio.h>     /* Defines functions such as gpio_request/gpio_free */     



#define DRIVER_AUTHOR "tuantnt08@.com"
#define DRIVER_DESC   "gpio subsystem"

#define LOW                 0
#define HIGH                1

/* Constructor */
static int __init gpio_init(void)
{
    gpio_request(30, "PINOUT");
    gpio_direction_output(30, 1);
    gpio_set_value(30, HIGH);
    pr_info("State of led 30 : %d\n", gpio_get_value(30));
    return 0;

}

/* Destructor */
static void __exit gpio_exit(void)
{
    gpio_set_value(30, LOW);
    pr_info("State of led 30 : %d\n", gpio_get_value(30));
    gpio_free(30);
    pr_info("Good bye\n");

}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 
