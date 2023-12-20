	/**
  ******************************************************************************
  * @file    platform.c
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

#include "vl53l4cd_api.h"

/* when not customized by application define dummy one */
#ifndef VL53L4CD_GetI2cBus
/** This macro can be overloaded by user to enforce i2c sharing in RTOS context
 */
#define VL53L4CD_GetI2cBus(...) (void)0
#endif

#ifndef VL53L4CD_PutI2cBus
/** This macro can be overloaded by user to enforce i2c sharing in RTOS context
 */
#define VL53L4CD_PutI2cBus(...) (void)0
#endif

uint8_t _I2CBuffer[256];

/* Private functions prototypes */

static int _I2CWrite(Dev_t Dev, uint8_t *pdata, uint32_t count) {
	return Dev->IO.WriteReg(Dev->IO.Address, pdata, count);
}

static int _I2CRead(Dev_t Dev, uint8_t *pdata, uint32_t count) {
	return Dev->IO.ReadReg(Dev->IO.Address, pdata, count);
}

uint8_t VL53L4CD_WrByte(Dev_t Dev, uint16_t index, uint8_t data) {
    uint8_t Status = VL53L4CD_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    _I2CBuffer[2] = data;

    VL53L4CD_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 3);
    if (status_int != 0) {
        Status = status_int;
    }
    VL53L4CD_PutI2cBus();
    return Status;
}

uint8_t VL53L4CD_WrWord(Dev_t Dev, uint16_t index, uint16_t data) {
    uint8_t Status = VL53L4CD_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    _I2CBuffer[2] = data >> 8;
    _I2CBuffer[3] = data & 0x00FF;

    VL53L4CD_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 4);
    if (status_int != 0) {
        Status = status_int;
    }
    VL53L4CD_PutI2cBus();
    return Status;
}

uint8_t VL53L4CD_WrDWord(Dev_t Dev, uint16_t index, uint32_t data) {
    uint8_t Status = VL53L4CD_ERROR_NONE;
    int32_t status_int;
    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    _I2CBuffer[2] = (data >> 24) & 0xFF;
    _I2CBuffer[3] = (data >> 16) & 0xFF;
    _I2CBuffer[4] = (data >> 8)  & 0xFF;
    _I2CBuffer[5] = (data >> 0 ) & 0xFF;
    VL53L4CD_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 6);
    if (status_int != 0) {
        Status = status_int;
    }
    VL53L4CD_PutI2cBus();
    return Status;
}

uint8_t VL53L4CD_RdByte(Dev_t Dev, uint16_t index, uint8_t *data) {
    uint8_t Status = VL53L4CD_ERROR_NONE;
    int32_t status_int;

	_I2CBuffer[0] = index>>8;
	_I2CBuffer[1] = index&0xFF;
    VL53L4CD_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);
    if( status_int ){
        Status = status_int;
        goto done;
    }
    status_int = _I2CRead(Dev, data, 1);
    if (status_int != 0) {
        Status = status_int;
    }
done:
    VL53L4CD_PutI2cBus();
    return Status;
}

uint8_t VL53L4CD_RdWord(Dev_t Dev, uint16_t index, uint16_t *data) {
    uint8_t Status = VL53L4CD_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
	_I2CBuffer[1] = index&0xFF;
    VL53L4CD_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);

    if( status_int ){
        Status = status_int;
        goto done;
    }
    status_int = _I2CRead(Dev, _I2CBuffer, 2);
    if (status_int != 0) {
        Status = status_int;
        goto done;
    }

    *data = ((uint16_t)_I2CBuffer[0]<<8) + (uint16_t)_I2CBuffer[1];
done:
    VL53L4CD_PutI2cBus();
    return Status;
}

uint8_t VL53L4CD_RdDWord(Dev_t Dev, uint16_t index, uint32_t *data) {
    uint8_t Status = VL53L4CD_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
	_I2CBuffer[1] = index&0xFF;
    VL53L4CD_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);
    if (status_int != 0) {
        Status = status_int;
        goto done;
    }
    status_int = _I2CRead(Dev, _I2CBuffer, 4);
    if (status_int != 0) {
        Status = status_int;
        goto done;
    }

    *data = ((uint32_t)_I2CBuffer[0]<<24) + ((uint32_t)_I2CBuffer[1]<<16) + ((uint32_t)_I2CBuffer[2]<<8) + (uint32_t)_I2CBuffer[3];

done:
    VL53L4CD_PutI2cBus();
    return Status;
}

uint8_t WaitMs(
		Dev_t Dev,
		uint32_t TimeMs)
{
  uint32_t tickstart;
  tickstart = Dev->IO.GetTick();

  while ((Dev->IO.GetTick() - tickstart) < TimeMs);

  return 0;
}
