/************************************************
���ܽ��ܣ������ӹ��ܵĶ�ʱ����װ��
�˿ڣ�
	P0  -->  ����ܶ�λ��ʾ
	P2.0 ~ P2.3  -->  �����λ��
	P2.5  -->  ����������������ʱΪ�͵�ƽ)
	P3.2 ~ P3.5  -->  ��ť����
*************************************************/

#include <stdio.h>
#include "SystemFunc.h"
#include "eeprom.h"

/************************************************
��������
************************************************/
uint count=0;  // ������ֵ
uchar Dis_code[]
={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar dis_buff[4];  //�������ʾ����������
uchar MODE=0;  // ģʽѡ����������
uint clock[3]={0,1,2};  // ��һ��ʵʱʱ�䣬2���������ã�������ʱ����ʱ��
bit Clock_Status=0;  // ���ӿ��ر�־
bit is_open = 0;  // �����Ѿ�����ı�־λ
uint this_time;  // ����ʱ���Ѿ������ֹ�ر����ӵ�ʱ�仹��������ʱ���ʱ���ظ�������

// buzzer
sbit output=P2^5;
// buzzer control button
sbit KEY_CONTROL = P3^3;
// clock status led
sbit status_led = P1^7;
/************************************************/

/*
�ⲿ�ж�1��Ӧ����
���ӿ��ư�ť������û��ʱ�������ӿ��أ������Ѿ�����ʱ�ر�����
*/
void EXT1(void) interrupt 2
{
	Delay(2);
	if(KEY_CONTROL == 0)  // ��ʱ����
	{
		if (is_open)  // ��������Ѿ�����
		{
			output = 1;  // �ر�����
			is_open = 0;  // �����־λ
		}
		else  // �������û��
		{
			Clock_Status=!Clock_Status;  // �ر����ӿ���
			status_led = !Clock_Status; 
		}
	}
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
	for(j=1;j<3;j++)  // ���������������ʱ��
	{
			if((clock[0] == clock[j]) && clock[0] != this_time)  // ��ǰʱ���Ƿ��Ѿ������趨������ʱ��
			{
				output=0;  // ����������
				is_open = 1;  // ���ñ�־λ
				this_time = clock[0];
			}
	}
}

void main(void)
{
	// ����ʼʱ�ȴӵ�eeprom�������õ�����ʱ��
	clock[1] = eeprom_read_byte(0x2800) | ((uint)eeprom_read_byte(0x2801) << 8);
	clock[2] = eeprom_read_byte(0x2802) | ((uint)eeprom_read_byte(0x2803) << 8);
	INT_Init();  // �жϳ�ʼ��
	PT0=1;  // ���ö�ʱ��0Ϊ������ȼ�
	P1 &= 0xfe;  // Init the start led status
	output = 1;
	while(1)
	{
		Bin2Bcd(clock[MODE]);  // ����ܱ���
		display();  // �������ʾ
		//deal_cmd();
		key_press();  // ��鰴���Ƿ���
		if(Clock_Status==0) continue;  // ������ӱ��رվͲ�ִ����������
		check_clock();
	}
}