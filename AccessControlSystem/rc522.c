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

unsigned char SPIReadByte(void)// ��SPI����
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

void SPIWriteByte(unsigned char SPIData)// дSPI����
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
��    �ܣ���RC632�Ĵ���
����˵����Address[IN]:�Ĵ�����ַ
��    �أ�������ֵ
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
��    �ܣ�дRC632�Ĵ���
����˵����Address[IN]:�Ĵ�����ַ
          value[IN]:д���ֵ
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
��    �ܣ���RC522�Ĵ���λ
����˵����reg[IN]:�Ĵ�����ַ
          mask[IN]:��λֵ
***********************************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
  char tmp = 0x00;
  tmp = ReadRawRC(reg);
  WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/********************************************************************
��    �ܣ���RC522�Ĵ���λ
����˵����reg[IN]:�Ĵ�����ַ
          mask[IN]:��λֵ
***********************************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
  char tmp = 0x00;
  tmp = ReadRawRC(reg);
  WriteRawRC(reg,tmp | mask);  // set bit mask
}

/********************************************************************
��    �ܣ�ͨ��RC522��ISO14443��ͨѶ
����˵����Command[IN]:RC522������
          pInData[IN]:ͨ��RC522���͵���Ƭ������
          InLenByte[IN]:�������ݵ��ֽڳ���
          pOutData[OUT]:���յ��Ŀ�Ƭ��������
          *pOutLenBit[OUT]:�������ݵ�λ����
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
    
    //i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
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
    SetBitMask(ControlReg,0x80);           //ֹͣʱ��
    WriteRawRC(CommandReg,PCD_IDLE); 
    return status;
}
                      
/********************************************************************
��    �ܣ�Ѱ��
����˵��: req_code[IN]:Ѱ����ʽ
          0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
          0x26 = Ѱδ��������״̬�Ŀ�
          pTagType[OUT]����Ƭ���ʹ���
          0x4400 = Mifare_UltraLight
          0x0400 = Mifare_One(S50)
          0x0200 = Mifare_One(S70)
          0x0800 = Mifare_Pro(X)
          0x4403 = Mifare_DESFire
return: �ɹ�����MI_OK
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
��    �ܣ�����ײ
����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�
��    ��: �ɹ�����MI_OK
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

char PcdReset(void)//��λRC522
{
	SET_RC522RST;
    delay_ns(10);
	CLR_RC522RST;
    delay_ns(10);
	SET_RC522RST;
    delay_ns(10);
  WriteRawRC(CommandReg,PCD_RESETPHASE);
    delay_ns(10);
    
  WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
  WriteRawRC(TReloadRegL,30);           
  WriteRawRC(TReloadRegH,0);
  WriteRawRC(TModeReg,0x8D);
  WriteRawRC(TPrescalerReg,0x3E);
	WriteRawRC(TxAutoReg,0x40);//����Ҫ
  return MI_OK;
}

char M500PcdConfigISOType(unsigned char type)//����RC632�Ĺ�����ʽ 
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
 
//ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
void PcdAntennaOn(void)//�������� 
{
    unsigned char i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

void PcdAntennaOff(void)//�ر�����
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


uchar IC_READ( void )	//����
{
	uchar i;
	if( PcdRequest( PICC_REQIDL, Card_type ) != MI_OK )    //Ѱ��������δ��������״̬�Ŀ������ؿ�Ƭ���� 2�ֽ�
	{
			return 0;
	}
	if( PcdAnticoll( Card_type ) != MI_OK )                  //����ײ�����ؿ������к� 4�ֽ�
	{
		return 0;
	}										
	for(i=0; i<4; i++) //��ID��ת����ASCII
	{
			LCD_Data(Card_type[i],i);
	}
	return 1;
}









void LCD_Data(uchar LCDbyte,uchar i )
	{
		 uchar bai,shi;	//ʮ����
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

	uchar IC_READ( void )	//����
	{
		uchar i;
		if( PcdRequest( PICC_REQIDL, Card_type ) != MI_OK )    //Ѱ��������δ��������״̬�Ŀ������ؿ�Ƭ���� 2�ֽ�
		{
				return 0;
		}
		if( PcdAnticoll( Card_type ) != MI_OK )                  //����ײ�����ؿ������к� 4�ֽ�
		{
			return 0;
		}										
		for(i=0; i<4; i++) //��ID��ת����ASCII
		{
				LCD_Data(Card_type[i],i);
		}
		return 1;
	}


	void checkcard(void)	//У�鿨
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

	void shuaka(void)	//ˢ��
	{
		if(IC_READ()==1)
		{
			checkcard();    
			if(flag_id==1)//ˢ��ɨ��
			{
				flag_id=0;
				LcmClearTXT();
				WriteCommand(0x89);
				WriteCommand(0x8B);
				PutStr(1,2,"���Ѵ�");
				jidianqi=0;
				PutStr(2,0,"���ţ�");
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
				PutStr(0,0,"    ��ӭʹ��    ");
				PutStr(1,0,"  �๦���Ž���  ");
				PutStr(2,0,"  ���������룺  ");
			}
			else
			{
				LcmClearTXT();
				PutStr(1,2,"��δע��");
				PutStr(2,0,"���ţ�");
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
				PutStr(0,0,"    ��ӭʹ��    ");
				PutStr(1,0,"  �๦���Ž���  ");
				PutStr(2,0,"  ���������룺  ");
			}
		}
	}

	void kapianguanli(void)	//��Ƭ����
	{
		uchar numID[6]={0,1,2,3,4,5},i=0,k;
		uchar j,temp00;
		LcmClearTXT();
		PutStr(0,2,"��ˢ��");
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
			if(Key==0x82)	//�洢��
			{
				 IC_READ();
				 if(IC_READ()==1)
				 {
					 checkcard();
					 if(flag_id==1)
					 {	 
						 flag_id=0;
						 LcmClearTXT();
						 PutStr(0,2,"����ע��");
						 PutStr(1,2,"�뻻��Ƭ");
						 PutStr(2,1,"�����������");
						 while(Keycan()==0);
						 LcmClearTXT();
						 PutStr(0,2,"��ˢ��");
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
						 PutStr(0,2,"ע��ɹ�");
						 PutStr(1,1,"�����������");
						 while(Keycan()==0);
						 LcmClearTXT();
						 PutStr(0,2,"��ˢ��");
					 }
				 }
			}
			if(Key==0x84)	//ɾ����
			{
				 if((IDn[0]==0x30)&&(IDn[1]==0x30)&&(IDn[2]==0x30)&&(IDn[3]==0x30)&&(IDn[4]==0x30)&&(IDn[5]==0x30)&&(IDn[6]==0x30)&&(IDn[7]==0x30))
				 {
					 LcmClearTXT();
					 PutStr(1,2,"δ�洢��");
					 PutStr(2,1,"�����������");
					 while(Keycan()==0);
					 LcmClearTXT();
					 PutStr(0,2,"��ˢ��");
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
					 PutStr(0,2,"ɾȥ�ɹ�");
					 PutStr(1,1,"�����������");
					 while(Keycan()==0);
					 LcmClearTXT();
					 PutStr(0,2,"��ˢ��");
				 }	
			}
			if(Key==0x81)	//������л�
			{
				i++;
			}
			if(i==6)
			{
				i=0;
			}
		
		}
	}

	