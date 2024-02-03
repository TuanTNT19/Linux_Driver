#include <linux/module.h>  /* Thu vien nay dinh nghia cac macro nhu module_init/module_exit */
#include <linux/fs.h>      /* Thu vien nay dinh nghia cac ham allocate major/minor number */
#include <linux/device.h>  /* Thu vien nay dinh nghia cac ham class_create/device_create */
#include <linux/cdev.h>    /* Thu vien nay dinh nghia cac ham kmalloc */
#include<linux/slab.h>     /* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */
#include<linux/uaccess.h>  /* Thu vien nay dinh nghia cac ham copy_to_user/copy_from_user *//* Thu vien nay dinh nghia cac ham cdev_init/cdev_add */


#define DRIVER_AUTHOR "TuanTNT xxxxxxxx@gmail.com"
#define DRIVER_DESC   "Hello world kernel module"

typedef struct {
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
    int size;
    //char *k_buff;
} m_foo_dev;

char kernel_buff[40];

m_foo_dev mdev;

static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static ssize_t  m_read(struct file *filp, char __user *user_buf, size_t size,loff_t * offset);
static ssize_t  m_write(struct file *filp, const char *user_buf, size_t size, loff_t * offset);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = m_read,
    .write      = m_write,
    .open       = m_open,
    .release    = m_release,
};


/* This function will be called when we open the Device file */
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

/* This function will be called when we read the Device file */
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset)
{
    pr_info("System call read() called...!!!\n");

    // Check if the offset is beyond the end of the buffer
    if (*offset >= mdev.size) {
        return 0; // End of file
    }

    // Determine how many bytes to read (up to the end of the buffer)
    size_t bytes_to_read = min(size, (size_t)(mdev.size - *offset));

    // Copy data from kernel space to user space
    if (copy_to_user(user_buf, kernel_buff + *offset, bytes_to_read) != 0) {
        pr_err("Can not copy kernel to user\n");
        return -EFAULT;
    }

    // Update the offset and return the number of bytes read
    *offset += bytes_to_read;

    //kfree(mdev.k_buff);
    return bytes_to_read;
}

/* This function will be called when we write the Device file */
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset)
{
    pr_info("System call write() called...!!!\n");
    //mdev.k_buff = kmalloc(size, GFP_KERNEL);

    if (copy_from_user(kernel_buff, user_buf, size)){
        pr_err("Can not copy user to kernel\n");
        //kfree(mdev.k_buff);
        return -EFAULT;
    }

    pr_info("Data from user space: %s\n", kernel_buff);

    mdev.size = size;
    //kfree(mdev.k_buff);

    return size;
}



static int __init Khoitao(void)
{
    if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "my-cdev"))
    {
        pr_err("ERROR: Can not make number device\n");
        return -1;
    }
    pr_info("Hello from main\n");
    
    pr_info("Major : %d    Minor : %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));
    
    if ((mdev.m_class = class_create(THIS_MODULE, "my_class")) == NULL){
        pr_err("ERROR: Can not create class\n");
        goto rm_dev_num;
    }

    if (device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "my_device") == NULL)
    {
        pr_err("ERROR: Can not create device\n");
        goto rm_class;

    }

    cdev_init(&mdev.m_cdev, &fops);
    if (cdev_add(&mdev.m_cdev, mdev.dev_num, 1) < 0)
    {
        pr_err("ERROR: Can not add device\n");
        goto rm_dev_num;
    }

    return 0;


rm_dev_num:
    unregister_chrdev_region(mdev.dev_num,1);
rm_class:
    class_destroy(mdev.m_class);

    return -1;
}

static void __exit Huy(void)
{
    device_destroy(mdev.m_class, mdev.dev_num);
    class_destroy(mdev.m_class);
    cdev_del(&mdev.m_cdev);  
    unregister_chrdev_region(mdev.dev_num,1);

    pr_info("GOODBYE from main\n");

}


module_init(Khoitao);
module_exit(Huy);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);  

