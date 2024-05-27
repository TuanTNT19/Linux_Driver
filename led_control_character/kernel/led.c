#include <linux/module.h>           /* Defines functions such as module_init/module_exit */
#include <linux/gpio.h>             /* Defines functions such as gpio_request/gpio_free */
#include <linux/platform_device.h>  /* For platform devices */
#include <linux/gpio/consumer.h>    /* For GPIO Descriptor */
#include <linux/of.h>               /* For DT */  
#include <linux/interrupt.h>
#include <linux/fs.h>      /* Thu vien nay dinh nghia cac ham allocate major/minor number */
#include <linux/device.h>  /* Thu vien nay dinh nghia cac ham class_create/device_create */
#include <linux/cdev.h>    /* Thu vien nay dinh nghia cac ham kmalloc */
#include<linux/slab.h>     /* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include<linux/uaccess.h>  /* Thu vien nay dinh nghia cac ham copy_to_user/copy_from_user *//* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */

static int my_pdrv_probe(struct platform_device *pdev);
static int my_pdrv_remove(struct platform_device *pdev);


static int __init Khoitao(void);
static void __exit Huy(void);

static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static ssize_t  m_write(struct file *filp, const char *user_buf, size_t size, loff_t * offset);


typedef struct {
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
    int size;
} m_foo_dev;

char kernel_buff[1];

m_foo_dev mdev;

struct gpio_desc *gpio0_14;

static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "led_test", },
    { /* sentinel */ }
};

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

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .write      = m_write,
    .open       = m_open,
    .release    = m_release,
};

static int my_pdrv_probe(struct platform_device *pdev){
    pr_info("Probe function\n");
    struct device *dev = &pdev->dev;
    gpio0_14 = gpiod_get(dev, "led61", GPIOD_OUT_HIGH);
    pr_info("%s - %d", __func__, __LINE__);
    return 0;

}

static int my_pdrv_remove(struct platform_device *pdev){

    gpiod_put(gpio0_14);
    return 0;

}

static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called...!!!\n");
    return 0;
}

/* This function will be called when we close the Device file */
static int m_release(struct inode *inode, struct file *file)
{
    pr_info("System call close() called...!!!\n");
    return 0;
}


/* This function will be called when we write the Device file */
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset)
{

    /* Copy the buffer from user */
    if (copy_from_user(kernel_buff, user_buf, size))
    {
        pr_err("%s - copy_from_user failed\n", __func__);
        return -EFAULT;
    }

    if ( !strncmp("1", kernel_buff, 1))
    {
        gpiod_set_value(gpio0_14, 1);
    }
    else{
        gpiod_set_value(gpio0_14, 0);
    }

    /* Make the buffer empty */
    memset(kernel_buff, 0, sizeof(kernel_buff));
    
    return size;
}


static int __init Khoitao(void)
{
    if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "my-cdevt-led"))
    {
        pr_err("ERROR: Can not make number device\n");
        return -1;
    }
    pr_info("Kho tao : bat dau\n");
    
    pr_info("Major : %d    Minor : %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));
    
    if ((mdev.m_class = class_create(THIS_MODULE, "my_class_led")) == NULL){
        pr_err("ERROR: Can not create class\n");
        goto rm_dev_num;
    }
    pr_info("Init : Khoi tao class create\n");

    if (device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "my_device_led") == NULL)
    {
        pr_err("ERROR: Can not create device\n");
        goto rm_class;

    }
    pr_info("Init : Khoi tao device create\n");

    cdev_init(&mdev.m_cdev, &fops);
    pr_info("Init : Khoi tao Cdev Init\n");
    if (cdev_add(&mdev.m_cdev, mdev.dev_num, 1) < 0)
    {
        pr_err("ERROR: Can not add device\n");
        goto rm_dev_num;
    }
    pr_info("Init : Khoi tao Cdev add\n");

    platform_driver_register(&mypdrv);
    pr_info("register platform device\n");
    
    pr_info("Khoi tao ket thuc\n");

    return 0;

rm_dev_num:
    unregister_chrdev_region(mdev.dev_num,1);
rm_class:
    class_destroy(mdev.m_class);

    return -1;
}

static void __exit Huy(void) {
    pr_info("Start Huy\n");

    platform_driver_unregister(&mypdrv);
    pr_info("unregister platform device\n");

    cdev_del(&mdev.m_cdev); 
    pr_info("Exit: huy cdev");

    device_destroy(mdev.m_class, mdev.dev_num);
    pr_info("Exit: huy device_destroy\n");

    class_destroy(mdev.m_class);
    pr_info("Exit: check huy class_destroy\n");


    unregister_chrdev_region(mdev.dev_num, 1);
    pr_info("Exit: check huy number\n");

    pr_info("End Huy\n");
}

module_init(Khoitao);
module_exit(Huy);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("tuantnt08@gmail.com");
MODULE_DESCRIPTION("LED_ON_OFF");
