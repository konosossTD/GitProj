#include "ds1302.h"
#include "intrins.h"



uchar time_buf[8] = {0x20,0x21,0x12,0x13,0x17,0x54,0x00,0x43};//��ʼʱ��
uchar realtime[14];//��ǰʱ��
uchar sec_buf=0;  //��buf
uchar sec_flag=0; //���־λ
uint time_buf_ten[8];
bit mute_flag=0;

void Clock_Init(void) 
{
	RST=0;			//RST���õ�
	SCK=0;			//SCK���õ�
}

void Clock_write(uchar addr, uchar d) 
{
	uchar i;
	RST=1;					//����DS1302����	
	//д��Ŀ���ַ��addr
	addr = addr & 0xFE;   //���λ���㣬�Ĵ���0λΪ0ʱд��Ϊ1ʱ��
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			DIO=1;
			}
		else {
			DIO=0;
			}
		SCK=1;      //����ʱ��
		SCK=0;
		addr = addr >> 1;
		}	
	//д�����ݣ�d
	for (i = 0; i < 8; i ++) {
		if (d & 0x01) {
			DIO=1;
			}
		else {
			DIO=0;
			}
		SCK=1;    //����ʱ��
		SCK=0;
		d = d >> 1;
		}
	RST=0;		//ֹͣ����
}


uchar Clock_read(uchar addr) {

	uchar i,temp;	
	RST=1;					//����DS1302����

	//д��Ŀ���ַ��addr
	addr = addr | 0x01;    //���λ�øߣ��Ĵ���0λΪ0ʱд��Ϊ1ʱ��
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			DIO=1;
			}
		else {
			DIO=0;
			}

		SCK=1;

		SCK=0;

		addr = addr >> 1;
		}	
	//������ݣ�temp
	for (i = 0; i < 8; i ++) {
		temp = temp >> 1;
		if (DIO) {
			temp |= 0x80;
			}
		else {
			temp &= 0x7F;
			}

		SCK=1;

		SCK=0;
		}	

	RST=0;					//ֹͣDS1302����
	return temp;
}

void Clock_write_time(void) 
{
	Clock_write(ds1302_control_add,0x00);			//�ر�д���� 
	Clock_write(ds1302_sec_add,0x80);				//��ͣʱ�� 
	Clock_write(ds1302_charger_add,0xa9);	    	//������ 
	Clock_write(ds1302_year_add,time_buf[1]);		//�� 
	Clock_write(ds1302_month_add,time_buf[2]);	//�� 
	Clock_write(ds1302_date_add,time_buf[3]);		//�� 
	Clock_write(ds1302_hr_add,time_buf[4]);		//ʱ 
	Clock_write(ds1302_min_add,time_buf[5]);		//��
	Clock_write(ds1302_sec_add,time_buf[6]);		//��
	Clock_write(ds1302_day_add,time_buf[7]);		//�� 
	Clock_write(ds1302_control_add,0x80);			//��д����     
}

void Clock_read_time(void)  //����buffer��
{

	time_buf[1]=Clock_read(ds1302_year_add);		//��ʮλ�͸�λ s
	time_buf[2]=Clock_read(ds1302_month_add);		//��ʮλ�͸�λ
	time_buf[3]=Clock_read(ds1302_date_add);		//��ʮλ�͸�λ
	time_buf[4]=Clock_read(ds1302_hr_add);			//ʱʮλ�͸�λ
	time_buf[5]=Clock_read(ds1302_min_add);			//��ʮλ�͸�λ
	time_buf[6]=(Clock_read(ds1302_sec_add))&0x7f;//��ʮλ�͸�λ��������ĵ�7λ�����ⳬ��59
	time_buf[7]=Clock_read(ds1302_day_add);		//��
	if(time_buf[4]>0x22 || time_buf[4]<0x07){mute_flag=1;}
	else{mute_flag=0;}
}

void separate()//
{
	uint i=0;
	Clock_read_time();  //��ȡʱ�� 
	realtime[0]=(time_buf[0]>>4);   //��ǧλ
	realtime[1]=(time_buf[0]&0x0F); //���λ 
	realtime[2]=(time_buf[1]>>4);   //��ʮλ
	realtime[3]=(time_buf[1]&0x0F); //���λ 
	realtime[4]=(time_buf[2]>>4);   //��ʮλ
	realtime[5]=(time_buf[2]&0x0F); //�¸�λ 
	realtime[6]=(time_buf[3]>>4);   //��ʮλ
	realtime[7]=(time_buf[3]&0x0F); //�ո�λ 
	realtime[8]=(time_buf[4]>>4);   //Сʮλ
	realtime[9]=(time_buf[4]&0x0F); //С��λ 
	realtime[10]=(time_buf[5]>>4);   //��ʮλ
	realtime[11]=(time_buf[5]&0x0F); //�ָ�λ
	realtime[12]=(time_buf[6]>>4);   //��ʮλ
	realtime[13]=(time_buf[6]&0x0F); //���λ   
//	for(i=0;i<14;i++)
//	{	
//		Send_Byte(realtime[i]+'0');

//	
//	}
}


