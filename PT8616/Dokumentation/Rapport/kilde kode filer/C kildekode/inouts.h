#include <c8051f320.h>

//defination af inputs og outputs

//I2C til mainframe
sbit sda_io = P1^0;	
sbit scl_io = P1^1;

//intern I2C til FPGA
sbit sda_int_io = P1^3;
sbit scl_int_io = P1^2;

//Debug LEDs
sbit LED0_o = P2^0;
sbit LED1_o = P2^1;

//DIP
sbit DIP_i = P2^6;

//General purpose IOs
sbit gpio0_io = P2^2;
sbit gpio1_io = P2^3;
sbit gpio2_io = P2^4;
sbit gpio3_io = P2^5;
