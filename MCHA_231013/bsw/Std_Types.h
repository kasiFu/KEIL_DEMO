/**
*   @file    Std_Types.h
*   @version 1.0.1
*
*   @brief   AUTOSAR Base - Standard types definition.
*   @details AUTOSAR standard types header file. It contains all types that are used across several
*                modules of the basic software and that are platform and compiler independent
*
*   @addtogroup BASE_COMPONENT
*   @{
*/
/*==================================================================================================
*   Project              : AUTOSAR 4.0 MCAL
*   Platform             : PA
*   Peripheral           : XPC56XXL
*   Dependencies         : none
*
*   Autosar Version      : 4.0.3
*   Autosar Revision     : ASR_REL_4_0_REV_0003
*   Autosar Conf.Variant :
*   SW Version           : 1.0.1
*   Build Version        : SPC56XXL_MCAL_1_0_1_RTM_HF4_ASR_REL_4_0_REV_0003_20140515
*
*   (c) Copyright 2006-2014 Freescale Semiconductor Inc and STMicroelectronics
*   All Rights Reserved.
==================================================================================================*/
/*==================================================================================================
==================================================================================================*/
/**
* @file        Std_Types.h
* @implements  DBASE12000
*/

/**
* @page misra_violations MISRA-C:2004 violations
*
*
* @section Std_Types_h_REF_1
* Violates MISRA 2004 Required Rule 19.15, Precautions shall be taken in order to prevent the
* contents of a header being included twice.
* This comes from the order of includes in the .c file and from include dependencies. As a safe
* approach, any file must include all its dependencies. Header files are already protected against
* double inclusions.
*
* @section Std_Types_h_REF_2
* Violates MISRA 2004 Required Rule 5.1, Identifiers (internal and external) shall not rely on the
 * significance of more than 31 characters.
*/

/**
* @file         Std_Types.h
* @requirements   STD014
*/
#ifndef STD_TYPES_H
#define STD_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
/**
* @file        Std_Types.h
* @brief Include platform types
* @implements DBASE12001
*/
#include "Platform_Types.h"


/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/**
* @brief Parameters that shall be published within the standard types header file and also in the
*        module's description file
* @implements DBASE12012, DBASE12013, DBASE12014, DBASE12015, DBASE12016, DBASE12017, DBASE12018
*/
#define STD_TYPES_VENDOR_ID                     27
/*
* @violates @ref Std_Types_h_REF_2 This is required as per autosar Requirement, 
*/
#define STD_TYPES_AR_RELEASE_MAJOR_VERSION      4
/*
* @violates @ref Std_Types_h_REF_2 This is required as per autosar Requirement, 
*/
#define STD_TYPES_AR_RELEASE_MINOR_VERSION      0
/*
* @violates @ref Std_Types_h_REF_2 This is required as per autosar Requirement, 
*/
#define STD_TYPES_AR_RELEASE_REVISION_VERSION   3
#define STD_TYPES_SW_MAJOR_VERSION              1
#define STD_TYPES_SW_MINOR_VERSION              0
#define STD_TYPES_SW_PATCH_VERSION              1



/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/
/**
* @brief Physical state 5V or 3.3V
* @implements DBASE12006
*/
#define STD_HIGH    0x01

/**
* @brief Physical state 0V.
* @implements  DBASE12007
*/
#define STD_LOW     0x00

/**
* @brief Logical state active.
* @implements DBASE12008
*/
#define STD_ACTIVE  0x01

/**
* @brief Logical state idle.
* @implements DBASE12009
*/
#define STD_IDLE    0x00

/**
* @brief ON State.
* @implements DBASE12010
*/
#define STD_ON      0x01

/**
* @brief OFF state.
* @implements DBASE12011
*/
#define STD_OFF     0x00

/**
* @brief Return code for failure/error.
* @implements DBASE12005
*/
#define E_NOT_OK    0x01

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
* @brief Because E_OK is already defined within OSEK, the symbol E_OK has to be shared. To avoid
*        name clashes and redefinition problems, the symbols have to be defined in the following way
*       (approved within implementation).
*/
#ifndef STATUSTYPEDEFINED
    #define STATUSTYPEDEFINED
    /**
    * @brief Success return code
    * @implements DBASE12004
    */
    #define E_OK      0x00
    /**
    * @brief This type is defined for OSEK compliance.
    * @implements DBASE12001
    */
    typedef unsigned char StatusType;
#endif

/**
* @brief This type can be used as standard API return type which is shared between the RTE and the
*        BSW modules.
* @implements DBASE12002
*/
typedef uint8 Std_ReturnType;

/**
* @brief This type shall be used to request the version of a BSW module using the
*       "ModuleName"_GetVersionInfo() function.
* @implements DBASE12003
*/
typedef struct
{
    uint16  vendorID;               /**< @brief vendor ID */
    uint16  moduleID;               /**< @brief BSW module ID */
    uint8   sw_major_version;       /**< @brief BSW module software major version */
    uint8   sw_minor_version;       /**< @brief BSW module software minor version */
    uint8   sw_patch_version;       /**< @brief BSW module software patch version */
} Std_VersionInfoType;

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/

#if defined(__CC_ARM)
/*Supprerss warning message: extended constant initialiser used*/
#pragma diag_suppress 1296
#elif defined(__ICCARMM__)
#elit defined(__GNUC__)
#endif




#ifdef __cplusplus
}
#endif

#endif /* #ifndef STD_TYPES_H */

/** @} */
