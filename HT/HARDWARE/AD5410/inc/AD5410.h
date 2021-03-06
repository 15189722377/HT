
#ifndef __AD5410_H__
#define __AD5410_H__

#include "stm32f10x.h"
#include "delay.h"

/* PB4是一个特殊的引脚，配置时需要GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE) */
#define AD5410_SLAVE_IN_PIN   GPIO_Pin_4    
#define AD5410_SLAVE_IN_PORT  GPIOB

#define AD5410_CLOCK_PIN      GPIO_Pin_5
#define AD5410_CLOCK_PORT     GPIOB

#define AD5410_LATCH_PIN      GPIO_Pin_6
#define AD5410_LATCH_PORT     GPIOB

#define AD5410_CLEAR_PIN      GPIO_Pin_7
#define AD5410_CLEAR_PORT     GPIOB

#define AD5410_SLAVE_OUT_PIN  GPIO_Pin_8
#define AD5410_SLAVE_OUT_PORT GPIOB

#define AD5410_FAULT_PIN      GPIO_Pin_9
#define AD5410_FAULT_PORT     GPIOB

#define AD5410_LATCH_H        GPIO_SetBits(AD5410_LATCH_PORT,AD5410_LATCH_PIN)
#define AD5410_LATCH_L        GPIO_ResetBits(AD5410_LATCH_PORT,AD5410_LATCH_PIN)

#define AD5410_CLOCK_H        GPIO_SetBits(AD5410_CLOCK_PORT,AD5410_CLOCK_PIN)
#define AD5410_CLOCK_L        GPIO_ResetBits(AD5410_CLOCK_PORT,AD5410_CLOCK_PIN)

#define AD5410_CLEAR_H        GPIO_SetBits(AD5410_CLEAR_PORT,AD5410_CLEAR_PIN)
#define AD5410_CLEAR_L        GPIO_ResetBits(AD5410_CLEAR_PORT,AD5410_CLEAR_PIN)

void AD5410_Init(void);
void AD5410_IOUT(float DATA);   //0<=DATA<=20

#endif
