/***********************************************************************************************************************
*
*   All Contents@CopyRight Of Yihang Automotive Parts Wuxi Co.,Ltd.
*
*   All rights reserved.
*
*   Filename:    Mcu.h
*
*   Description: Source Code File for Real Time Environment
*
*   Revision History:
*
*   Date            Author          Description
*   ----------      ----------      -----------------------------------------
*   2023-09-14      Armand.zhao     Initial created
*
***********************************************************************************************************************/

#include "Mcu.h"
#include "adc.h"
#include "gpio.h"
#include "cgc.h"
#include "delay.h"



/*****************************************************************************
Description : 系统时钟初始化
Input       : 无
Return      : 无
Version     : 0.0.1 
History     : 创建
Date        : 2023.9.15
Others      : 无
*****************************************************************************/
static void Mcu_SystemClock_Init(void)
{
	uint32 msCnt; // count value of 1ms
	OSC_Pin_Mode_t main = OSC_PORT;
	OSC_Speed_Mode_t amph = OSC_UNDER_10M;
	OSC_Pin_Mode_t sub = OSC_PORT;
	OSC_Power_Mode_t amphs = OSC_NORMAL_POWER;

	SystemCoreClockUpdate();

	CGC_Osc_Setting(main, amph, sub, amphs);

	msCnt = SystemCoreClock / 1997;
	SysTick_Config(msCnt);
	delay_init(SystemCoreClock);
}

static void Mcu_PortOutput_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;


	// PORT_Init(PORT0, PIN0, OUTPUT);	 /*5Massage L 3*/
	// PORT_Init(PORT0, PIN1, OUTPUT);	 /*3Massage L 2*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_DIG;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Level = GPIO_Level_LOW;
	GPIO_Init(GPIO_PORT0, &GPIO_InitStruct);

	// PORT_Init(PORT1, PIN0, OUTPUT);	 /*9Massage L 5*/
	// PORT_Init(PORT1, PIN2, OUTPUT);  /*VCC_IO_COR 9V out control out*/
	// PORT_Init(PORT1, PIN3, OUTPUT);	 /*6Massage r 3*/
	// PORT_Init(PORT1, PIN4, OUTPUT);	 /*IO_WakeUp_Config */
	// PORT_Init(PORT1, PIN5, OUTPUT);	 /*IO_SALV_VCC solenoid power*/
	// PORT_Init(PORT1, PIN6, OUTPUT);	 /*8Massage r 4*/
	// PORT_Init(PORT1, PIN7, OUTPUT);	 /*10Massage r 5*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_DIG;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Level = GPIO_Level_LOW;
	GPIO_Init(GPIO_PORT1, &GPIO_InitStruct);

	// PORT_Init(PORT2, PIN0, OUTPUT);	 /*1Massage L 1*/
	// PORT_Init(PORT2, PIN1, OUTPUT);	 /*2Massage r 1*/
	// PORT_Init(PORT2, PIN2, OUTPUT);	 /*4Massage r 2*/
	// PORT_Init(PORT2, PIN3, OUTPUT); /*Lumber d 3*/
	// PORT_Init(PORT2, PIN4, OUTPUT); /*Lumber d 2*/
	// PORT_Init(PORT2, PIN5, OUTPUT); /*lumber D 1*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_DIG;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Level = GPIO_Level_LOW;
	GPIO_Init(GPIO_PORT2, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; /*Lumber U 3*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_DIG;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Level = GPIO_Level_LOW;
	GPIO_Init(GPIO_PORT3, &GPIO_InitStruct);

	// PORT_Init(PORT6, PIN0, OUTPUT); /*test point 12*/
	// PORT_Init(PORT6, PIN1, OUTPUT); /*test point 11*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_DIG;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Level = GPIO_Level_LOW;
	GPIO_Init(GPIO_PORT6, &GPIO_InitStruct);

	// PORT_Init(PORT7, PIN0, OUTPUT); /*Lumber U 1*/
	// PORT_Init(PORT7, PIN2, OUTPUT); /*Lumber U 2*/
	// PORT_Init(PORT7, PIN3, OUTPUT); /*pump ctrl out*/
	// PORT_Init(PORT7, PIN4, OUTPUT); /*IO_YFP_Enable motor current monitor enable Low */
	// PORT_Init(PORT7, PIN5, OUTPUT); /*IO_MOTO_VCC Motor power*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_DIG;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Level = GPIO_Level_LOW;
	GPIO_Init(GPIO_PORT7, &GPIO_InitStruct);

	// PORT_Init(PORT14, PIN0, OUTPUT); /*7Massage L 4*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_DIG;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Level = GPIO_Level_LOW;
	GPIO_Init(GPIO_PORT14, &GPIO_InitStruct);
}

static void Mcu_PortInput_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	  /*BAT_DEC_AD*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_ANA;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT1, &GPIO_InitStruct);

	 /*MOTO_Cir_AD*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_ANA;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT12, &GPIO_InitStruct);

 /*KEY_IO_AD*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_ANA;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT14, &GPIO_InitStruct);
////////////////////////////////////////////////////////////////////

	// PORT_Init(PORT12, PIN1, PULLUP_INPUT); /*UP_IO_AD  */
	// PORT_Init(PORT12, PIN2, PULLUP_INPUT); /*DOWN_IO_AD  */
	// PORT_Init(PORT12, PIN3, PULLUP_INPUT); /*FRONT_IO_AD */
	// PORT_Init(PORT12, PIN4, PULLUP_INPUT); /*REAR_IO_AD */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_DIG;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT12, &GPIO_InitStruct);

/*LIN RX*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; /*LIN RX*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_DIG;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT5, &GPIO_InitStruct);

/*WakeUp_Signe*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Ctrl = GPIO_Control_DIG;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_PORT13, &GPIO_InitStruct);
}
/*****************************************************************************
Description : 端口配置初始化
Input       : 无
Return      : 无
Version     : 0.0.1 
History     : 创建 
Date        : 
Others      : 无
*****************************************************************************/
static void Mcu_PORTConfig_Init(void)
{
	Mcu_PortOutput_Init();

	Mcu_PortInput_Init();
}

void MCU_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;

	ADC_InitStruct.ADC_Mode = ADC_Mode_Select;
	ADC_InitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_InitStruct.ADC_RefVoltage = ADC_Ref_Vdd;
	ADC_InitStruct.ADC_ConvSpeed = ADC_ConvSpeed_High;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrig_Software;
	ADC_InitStruct.ADC_UpLimit = 0XFF;
	ADC_InitStruct.ADC_LowLimit = 0x00;
	// ADC_InitStruct.ADC_ScanConf = ADC_ChannelSign;
	//ADC_InitStruct.ADC_TwoSamplingDelay = 55;
	ADC_InitStruct.ADC_ContinuousConvMode = ADC_Conv_Oneshot;

	Mcu_SystemClock_Init();

	Mcu_PORTConfig_Init();

	ADC_Init(&ADC_InitStruct);
}

/*****************************************************************************
Description : 中断优先级配置
Input       : 无
Return      : 无
Version     : 0.0.1 
History     : 创建
Date        : 2022.3.1
Others      : 无
*****************************************************************************/
void MCU_Interrupt_Config(void)
{
    NVIC_SetPriority(SysTick_IRQn, 3);
      
}



