/**
  ******************************************************************************
  * @file    tmp122.c
  * @author  lx
  * @version V1.0
  * @date    2018/7/10
  * @brief   
  ******************************************************************************
  * @attention
  * �¶ȼĴ�������ֵΪ16λ�����и�13λΪ�¶ȣ���3λӦȥ����
  * ��13λ�¶�ֵ�У����λ��ʾ���ţ�0Ϊ����1Ϊ����������8λ��ʾ���������4λ��ʾС����
	* 
  ******************************************************************************
  */ 
#include "tmp122.h"
#include "delay.h"

void TMP122_GPIOInit(void)
{
	GPIO_InitTypeDef g;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//RCC_APB2Periph_AFIO ?
	
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;	
	g.GPIO_Pin=TMP122_CS_PIN | TMP122_SCK_PIN;   
	GPIO_Init(GPIOA,&g);
	
	g.GPIO_Mode=GPIO_Mode_IPU;
// 	g.GPIO_Speed=GPIO_Speed_50MHz;	
	g.GPIO_Pin=TMP122_SOI_PIN;   
	GPIO_Init(TMP122_SOI_PORT,&g);
	
	g.GPIO_Mode=GPIO_Mode_IPU;
	g.GPIO_Pin=ALERT_PIN;   
	GPIO_Init(ALERT_PORT,&g);
}

void TMP122_Init(void)
{
	TMP122_GPIOInit();
	TMP122_CS_1;
	TMP122_SCK_1;
}

u16 TMP122_Output()
{
	u16 outdata=0;
	u8 n=0;
	TMP122_CS_1;
	TMP122_SCK_1;
	TMP122_CS_0;
// 	delay_us(1);
	
	for(n=0;n<16;n++)
	{
		outdata<<=1;
		TMP122_SCK_0;
// 		delay_us(1);
		outdata+=READ_TMP122_SOI;
		delay_us(1);
		TMP122_SCK_1;
		delay_us(1);
	}
	TMP122_CS_1;
	TMP122_SCK_1;
	if(outdata<0x8000)
		return (outdata>>3);
	else
	{
	  /* �������룬��λȡ���ټ�1 */
		outdata=0xFFFF-outdata;
		outdata>>=3;
		outdata+=1;
		return (0-outdata);
	}
}
float TMP122_CalTemp(void)
{
	u16 tempint=0;
	u16 tempdec=0;
	float tempfloat=0.0;
	u16 outData;
	
	outData=TMP122_Output();
	tempint=outData>>4;
	tempdec=(outData&0x08)/2+(outData&0x04)/4+(outData&0x02)/8+(outData&0x01)/16;
	tempfloat=tempint+tempdec;
	
	return tempfloat;
}
