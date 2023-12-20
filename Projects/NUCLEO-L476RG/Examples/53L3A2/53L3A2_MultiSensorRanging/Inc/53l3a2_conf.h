/**
  ******************************************************************************
  * @file    53l3a2_conf.h
  * @author  IMG SW Application Team
  * @brief   This file contains definitions for the ToF components bus interfaces
  *          when using the X-NUCLEO-53L3A2 expansion board
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
#ifndef VL53L3A2_CONF_H
#define VL53L3A2_CONF_H

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_bus.h"
#include "stm32l4xx_nucleo_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/*
 * the 53L3A2 BSP driver uses this symbol to allocate a structure for each device
 * if you are only using the on-board sensor without break-out boards
 * change its to (1U) in order to save space in RAM memory
 */
#define RANGING_SENSOR_INSTANCES_NBR    (3U)

#define VL53L3A2_HI2C                   (hi2c1)

#define VL53L3A2_I2C_SCL_GPIO_PORT      BUS_I2C1_SCL_GPIO_PORT
#define VL53L3A2_I2C_SCL_GPIO_PIN       BUS_I2C1_SCL_GPIO_PIN
#define VL53L3A2_I2C_SDA_GPIO_PORT      BUS_I2C1_SDA_GPIO_PORT
#define VL53L3A2_I2C_SDA_GPIO_PIN       BUS_I2C1_SDA_GPIO_PIN

#define VL53L3A2_I2C_INIT               BSP_I2C1_Init
#define VL53L3A2_I2C_DEINIT             BSP_I2C1_DeInit
#define VL53L3A2_I2C_WRITEREG           BSP_I2C1_Send
#define VL53L3A2_I2C_READREG            BSP_I2C1_Recv
#define VL53L3A2_GETTICK                BSP_GetTick

#ifdef __cplusplus
}
#endif

#endif /* VL53L3A2_CONF_H */
