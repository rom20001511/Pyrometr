#ifndef COMMAND_H_
#define COMMAND_H_

#define Software_Reset 0x01
#define Sleep_mode 0x10
#define Sleep_out 0x11
#define Patrial_Mode_ON 0x12
#define Normal_Display_mode 0x13
#define Display_OFF 0x28
#define Display_ON 0x29
#define Column_Address_Set 0x2a
#define Page_Adress_Set 0x2b
#define Memory_Write 0x2c
#define Color_Set 0x2d
#define Memory_Access_Control 0x36 // for the order of rendering
#define Idle_Mode_OFF 0x38
#define Idle_Mode_ON 0x39
#define Pixel_Format_Set 0x3a
#define Pixel_16_bits 0x55


//MADCTL (Memory Access Control) the order of rendering

#define MADCTL_MY 0x80  //  where 0 by x
#define MADCTL_MX 0x40 // where 0 by y
#define MADCTL_ML 0x10 // vertical update order
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH 0x04 // horizontal update order

#define ROTATION (MADCTL_MX | MADCTL_BGR)


#endif /* COMMAND_H_ */
