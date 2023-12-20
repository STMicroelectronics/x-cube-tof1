/**
  ******************************************************************************
  * @file    app_tof_pin_conf.h
  * @author  IMG SW Application Team
  * @brief   This file contains definitions for TOF pins
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_TOF_PIN_CONF_H__
#define __APP_TOF_PIN_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported symbols ----------------------------------------------------------*/

#define VL53L4A2_XSHUT_C_PIN   (GPIO_PIN_3)
#define VL53L4A2_XSHUT_C_PORT  (GPIOB)
#define VL53L4A2_XSHUT_L_PIN   (GPIO_PIN_10)
#define VL53L4A2_XSHUT_L_PORT  (GPIOB)
#define VL53L4A2_XSHUT_R_PIN   (GPIO_PIN_5)
#define VL53L4A2_XSHUT_R_PORT  (GPIOB)

#ifdef __cplusplus
}
#endif

#endif /* __APP_TOF_PIN_CONF_H__ */
