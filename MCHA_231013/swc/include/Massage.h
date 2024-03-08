/*****************************************************************************
*
*   All Contents@CopyRight Of Yihang Automotive Parts Wuxi Co.,Ltd.
*
*   All rights reserved.
*
*   Filename:    MassageM.h
*
*   Description: Header file for MassageM.h
*
*   Revision History:
*
*   Date            Author          Description
*   ----------      ----------      -----------------------------------------
*   2023-09-22      Armand.zhao     Initial created
*
*****************************************************************************/

#ifndef MASSAGE_H
#define MASSAGE_H 1
#include "Std_Types.h"
#include "BAT32A237.h"
#include "gpio.h"

#define MASSAGE_BAG_FLATE    1
#define MASSAGE_BAG_DEFLATE  2
/*reference actual bag number*/
#define Massage_bag1On() GPIO_SetBits(GPIO_PORT2, GPIO_Pin_0)
#define Massage_bag2On() GPIO_SetBits(GPIO_PORT2, GPIO_Pin_1)
#define Massage_bag3On() GPIO_SetBits(GPIO_PORT0, GPIO_Pin_1)
#define Massage_bag4On() GPIO_SetBits(GPIO_PORT2, GPIO_Pin_2)
#define Massage_bag5On() GPIO_SetBits(GPIO_PORT0, GPIO_Pin_0)
#define Massage_bag6On() GPIO_SetBits(GPIO_PORT1, GPIO_Pin_3)
#define Massage_bag7On() GPIO_SetBits(GPIO_PORT14, GPIO_Pin_0)
#define Massage_bag8On() GPIO_SetBits(GPIO_PORT1, GPIO_Pin_6)
#define Massage_bag9On() GPIO_SetBits(GPIO_PORT1, GPIO_Pin_0)
#define Massage_bag10On() GPIO_SetBits(GPIO_PORT1, GPIO_Pin_7)

#define Massage_bag1Off() GPIO_ResetBits(GPIO_PORT2, GPIO_Pin_0)
#define Massage_bag2Off() GPIO_ResetBits(GPIO_PORT2, GPIO_Pin_1)
#define Massage_bag3Off() GPIO_ResetBits(GPIO_PORT0, GPIO_Pin_1)
#define Massage_bag4Off() GPIO_ResetBits(GPIO_PORT2, GPIO_Pin_2)
#define Massage_bag5Off() GPIO_ResetBits(GPIO_PORT0, GPIO_Pin_0)
#define Massage_bag6Off() GPIO_ResetBits(GPIO_PORT1, GPIO_Pin_3)
#define Massage_bag7Off() GPIO_ResetBits(GPIO_PORT14, GPIO_Pin_0)
#define Massage_bag8Off() GPIO_ResetBits(GPIO_PORT1, GPIO_Pin_6)
#define Massage_bag9Off() GPIO_ResetBits(GPIO_PORT1, GPIO_Pin_0)
#define Massage_bag10Off() GPIO_ResetBits(GPIO_PORT1, GPIO_Pin_7)

typedef enum
{
    MODE1 = 0,
    MODE2 = 1,
    MODE3 = 2,
    MODE4 = 3,
    MODE5 = 4,
    
    MODE_END = 5
} Massage_mode_enum_t;

typedef enum
{
    SUB_WAVE_MODE = 0,
    SUB_PULSE_MODE = 1,
    SUB_STRETCH_MODE = 2,

    SUB_END
}Massage_subMode_enum_t;

typedef void(*p_func_type)(uint16,Massage_mode_enum_t,uint8);
typedef void(*p_funcMode_type)(Massage_mode_enum_t);
typedef void(*p_funcIO_type)(uint8);
typedef struct
{
    Massage_mode_enum_t mode; /*mode number*/
    uint8 index;              /*start bag running*/
    uint8 repeatNum;          /*basic massage cycyles*/
    uint8 repeatFlag;         /*basic massage 3cycles end flag*/

    uint16 level; /*reflate time*/

} Massage_Handle_type;

typedef struct 
{
    Massage_subMode_enum_t subMode[3]; /*3，4，5 3个模式各运行进度*/
    p_func_type subModeFunc[3];
}Massage_multiMode_type;





void Massage_task10ms(void);

#endif
