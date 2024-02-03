#ifndef LED_H
#define LED_H

//define some address macros
#define GPIO_Start_addr 0x44E07000
#define GPIO_End_addr   0x44E07FFF
#define GPIO_Size (GPIO_End_addr - GPIO_Start_addr)
//define offset macros
#define GPIO_OE_OFFSET 0x134
#define GPIO_SETOUT_OFFSET 0x194
#define GPIO_CLEAROUT_OFFSET 0x190
#define GPIO_DATAOUT_OFFSET 0x13C

#endif LED_H
