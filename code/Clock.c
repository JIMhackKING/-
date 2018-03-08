/************************************************
���ܽ��ܣ������ӹ��ܵĶ�ʱ����װ��
�˿ڣ�
	P0  -->  ����ܶ�λ��ʾ
	P2.0 ~ P2.3  -->  �����λ��
	P1.0  -->  �̵�������������ʱΪ�͵�ƽ)
	P3.2 ~ P3.5  -->  ��ť����
*************************************************/

#include <stdio.h>
#include "SystemFunc.h"

/************************************************
��������
************************************************/
sbit output=P1^0;
uint count=0;  // ������ֵ
uchar Dis_code[]
={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar dis_buff[4];  //�������ʾ����������
uchar MODE=0;  // ģʽѡ����������
int time;  // ��Ϊ��λʱ��
int clock[4]={0,5,10,1};  // ��һ��ʵʱʱ�䣬2���������ã�������ʱ����ʱ��
bit open;  // �̵����˿ڿ�����־
bit Clock_Status=0;  // ���ӿ��ر�־
/************************************************/

void EXT1(void) interrupt 2
{
	Clock_Status=!Clock_Status;
	P1=~((char)Clock_Status<<7); 
}

/************************************************
���ã���ʱ��0�жϺ�������ʱʱ��Ϊ��
************************************************/
void TC0(void) interrupt 1
{
	// 11.0592MHz ����Ƶ�� 50ms ����ֵΪ 46080
	TH0=TH0+0x4c;  // ��ʼ����ֵ
	TL0=TL0+0x00;
	count++;
	if(count==1200)  // 1m
	{
		count=0;
		clock[0]++;
		if(clock[0]==1440)
			clock[0]=0;
	}
}

// ���ʱ���Ƿ�������ʱ��
void check_clock(void)
{
	uchar j;
	for(j=1;j<3;j++)
	{
		if(clock[0]==clock[j]&&open==0)
		{
			time=clock[0];
			output=0;
			open=1;
		}
	}
	if(open)
	{
		if(clock[0]-time==clock[3]||clock[0]<time)
		{
			output=1;
			open=0;
		}
	}
}

void main(void)
{
	INT_Init();
	UART_init();
	PT0=1;  // ���ö�ʱ��0Ϊ������ȼ�
	while(1)
	{
		Bin2Bcd(clock[MODE]);
		display();
		deal_cmd();
		key_press();
		if(Clock_Status==0) continue;
		check_clock();
	}
}