#include "app.h"
#include "spi.h"
#include "sensor.h"
#include "ad5410.h"
#include "tmp122.h"


void Configure_IWDG(void)
{
  RCC->CSR |= RCC_CSR_LSION; 
  while((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY) 
  { 
    /* add time out here for a robust application */
  }
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(1562);//10S
	
	IWDG_ReloadCounter();
	IWDG_Enable();	
}

u16 test=0;
int main()
{
	#define VECT_TAB_OFFSET  0x3000

	/* Set the Vector Table base address */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, VECT_TAB_OFFSET);
	
	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
  
 	delay_Init();
 	LTC2630ISC6_Init();
 	switch_GPIOInit();
	ADS1120_Init();
	AD5410_Init();
	TIM2_MeasureInit();
	TIM1_ModpollInit();
	TMP122_Init();
	
// 	Configure_IWDG();  //���ÿ��Ź�
	
 	RestoreModbusReg(); 
		
 	eMBInit(MB_RTU, comm_settings.modbusAddr, 0x02, comm_settings.modbusBaud, comm_settings.modbusParity);
  eMBEnable(); 
	
  while(1)
 { 	
	  FunctionPoll(); 
	 
    AD5410_IOUT(measure_values.sensorValue_mA);	 
	 
	  if(isMeasureFlg==1)
		{
// 			measure();
			__disable_irq() ;
			measure_values.temperatureValue=TMP122_CalTemp();
			__enable_irq() ;
		}	
	measure();
	}
}

