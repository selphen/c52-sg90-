#include <REGX52.H>
#include <intrins.h>
#include "output.h"
#include "BeeToll.h"
#define PWM P2_1 // 舵机输入的口

unsigned char angle = 0; // 以挡位控制旋转方向（1~5档）
unsigned char output_flag = 0;

void Delay1ms(unsigned int xms) //@11.0592MHz
{
	unsigned char i, j;
	while (xms--)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j)
				;
		} while (--i);
	}
}

void Timer1init() // 500微秒@11.0592MHz
{
	TMOD |= 0x10; // 设置定时器模式

	TL1 = 0x33;
	TH1 = 0xFE;

	TF1 = 0; // 清除标志
	TR1 = 1; // 定时器开始计时
	ET1 = 1; // 定时器的中端允许开关
	EA = 1;	 // 定时器总开关
}

void TimerZhongDuan() interrupt 3 // 中断
{
	static unsigned char counter = 0;
	TL1 = 0x33;
	TH1 = 0xFE;
	counter++;
	if (counter == angle)
	{
		PWM = 0;
	}
	else if (counter == 40)
	{
		PWM = 1;
		counter = 0;
	}
}

void outzero()
{
	angle = 3;
	Delay1ms(1000);
}
void outon()
{
	output_flag = 1;
	Bee();	// 蜂鸣器响一声
	angle = 2;
	Delay1ms(1000);
	angle = 3;
	Delay1ms(1000);
}
void outoff()
{
	output_flag = 0;
	Bee();	// 蜂鸣器响两声
	angle = 4;
	Delay1ms(1000);
	angle = 3;
	Delay1ms(1000);
}
