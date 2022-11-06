#ifndef __DHT11_H 
#define __DHT11_H 

#include "HT66F2390.h"
#define u8 unsigned char
#define u32 unsigned int
#define u16 unsigned short
void DHT11_Rst();
u8 DHT11_rec_byte();
void DHT11_receive();
void DHT11_time_init();
#endif