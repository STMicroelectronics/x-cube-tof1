/**
  ******************************************************************************
  * @file    platform.h
  * @author  IMG SW Application Team
  * @brief   This file contains all the platform functions prototypes
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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#pragma once

#include "vl53l4cd.h"

typedef VL53L4CD_Object_t* Dev_t;

/**
 * @brief Mandatory function used to read one single byte.
 * @param Dev_t : Pointer of VL53L4CD Object
 * structure.
 * @param (uint16_t) Address : I2C location of value to read.
 * @param (uint8_t) *p_values : Pointer of value to read.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L4CD_RdByte(
		Dev_t dev,
		uint16_t RegisterAdress,
		uint8_t *p_value);

/**
 * @brief Mandatory function used to write one single byte.
 * @param Dev_t : Pointer of VL53L4CD Object
 * structure.
 * @param (uint16_t) Address : I2C location of value to read.
 * @param (uint8_t) value : Pointer of value to write.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L4CD_WrByte(
		Dev_t dev,
		uint16_t RegisterAdress,
		uint8_t value);

/**
 * @brief Mandatory function used to read word.
 * @param Dev_t : Pointer of VL53L4CD Object
 * structure.
 * @param (uint16_t) Address : I2C location of value to read.
 * @param (uint16_t) *p_values : Pointer of value to read.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L4CD_RdWord(
		Dev_t dev,
		uint16_t RegisterAdress,
		uint16_t *p_value);

/**
 * @brief Mandatory function used to write word.
 * @param Dev_t : Pointer of VL53L4CD Object
 * structure.
 * @param (uint16_t) Address : I2C location of value to read.
 * @param (uint16_t) value : Pointer of value to write.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L4CD_WrWord(
		Dev_t dev,
		uint16_t RegisterAdress,
		uint16_t value);

/**
 * @brief Mandatory function used to read double word.
 * @param Dev_t : Pointer of VL53L4CD Object
 * structure.
 * @param (uint16_t) Address : I2C location of value to read.
 * @param (uint32_t) *p_values : Pointer of value to read.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L4CD_RdDWord(
		Dev_t dev,
		uint16_t RegisterAdress,
		uint32_t *p_value);

/**
 * @brief Mandatory function used to write double word.
 * @param Dev_t : Pointer of VL53L4CD Object
 * structure.
 * @param (uint16_t) Address : I2C location of value to read.
 * @param (uint32_t) value : Pointer of value to write.
 * @return (uint8_t) status : 0 if OK
 */

uint8_t VL53L4CD_WrDWord(
		Dev_t dev,
		uint16_t RegisterAdress,
		uint32_t value);


/**
 * @brief Mandatory function, used to wait during an amount of time. It must be
 * filled as it's used into the API.
 * @param Dev_t : Pointer of VL53L4CD Object
 * structure.
 * @param (uint32_t) TimeMs : Time to wait in ms.
 * @return (uint8_t) status : 0 if wait is finished.
 */

uint8_t WaitMs(
		Dev_t dev,
		uint32_t TimeMs);

#endif	// _PLATFORM_H_
