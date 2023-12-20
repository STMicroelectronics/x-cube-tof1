/**
  ******************************************************************************
  * @file    53l1a2.h
  * @author  IMG SW Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the 53l1a2.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef XNUCLEO_53L1A2_H
#define XNUCLEO_53L1A2_H

/* Includes ------------------------------------------------------------------*/
#include "53l1a2_conf.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup XNUCLEO_53L1A2 X-NUCLEO-53L1A2
  * @{
  */

/** @defgroup XNUCLEO_53L1A2_COMMON X-NUCLEO-53L1A2 COMMON
  * @{
  */

/* XNUCLEO-53L1A2 Device selector */
enum VL53L1A2_dev_e
{
  VL53L1A2_DEV_LEFT    =  0,   /* !< left satellite device P21 header */
  VL53L1A2_DEV_CENTER  =  1,   /* !< center (built-in) device */
  VL53L1A2_DEV_RIGHT   =  2    /* !< Right satellite device P22 header */
};

/** @defgroup XNUCLEO_53L1A2_COMMON_Exported_Functions Exported Functions
  * @{
  */
int32_t VL53L1A2_Init(void);
int32_t VL53L1A2_DeInit(void);
int32_t VL53L1A2_ResetId(uint8_t DevNo,  uint8_t state);
int32_t VL53L1A2_SetDisplayString(const char *str);
/** 
 * @} 
 */

/**
  * @}
  */

/**
  * @}
  */

 /**
  * @}
  */

#endif /* XNUCLEO_53L1A2_H */
