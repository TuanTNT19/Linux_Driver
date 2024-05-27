#include "stdio.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_FILE "/dev/my_device_led"

int main()
{
    int t;
    char buff[2];
    int fd = open(DEVICE_FILE, O_WRONLY);
    if (fd == -1)
    {
        printf("Can not open dev file \n");
        return -1;
    }

    while(1)
    {
        printf("1. ON LED      0. OFF LED\n");
        printf("Enter your answer : ");
        scanf("%d", &t);
        if (t == 1 || t == 0)
        {
            sprintf(buff, "%d", t);
            int w = write(fd, buff, strlen(buff));
            if (w == -1)
            {
                printf("Can not write to kernel \n");
                return -1;
            }

        }
        else {
            printf("Enter again !\n");
        }
        

    }

    close(fd);

    return 0;
}