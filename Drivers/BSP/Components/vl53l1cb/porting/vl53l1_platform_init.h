/**
 ******************************************************************************
 * Copyright (c) 2020, STMicroelectronics - All Rights Reserved
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */



#ifndef _VL53L1_PLATFORM_INIT_H_
#define _VL53L1_PLATFORM_INIT_H_

#include "vl53l1_platform.h"

#ifdef __cplusplus
extern "C"
{
#endif






VL53L1_Error VL53L1_platform_init(
	VL53L1_Dev_t *pdev,
	uint8_t       i2c_slave_address,
	uint8_t       comms_type,
	uint16_t      comms_speed_khz);




VL53L1_Error VL53L1_platform_terminate(
	VL53L1_Dev_t *pdev);


#ifdef __cplusplus
}
#endif

#endif


