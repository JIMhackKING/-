#include "SystemFunc.h"
#include "eeprom.h"

extern uchar MODE;
extern int clock[4];
sbit KEY1 = P3^4;
sbit KEY2 = P3^5;
sbit KEY3 = P3^2;

// ��ʱ
void add_time(void)
{
	clock[MODE]++;
	if(clock[MODE]>=1440)
		clock[MODE]=0;
}

// ��ʱ
void sub_time(void)
{
	clock[MODE]--;
	if(clock[MODE]<0)
		clock[MODE]=1439;
}

/************************************************
���ã���ťһ��Ӧ����
************************************************/
void key1_down(void)
{
	uchar i,j=20,n=3;
	Delay(5);  // ��ʱ����
	while(KEY1 == 0)
	{
		add_time();
		if(n==0)  // �������һֱ������ô���ֱ仯ʱ��Խ��Խ��
		{
			j-=5;
			n=3;
		}
		if(j<5) j=5;
		n-=1;
		for(i=0;i<j;i++)
		{
			Bin2Bcd(clock[MODE]);
			display();
		}
	}
}

/************************************************
���ã���ť����Ӧ����
************************************************/
void key2_down(void)
{
	uchar i,j=20,n=3;
	Delay(5);  // ��ʱ����
	while(KEY2 == 0)
	{
		sub_time();
		if(n==0)  // �������һֱ������ô���ֱ仯ʱ��Խ��Խ��
		{
			j-=5;
			n=3;
		}
		if(j<5) j=5;
		n-=1;
		for(i=0;i<j;i++)
		{
			Bin2Bcd(clock[MODE]);
			display();
		}
	}
}

/************************************************
���ã���ť����Ӧ����
************************************************/
void key3_down(void)
{
	uchar i;
	Delay(2);
	if(KEY3 == 0)
	{
		if(MODE == 1)  // ��eepromд���һ�����ӵ�ʱ��
		{
			eeprom_write_byte(0x2800, (uchar)clock[MODE]);
			eeprom_write_byte(0x2801, (uchar)(clock[MODE] >> 8));
		}
		else if (MODE == 2)  // ��eepromд��ڶ������ӵ�ʱ��
		{
			eeprom_write_byte(0x2802, (uchar)clock[MODE]);
			eeprom_write_byte(0x2803, (uchar)(clock[MODE] >> 8));
		}
		MODE++;  // �ı���ʾ��ģʽ
		if(MODE==3)
			MODE=0;
	}
	for(i=0;i<30;i++)
	{
		Bin2Bcd(clock[MODE]);
		display();
	}
}

// �ж��ĸ���ť�����²���������
void key_press(void)
{
	if(P3==0xef) key1_down();
	else if(P3==0xdf) key2_down();
	else if(P3==0xfb) 
	{
		key3_down();
		P1 &= 0xf0;  // ��������λ����
		P1 |= 0x0f & ~(1<<MODE);
	}
}
