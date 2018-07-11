#include "AD5410.h"

void DA_SendByte(unsigned char byte);
void DA_control(unsigned char addr,unsigned int data) ;

void AD5410_GPIOInit(void)
{
	GPIO_InitTypeDef g;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	g.GPIO_Mode=GPIO_Mode_Out_PP;
	g.GPIO_Speed=GPIO_Speed_50MHz;
	
	g.GPIO_Pin=AD5410_CLEAR_PIN;   //5410_CLEAR
	GPIO_Init(AD5410_CLEAR_PORT,&g);
	
	g.GPIO_Pin=AD5410_LATCH_PIN;   //5410_LATCH
	GPIO_Init(AD5410_LATCH_PORT,&g);
	
	g.GPIO_Pin=AD5410_CLOCK_PIN;	 //5410_CLOCK
	GPIO_Init(AD5410_CLOCK_PORT,&g);
	
	g.GPIO_Pin=AD5410_SLAVE_IN_PIN;		//5410_SLAVE_IN
	GPIO_Init(AD5410_SLAVE_IN_PORT,&g);	
	
	g.GPIO_Mode=GPIO_Mode_IPU;
	g.GPIO_Pin=AD5410_FAULT_PIN;    //5410_FAULT
	GPIO_Init(AD5410_FAULT_PORT,&g);
	
	g.GPIO_Mode=GPIO_Mode_IPU;
	g.GPIO_Pin=AD5410_SLAVE_OUT_PIN;    //5410_SLAVE_OUT
	GPIO_Init(AD5410_SLAVE_OUT_PORT,&g);	
 } 
 
void SIMO_H(void)
{
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_SetBits(AD5410_SLAVE_IN_PORT,AD5410_SLAVE_IN_PIN);
}

void SIMO_L(void)
{
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_ResetBits(AD5410_SLAVE_IN_PORT,AD5410_SLAVE_IN_PIN);
} 

 /******************************/
 /*   ��AD5410��д��һ���ֽ�   */
 /******************************/
void DA_SendByte(unsigned char byte)  
{		
		
    unsigned char i;
    for ( i = 0; i < 8; i++)
    {
        AD5410_CLOCK_L; 
        if ( byte & 0x80 )
				{SIMO_H();
					delay_us(1);}
        else
				{SIMO_L();
					delay_us(1);}
        AD5410_CLOCK_H; 
        byte <<= 1;     
        AD5410_CLOCK_L;
    }
}
/***********************************/
/*   ��AD5410��д������������      */
/*   �ܳ�3���ֽ� addr�Ĵ�����ַ    */
/*            data����ָ��         */
void DA_control(unsigned char addr,unsigned int data)    
{
	u8 Hdata,Ldata;
	Hdata=data>>8;
	Ldata=data&255;
	AD5410_LATCH_L;
	DA_SendByte(addr);
	DA_SendByte(Hdata);
	DA_SendByte(Ldata);
	AD5410_LATCH_H;
	delay_us(2);
}
 
void LATCH(void)
{
	 AD5410_LATCH_L;
	 delay_us(5);
	 AD5410_LATCH_H;
	 delay_us(5);
	 AD5410_LATCH_L;
}
/**********************/
/*    ��ʼ��AD5410    */
void AD5410_Init(void)
{
	AD5410_GPIOInit();
	DA_control(0x56,0x0001);   //��λ  0x56��λ�Ĵ�����ַ   0x0001��λָ��
	AD5410_CLEAR_L;
	LATCH();
	DA_control(0x55,0x1016);   //0-20ma  257730����Ƶ��   1/16������С
	LATCH();
}
/********************/
/* IOUT  0<=DATA<=20*/
void AD5410_IOUT(float DATA)
{
	u16 I_OUT;
	if(DATA>=20)
	{
		I_OUT=4095;
	}
	else if(DATA<=4)
	{
		DATA=4;
		I_OUT=DATA*4096/20;
	}
	else
	{
		I_OUT=DATA*4096/20;
	}
	I_OUT=((int)I_OUT)<<4;
	DA_control(0x01,I_OUT);///��AD5410��д������������
	LATCH();
}
/************************************/
/*    ��AD5410��д��ض�ָ��        */
/*    addrΪ�ض��ļĴ���������      */ 
/*    0x00��ȡ״̬�Ĵ���            */ 
/*    0x01��ȡ���ݼĴ���            */
/*    0x02��ȡ���ƼĴ���            */  
// void DA_Read_Register(unsigned char addr)
// {
//   AD5410_LATCH_L; 
//   DA_SendByte(0x02);               //�ض��Ĵ���   
//   DA_SendByte(0x00);
//   DA_SendByte(addr);
//   AD5410_LATCH_H; 
// }

/***********************************************/
/*            ��Register�лض�����             */
/*���ض������ݴ洢��data_ReadFormRegister[x] ��*/
// void ReadDataFormRegister()    //���ض������ݴ洢��data_ReadFormRegister[x] ��
// {
// 	u8 temp;
// 	u8 i,j;
// 	//DA_CLK_HIGH;
// 	AD5410_LATCH_L;
// 	SIMO_L();
// 	delay_us(1);
// 	for(i=0; i<3; i++)
// 	{
// 		for(j=0; j<8; j++)
// 		{

// 			AD5410_CLOCK_L;

// 			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)==0)
// 			{
// 				temp=temp<<1;
// 			}
// 			else
// 			{
// 				temp=temp<<1;
// 				temp=temp+0x01;
// 			}
// 	    data_ReadFormRegister[i]=temp;
// 			temp=0;
// 			AD5410_CLOCK_H; 
// 		}
//   }
// 	SIMO_H();
// 	AD5410_LATCH_H;
// }
/************************************/
/*    ReadDatafromAD5410 Only       */
/*    addrΪ�ض��ļĴ���������      */  
/*    0x00��ȡ״̬�Ĵ���            */ 
/*    0x01��ȡ���ݼĴ���            */
/*    0x02��ȡ���ƼĴ���            */ 
// void ReadDataFromAD5410(unsigned char addr)
// {
// 	DA_Read_Register(addr);
// 	ReadDataFormRegister();
// }
