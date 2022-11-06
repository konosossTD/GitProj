#include <STC12C5A60S2.H>
#include "rc522.h"
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int.
void delay_ns(unsigned int ns)
{
	unsigned int i;
	for(i=0;i<ns;i++)
	{
		_nop_();
		_nop_();
	}
}

unsigned char SPIReadByte(void)// 读SPI数据
{
	unsigned char SPICount;                                       // Counter used to clock out the data
	unsigned char SPIData; 
	                 
	SPIData = 0;
	for (SPICount = 0; SPICount < 8; SPICount++)                  // Prepare to clock in the data to be read
	{
		SPIData <<=1;                                               // Rotate the data
		CLR_SPI_CK; //nop();//nop();                                         // Raise the clock to clock the data out of the MAX7456
		if(STU_SPI_MISO)
		{
 			SPIData|=0x01;
		}
		SET_SPI_CK;   //nop();//nop();                                               // Drop the clock ready for the next bit
	}                                                             // and loop back
	return (SPIData);                              // Finally return the read data
} 

void SPIWriteByte(unsigned char SPIData)// 写SPI数据
{
	unsigned char SPICount;                                       // Counter used to clock out the data
	for (SPICount = 0; SPICount < 8; SPICount++)
	{
		if (SPIData & 0x80)
		{
			SET_SPI_MOSI;
		}
		else
		{
			CLR_SPI_MOSI;
		} 
		_nop_();_nop_();
		CLR_SPI_CK;_nop_();_nop_();
		SET_SPI_CK;_nop_();_nop_();
		SPIData <<= 1;
	}         	
}


/********************************************************************
功    能：读RC632寄存器
参数说明：Address[IN]:寄存器地址
返    回：读出的值
***********************************************************************/
unsigned char ReadRawRC(unsigned char Address)
{
	unsigned char ucAddr;
  unsigned char ucResult=0;
	CLR_SPI_CS;
  ucAddr = ((Address<<1)&0x7E)|0x80;
	SPIWriteByte(ucAddr);
	ucResult=SPIReadByte();
	SET_SPI_CS;
	return ucResult;
}

/********************************************************************
功    能：写RC632寄存器
参数说明：Address[IN]:寄存器地址
          value[IN]:写入的值
***********************************************************************/
void WriteRawRC(unsigned char Address, unsigned char value)
{  
  unsigned char ucAddr;
	CLR_SPI_CS;
  ucAddr = ((Address<<1)&0x7E);
	SPIWriteByte(ucAddr);
	SPIWriteByte(value);
	SET_SPI_CS;
}   

/********************************************************************
功    能：清RC522寄存器位
参数说明：reg[IN]:寄存器地址
          mask[IN]:清位值
***********************************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
  char tmp = 0x00;
  tmp = ReadRawRC(reg);
  WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/********************************************************************
功    能：置RC522寄存器位
参数说明：reg[IN]:寄存器地址
          mask[IN]:置位值
***********************************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
  char tmp = 0x00;
  tmp = ReadRawRC(reg);
  WriteRawRC(reg,tmp | mask);  // set bit mask
}

/********************************************************************
功    能：通过RC522和ISO14443卡通讯
参数说明：Command[IN]:RC522命令字
          pInData[IN]:通过RC522发送到卡片的数据
          InLenByte[IN]:发送数据的字节长度
          pOutData[OUT]:接收到的卡片返回数据
          *pOutLenBit[OUT]:返回数据的位长度
***********************************************************************/
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
      case PCD_AUTHENT:
			  irqEn   = 0x12;
			  waitFor = 0x10;
			  break;
		  case PCD_TRANSCEIVE:
			  irqEn   = 0x77;
			  waitFor = 0x30;
			  break;
		  default:
			  break;
    }
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<InLenByte; i++)
    {   
			WriteRawRC(FIFODataReg, pInData[i]);    
		}
    WriteRawRC(CommandReg, Command);
    if (Command == PCD_TRANSCEIVE)
    {    
			SetBitMask(BitFramingReg,0x80);  
		}
    
    //i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
	  i = 2000;
    do 
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor)) ;
    ClearBitMask(BitFramingReg,0x80);
    if (i!=0)
    {    
        if(!(ReadRawRC(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   
								status = MI_NOTAGERR;   
						}
            if (Command == PCD_TRANSCEIVE)
            {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   
										*pOutLenBit = (n-1)*8 + lastBits;   
								}
                else
                {   
										*pOutLenBit = n*8;   
								}
                if (n == 0)
                {   
										n = 1;    
								}
                if (n > MAXRLEN)
                {   
										n = MAXRLEN;   
								}
                for (i=0; i<n; i++)
                {   
										pOutData[i] = ReadRawRC(FIFODataReg);    
								}
            }
        }
        else
        {   
			     status = MI_ERR;   
		    }
        
    }
    SetBitMask(ControlReg,0x80);           //停止时间
    WriteRawRC(CommandReg,PCD_IDLE); 
    return status;
}
                      
/********************************************************************
功    能：寻卡
参数说明: req_code[IN]:寻卡方式
          0x52 = 寻感应区内所有符合14443A标准的卡
          0x26 = 寻未进入休眠状态的卡
          pTagType[OUT]：卡片类型代码
          0x4400 = Mifare_UltraLight
          0x0400 = Mifare_One(S50)
          0x0200 = Mifare_One(S70)
          0x0800 = Mifare_Pro(X)
          0x4403 = Mifare_DESFire
return: 成功返回MI_OK
***********************************************************************/
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
	char status;  
	unsigned int unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 

	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
 
	ucComMF522Buf[0] = req_code;

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   
		status = MI_ERR;   
	}
   
	return status;
}


/********************************************************************
功    能：防冲撞
参数说明: pSnr[OUT]:卡片序列号，4字节
返    回: 成功返回MI_OK
***********************************************************************/ 
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
		{
			for (i=0; i<4; i++)
			{   
				*(pSnr+i)  = ucComMF522Buf[i];
				snr_check ^= ucComMF522Buf[i];
			}
			if (snr_check != ucComMF522Buf[i])
   		{   
				status = MI_ERR;    
			}
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

char PcdReset(void)//复位RC522
{
	SET_RC522RST;
    delay_ns(10);
	CLR_RC522RST;
    delay_ns(10);
	SET_RC522RST;
    delay_ns(10);
  WriteRawRC(CommandReg,PCD_RESETPHASE);
    delay_ns(10);
    
  WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
  WriteRawRC(TReloadRegL,30);           
  WriteRawRC(TReloadRegH,0);
  WriteRawRC(TModeReg,0x8D);
  WriteRawRC(TPrescalerReg,0x3E);
	WriteRawRC(TxAutoReg,0x40);//必须要
  return MI_OK;
}

char M500PcdConfigISOType(unsigned char type)//设置RC632的工作方式 
{
   if (type == 'A')                     //ISO14443_A
   { 
       ClearBitMask(Status2Reg,0x08);
       WriteRawRC(ModeReg,0x3D);//3F
       WriteRawRC(RxSelReg,0x86);//84
       WriteRawRC(RFCfgReg,0x7F);   //4F
   	   WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
			 WriteRawRC(TReloadRegH,0);
       WriteRawRC(TModeReg,0x8D);
	     WriteRawRC(TPrescalerReg,0x3E);
	     delay_ns(1000);
       PcdAntennaOn();
   }
   else
	 { 
			return -1; 
	 }
   return MI_OK;
}
 
//每次启动或关闭天险发射之间应至少有1ms的间隔
void PcdAntennaOn(void)//开启天线 
{
    unsigned char i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

void PcdAntennaOff(void)//关闭天线
{
	ClearBitMask(TxControlReg, 0x03);
}

void init_rc522(void)
{
  PcdReset();
  PcdAntennaOff();  
  PcdAntennaOn();
  M500PcdConfigISOType( 'A' );
}


uchar IC_READ( void )	//读卡
{
	uchar i;
	if( PcdRequest( PICC_REQIDL, Card_type ) != MI_OK )    //寻天线区内未进入休眠状态的卡，返回卡片类型 2字节
	{
			return 0;
	}
	if( PcdAnticoll( Card_type ) != MI_OK )                  //防冲撞，返回卡的序列号 4字节
	{
		return 0;
	}										
	for(i=0; i<4; i++) //卡ID号转化成ASCII
	{
			LCD_Data(Card_type[i],i);
	}
	return 1;
}









void LCD_Data(uchar LCDbyte,uchar i )
	{
		 uchar bai,shi;	//十进制
		 bai=LCDbyte/100;
		 shi=LCDbyte%100/10;
		 if(i==0)
		 {
			ID_ASC[0]=bai+0x30;
			ID_ASC[1]=shi+0x30;
		 }
		 if(i==1)
		 {
			 ID_ASC[2]=bai+0x30;
			 ID_ASC[3]=shi+0x30;
		 }
		 if(i==2)
		 {
			 ID_ASC[4]=bai+0x30;
		   ID_ASC[5]=shi+0x30;
		 }
		 if(i==3)
		 {
				ID_ASC[6]=bai+0x30;
				ID_ASC[7]=shi+0x30;
		 }
	}	

	uchar IC_READ( void )	//读卡
	{
		uchar i;
		if( PcdRequest( PICC_REQIDL, Card_type ) != MI_OK )    //寻天线区内未进入休眠状态的卡，返回卡片类型 2字节
		{
				return 0;
		}
		if( PcdAnticoll( Card_type ) != MI_OK )                  //防冲撞，返回卡的序列号 4字节
		{
			return 0;
		}										
		for(i=0; i<4; i++) //卡ID号转化成ASCII
		{
				LCD_Data(Card_type[i],i);
		}
		return 1;
	}


	void checkcard(void)	//校验卡
	{
		uchar j;
		uchar IDAddress;
		IC_READ();
		for(idnum=0;idnum<6;idnum++)
		{
			IDAddress= numaddress[idnum];
			for(j=0;j<8;j++)
			{
				IDn[j]=	x24c02_read(IDAddress);
				delayms(10);
				IDAddress++;
			}
			if( (ID_ASC[0]==IDn[0])&&(ID_ASC[1]==IDn[1])&&(ID_ASC[2]==IDn[2])&&(ID_ASC[3]==IDn[3])&&(ID_ASC[4]==IDn[4])&&(ID_ASC[5]==IDn[5])&&(ID_ASC[6]==IDn[6])&&(ID_ASC[7]==IDn[7]) )
			  break;
		}
		if(idnum<6) flag_id=1;
		else flag_id=0; 
	}

	void shuaka(void)	//刷卡
	{
		if(IC_READ()==1)
		{
			checkcard();    
			if(flag_id==1)//刷卡扫描
			{
				flag_id=0;
				LcmClearTXT();
				WriteCommand(0x89);
				WriteCommand(0x8B);
				PutStr(1,2,"门已打开");
				jidianqi=0;
				PutStr(2,0,"卡号：");
				WriteCommand(0x8B);
				WriteData(ID_ASC[0]);
				WriteData(ID_ASC[1]);
				WriteData(ID_ASC[2]);
				WriteData(ID_ASC[3]);
				WriteData(ID_ASC[4]);
				WriteData(ID_ASC[5]);
				WriteData(ID_ASC[6]);
				WriteData(ID_ASC[7]);
				delay(2500); 
				jidianqi=1;	
				delay(1000);
				LcmClearTXT();
				PutStr(0,0,"    欢迎使用    ");
				PutStr(1,0,"  多功能门禁锁  ");
				PutStr(2,0,"  请输入密码：  ");
			}
			else
			{
				LcmClearTXT();
				PutStr(1,2,"卡未注册");
				PutStr(2,0,"卡号：");
				WriteCommand(0x8B);
				WriteData(ID_ASC[0]);
				WriteData(ID_ASC[1]);
				WriteData(ID_ASC[2]);
				WriteData(ID_ASC[3]);
				WriteData(ID_ASC[4]);
				WriteData(ID_ASC[5]);
				WriteData(ID_ASC[6]);
				WriteData(ID_ASC[7]);
				beep=0;
				delay(300);
				beep=1;
				delay(3000);
				LcmClearTXT();
				PutStr(0,0,"    欢迎使用    ");
				PutStr(1,0,"  多功能门禁锁  ");
				PutStr(2,0,"  请输入密码：  ");
			}
		}
	}

	void kapianguanli(void)	//卡片管理
	{
		uchar numID[6]={0,1,2,3,4,5},i=0,k;
		uchar j,temp00;
		LcmClearTXT();
		PutStr(0,2,"请刷卡");
		KeyDeal(Keycan());
		while( Key!=tuichu )
		{
			WriteCommand(0x89);
			WriteData('I');
			WriteData('D');
			WriteData(0X30+numID[i]);
			WriteData(':');
			
			IC_READ();
			Key=Keycan();
		  KeyDeal(Key);
			temp00=numaddress[i];
			for(k=0;k<8;k++)
			{
				IDn[k]=	x24c02_read(temp00);
				delayms(10);
				temp00++;
			}
			temp00=numaddress[i];
			if((IDn[0]==0x30)&&(IDn[1]==0x30)&&(IDn[2]==0x30)&&(IDn[3]==0x30)&&(IDn[4]==0x30)&&(IDn[5]==0x30)&&(IDn[6]==0x30)&&(IDn[7]==0x30))
			{
				if(IC_READ()==1)
				{
				  WriteCommand(0x8B);
					WriteData(ID_ASC[0]);
					WriteData(ID_ASC[1]);
					WriteData(ID_ASC[2]);
					WriteData(ID_ASC[3]);
					WriteData(ID_ASC[4]);
					WriteData(ID_ASC[5]);
					WriteData(ID_ASC[6]);
					WriteData(ID_ASC[7]);
				}
				else
				{
					WriteCommand(0x8B);
					WriteData(' ');
					WriteData(' ');
					WriteData(' ');
					WriteData(' ');
					WriteData(' ');
					WriteData(' ');
					WriteData(' ');
					WriteData(' ');
				}
			}			
			else
			{
				WriteCommand(0x8B);
				WriteData(IDn[0]);
				WriteData(IDn[1]);
				WriteData(IDn[2]);
				WriteData(IDn[3]);
				WriteData(IDn[4]);
				WriteData(IDn[5]);
				WriteData(IDn[6]);
				WriteData(IDn[7]);	
			}
			if(Key==0x82)	//存储卡
			{
				 IC_READ();
				 if(IC_READ()==1)
				 {
					 checkcard();
					 if(flag_id==1)
					 {	 
						 flag_id=0;
						 LcmClearTXT();
						 PutStr(0,2,"卡已注册");
						 PutStr(1,2,"请换卡片");
						 PutStr(2,1,"按任意键继续");
						 while(Keycan()==0);
						 LcmClearTXT();
						 PutStr(0,2,"请刷卡");
					 }
					 else
					 {
						for(j=0;j<8;j++)
						 {
							 x24c02_write(temp00,ID_ASC[j]);
							 delayms(10);
							 temp00++;
						 }
						 LcmClearTXT();
						 PutStr(0,2,"注册成功");
						 PutStr(1,1,"按任意键继续");
						 while(Keycan()==0);
						 LcmClearTXT();
						 PutStr(0,2,"请刷卡");
					 }
				 }
			}
			if(Key==0x84)	//删除卡
			{
				 if((IDn[0]==0x30)&&(IDn[1]==0x30)&&(IDn[2]==0x30)&&(IDn[3]==0x30)&&(IDn[4]==0x30)&&(IDn[5]==0x30)&&(IDn[6]==0x30)&&(IDn[7]==0x30))
				 {
					 LcmClearTXT();
					 PutStr(1,2,"未存储卡");
					 PutStr(2,1,"按任意键继续");
					 while(Keycan()==0);
					 LcmClearTXT();
					 PutStr(0,2,"请刷卡");
				 }
				 else
				{
					for(j=0;j<8;j++)
					 {
						 IDn[j]=0;
						 x24c02_write(temp00,IDn[j]+0x30);
						 delayms(10);
						 temp00++;
					 }
					 delayms(50);
					 LcmClearTXT();
					 PutStr(0,2,"删去成功");
					 PutStr(1,1,"按任意键继续");
					 while(Keycan()==0);
					 LcmClearTXT();
					 PutStr(0,2,"请刷卡");
				 }	
			}
			if(Key==0x81)	//卡序号切换
			{
				i++;
			}
			if(i==6)
			{
				i=0;
			}
		
		}
	}

	