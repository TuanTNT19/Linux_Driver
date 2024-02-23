#include <linux/module.h>   /* Defines functions such as module_init/module_exit */
#include <linux/gpio.h>     /* Defines functions such as gpio_request/gpio_free */     
#include <linux/delay.h>  
#include <linux/kthread.h>
#include <linux/kernel.h>


#define DRIVER_AUTHOR "tuantnt07@gmail.com"
#define DRIVER_DESC   "gpio interger"

static struct task_struct *thread1;
int sta = 0;

#define LOW                 0
#define HIGH                1

static int thr_func(void *para)
{
   // int pin = *((int *) para);
    
   while(!kthread_should_stop())
    {
        if (gpio_get_value(60) == 0)
        {
            while(gpio_get_value(60) == 0);
            if (sta == 0)
            {
                sta = 1;
            }
            else if (sta == 1)
            {
                sta = 0;
            }

        }

        if (sta == 1)
        {
            gpio_set_value(30, HIGH);
        }
        else if (sta == 0)
        {
            gpio_set_value(30, LOW);
        }
    }

   return 0;
}

/* Constructor */
static int __init gpio_init(void)
{
    gpio_request(60, "PINOUT_BUTTON");
    gpio_direction_input(60);
    
    gpio_request(30, "PINOUT1");
    gpio_direction_output(30, LOW);
    gpio_request(31, "PINOUT2");
    gpio_direction_output(31, LOW);
    gpio_request(48, "PINOUT3");
    gpio_direction_output(48, LOW);

    gpio_set_value(31, HIGH);
    gpio_set_value(48, HIGH);
    
    thread1 = kthread_run(thr_func, NULL , "p_thread");

    pr_info("Hello\n");

 
    return 0;

}

/* Destructor */
static void __exit gpio_exit(void)
{
  
    kthread_stop(thread1);
    gpio_set_value(30, LOW);
    gpio_set_value(31, LOW);
    gpio_set_value(48, LOW);

    
    gpio_free(30);
    gpio_free(31);
    gpio_free(48);
    gpio_free(60);
    pr_info("Good bye\n");

}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_VERSION("1.0"); 