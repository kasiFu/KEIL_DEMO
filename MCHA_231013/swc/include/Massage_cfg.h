/*****************************************************************************
*
*   All Contents@CopyRight Of Yihang Automotive Parts Wuxi Co.,Ltd.
*
*   All rights reserved.
*
*   Filename:    Massage_cfg.h
*
*   Description: Header file for Massage_cfg.h
*
*   Revision History:
*
*   Date            Author          Description
*   ----------      ----------      -----------------------------------------
*   2023-09-22      Armand.zhao     Initial created
*
*****************************************************************************/
#ifndef MASSAGE_CFG_H
#define MASSAGE_CFG_H 1
#include "Std_Types.h"
#include "BAT32A237.h"

#define MASSAGE_GRAND_LOW		150u
#define MASSAGE_GRAND_MIDDLE	250u
#define MASSAGE_GRAND_HIGH		350u


typedef enum 
{
	CFG_NOINIT = 10,
	CFG_WAVE = 1,
	CFG_PULSE = 2,
	CFG_XXX = 3
}Massage_cfg_modetype_t;

typedef struct
{
	uint16 Grand;
	uint8 startbag;
	uint8 Endbag;
} Massage_cfg_data_type;

typedef struct 
{
	Massage_cfg_data_type mode[5][3];
}Massage_cfg_handle_type;

extern const Massage_cfg_handle_type Massage_cfg_handle ;

#endif
