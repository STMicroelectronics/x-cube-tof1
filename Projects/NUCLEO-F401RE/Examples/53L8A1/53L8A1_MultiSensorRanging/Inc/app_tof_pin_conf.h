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

#define VL53L8A1_PWR_EN_L_PIN   (GPIO_PIN_10)
#define VL53L8A1_PWR_EN_L_PORT  (GPIOB)

#define VL53L8A1_LPn_L_PIN   (GPIO_PIN_0)
#define VL53L8A1_LPn_L_PORT  (GPIOA)

#define VL53L8A1_PWR_EN_R_PIN   (GPIO_PIN_6)
#define VL53L8A1_PWR_EN_R_PORT  (GPIOA)

#define VL53L8A1_LPn_R_PIN   (GPIO_PIN_5)
#define VL53L8A1_LPn_R_PORT  (GPIOA)

#define VL53L8A1_PWR_EN_C_PIN   (GPIO_PIN_7)
#define VL53L8A1_PWR_EN_C_PORT  (GPIOA)

#define VL53L8A1_LPn_C_PIN   (GPIO_PIN_0)
#define VL53L8A1_LPn_C_PORT  (GPIOB)

#ifdef __cplusplus
}
#endif

#endif /* __APP_TOF_PIN_CONF_H__ */
