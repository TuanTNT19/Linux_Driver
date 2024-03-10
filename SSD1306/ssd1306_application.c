#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <netdb.h>
#include <errno.h>

#define SSD1306_DEV_FILE    "/dev/my_device_ssd"
#define MAX_CHAR_PER_LINE   21


int ret;
int fd;
char user_cmd[50];
//char temp_cmd[10];
//char *user_msg;

int main()
{
    /* Try open the LCD dev file */
    fd = open(SSD1306_DEV_FILE, O_WRONLY);

    if (fd == -1)
    {
        printf("Open /dev/my_ssd1306 failed. Please check the dev folder\n");
        exit(EXIT_FAILURE);
    }
    
    write(fd, "clear", 5);
    system("clear");  

    while(1){
        write(fd, "clear", 5);
        printf("Enter your data: ");
        fgets(user_cmd, 50, stdin);
        ret = write(fd, user_cmd, 50);
        if (ret == -1)
        {
            printf("Can not write\n");
        }
    }

    return 0;

}