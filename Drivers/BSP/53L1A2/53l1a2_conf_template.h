/**
  ******************************************************************************
  * @file    53l1a2_conf_template.h
  * @author  IMG SW Application Team
  * @brief   This file contains definitions for the ToF components bus interfaces
  *          when using the X-NUCLEO-53L1A2 expansion board
  *          This file should be copied to the application folder and renamed
  *          to 53l1a2_conf.h.
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

/* Replace the header file names with the ones of the target platform */
#include "stm32yyxx_hal.h"
#include "nucleo_xyyyzz_bus.h"
#include "nucleo_xyyyzz_errno.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __53L1A2_CONF_H__
#define __53L1A2_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
#define RANGING_SENSOR_INSTANCES_NBR    (3U)

#define VL53L1A2_HI2C                   (hi2c1)

#define VL53L1A2_I2C_SCL_GPIO_PORT      BUS_I2C1_SCL_GPIO_PORT
#define VL53L1A2_I2C_SCL_GPIO_PIN       BUS_I2C1_SCL_GPIO_PIN
#define VL53L1A2_I2C_SDA_GPIO_PORT      BUS_I2C1_SDA_GPIO_PORT
#define VL53L1A2_I2C_SDA_GPIO_PIN       BUS_I2C1_SDA_GPIO_PIN

#define VL53L1A2_I2C_INIT               BSP_I2C1_Init
#define VL53L1A2_I2C_DEINIT             BSP_I2C1_DeInit
#define VL53L1A2_I2C_WRITEREG           BSP_I2C1_Send
#define VL53L1A2_I2C_READREG            BSP_I2C1_Recv
#define VL53L1A2_GETTICK                BSP_GetTick

#ifdef __cplusplus
}
#endif

#endif /* __53L1A2_CONF_H__*/

