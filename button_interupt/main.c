#include <linux/module.h>           /* Defines functions such as module_init/module_exit */
#include <linux/gpio.h>             /* Defines functions such as gpio_request/gpio_free */
#include <linux/platform_device.h>  /* For platform devices */
#include <linux/gpio/consumer.h>    /* For GPIO Descriptor */
#include <linux/of.h>               /* For DT */  
#include <linux/interrupt.h>


#define DRIVER_AUTHOR "tuantnt08@gmail.com"
#define DRIVER_DESC   "gpio test device tree"

#define LOW     0
#define HIGH    1

struct gpio_desc *gpio2_4;
struct gpio_desc *gpio2_5;
struct gpio_desc *gpio1_13;
struct gpio_desc *gpio1_12;
struct gpio_desc *gpio0_23;
int irq1, irq2, irq3, irq4, irq5;
int check ;

static irqreturn_t handler1(int irq, void *dev_id);
static irqreturn_t handler2(int irq, void *dev_id);
static irqreturn_t handler3(int irq, void *dev_id);
static irqreturn_t handler4(int irq, void *dev_id);
static irqreturn_t handler5(int irq, void *dev_id);

static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "gpio_base_myself", },
    { /* sentinel */ }
};

static int my_pdrv_probe(struct platform_device *pdev)
{
    pr_info("Probe function\n");
    struct device *dev = &pdev->dev;
    gpio0_23 =  gpiod_get(dev, "button23", GPIOD_IN);
    gpio1_13 = gpiod_get(dev, "button45", GPIOD_IN);
    gpio1_12=  gpiod_get(dev, "button44", GPIOD_IN);
    gpio2_4=  gpiod_get(dev, "button68", GPIOD_IN);
    gpio2_5=  gpiod_get(dev, "button69", GPIOD_IN);

    gpiod_set_debounce(gpio0_23, 200);
    gpiod_set_debounce(gpio1_12, 200);
    gpiod_set_debounce(gpio1_13, 200);
    gpiod_set_debounce(gpio2_4, 200);
    gpiod_set_debounce(gpio2_5, 200);

    irq1 = gpiod_to_irq(gpio0_23);
    irq2 = gpiod_to_irq(gpio1_12);
    irq3 = gpiod_to_irq(gpio1_13);
    irq4 = gpiod_to_irq(gpio2_4);
    irq5 = gpiod_to_irq(gpio2_5);

    check = request_irq(irq1,handler1, IRQF_TRIGGER_FALLING, "button_irq1", NULL);
    if (check) {
    pr_err("Failed to request IRQ for Button GPIO\n");
    gpiod_put(gpio0_23);
    return check;
    }

    check = request_irq(irq2,handler2, IRQF_TRIGGER_FALLING, "button_irq2", NULL);
    if (check) {
    pr_err("Failed to request IRQ for Button GPIO\n");
    gpiod_put(gpio1_12);
    return check;
    }

    check = request_irq(irq3,handler3, IRQF_TRIGGER_FALLING, "button_irq3", NULL);
    if (check) {
    pr_err("Failed to request IRQ for Button GPIO\n");
    gpiod_put(gpio1_13);
    return check;
    }

    check = request_irq(irq4,handler4, IRQF_TRIGGER_FALLING, "button_irq4", NULL);
    if (check) {
    pr_err("Failed to request IRQ for Button GPIO\n");
    gpiod_put(gpio2_4);
    return check;
    }

    check = request_irq(irq5,handler5, IRQF_TRIGGER_FALLING, "button_irq5", NULL);
    if (check) {
    pr_err("Failed to request IRQ for Button GPIO\n");
    gpiod_put(gpio2_5);
    return check;
    }

    return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
    pr_info("remove function\n");

    free_irq(irq1, NULL);
    gpiod_put(gpio0_23);

    free_irq(irq2, NULL);
    gpiod_put(gpio1_12);

    free_irq(irq3, NULL);
    gpiod_put(gpio1_13);
    free_irq(irq4, NULL);
    gpiod_put(gpio2_4);

    free_irq(irq5, NULL);
    gpiod_put(gpio2_5);

    return 0;
}

static irqreturn_t handler1(int irq, void *dev_id)
{
    bool pressed = gpiod_get_value(gpio0_23);
    
    if (pressed)
        pr_info("Button1 Pressed\n");
    else
        pr_info("Button1 Released\n");

    return IRQ_HANDLED;
}

static irqreturn_t handler2(int irq, void *dev_id)
{
    bool pressed = gpiod_get_value(gpio1_12);
    
    if (pressed)
        pr_info("Button2 Pressed\n");
    else
        pr_info("Button2 Released\n");

    return IRQ_HANDLED;
}

static irqreturn_t handler3(int irq, void *dev_id)
{
    bool pressed = gpiod_get_value(gpio1_13);
    
    if (pressed)
        pr_info("Button3 Pressed\n");
    else
        pr_info("Button3 Released\n");

    return IRQ_HANDLED;
}

static irqreturn_t handler4(int irq, void *dev_id)
{
    bool pressed = gpiod_get_value(gpio2_4);
    
    if (pressed)
        pr_info("Button4 Pressed\n");
    else
        pr_info("Button4 Released\n");

    return IRQ_HANDLED;
}

static irqreturn_t handler5(int irq, void *dev_id)
{
    bool pressed = gpiod_get_value(gpio2_5);
    
    if (pressed)
        pr_info("Button5 Pressed\n");
    else
        pr_info("Button5 Released\n");

    return IRQ_HANDLED;
}



/* platform driver */
static struct platform_driver mypdrv = {
    .probe = my_pdrv_probe,
    .remove = my_pdrv_remove,
    .driver = {
        .name = "descriptor-based",
        .of_match_table = of_match_ptr(gpiod_dt_ids),
        .owner = THIS_MODULE,
    },
};
module_platform_driver(mypdrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");
