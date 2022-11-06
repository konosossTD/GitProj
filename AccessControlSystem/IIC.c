#include "delay.h"
#include <STC12C5A60S2.H>
#include "intrins.h"
#include "IIC.h"



void IIC_start(void)
{
        SDA=1;
        _nop_();
        SCL=1;
        _nop_();
//        _nop_();
//        _nop_();
//        _nop_();
        _nop_();
        SDA=0;
        _nop_();
//        _nop_();
//        _nop_();
//        _nop_();
//        _nop_();
        SCL=0;
}




//********************************** IIC ����д1���ֽ� ******************************************


void IIC_writeByte(char temp)
{
        char i;
        for(i=0;i<8;i++)
        {
                SDA=(bit)(temp & 0x80) ;   // ���ݹ涨1602���������λ����Ϊ  1  
                temp <<=1;
//                _nop_();
//                _nop_();
                SCL=1;
//                _nop_();
//                _nop_();
//                _nop_();
//                _nop_();
//                _nop_();
                SCL=0;
        }
//        _nop_(); 
//        _nop_();
//        _nop_();
//        _nop_();
        SDA=1;
//        _nop_(); 
//        _nop_(); 
//        _nop_();
//        _nop_();
        SCL=1; 
//        _nop_();
//        _nop_();
//        _nop_();
        while(SDA);
//        _nop_();
        SCL=0;
}




