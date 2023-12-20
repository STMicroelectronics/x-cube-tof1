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

#include <vl53lx_platform.h>
#ifndef SMALL_FOOTPRINT
#include "vl53lx_platform_ipp.h"
#endif
#include <vl53lx_platform_log.h>
#include "vl53lx_api.h"

#include <time.h>
#include <math.h>

#ifdef VL53LX_LOG_ENABLE
#define trace_print(level, ...) VL53LX_trace_print_module_function(VL53LX_TRACE_MODULE_PLATFORM, level, VL53LX_TRACE_FUNCTION_NONE, ##__VA_ARGS__)
#define trace_i2c(...) VL53LX_trace_print_module_function(VL53LX_TRACE_MODULE_NONE, VL53LX_TRACE_LEVEL_NONE, VL53LX_TRACE_FUNCTION_I2C, ##__VA_ARGS__)
#endif

/* when not customized by application define dummy one */
#ifndef VL53LX_GetI2cBus
/** This macro can be overloaded by user to enforce i2c sharing in RTOS context
 */
#define VL53LX_GetI2cBus(...) (void)0
#endif

#ifndef VL53LX_PutI2cBus
/** This macro can be overloaded by user to enforce i2c sharing in RTOS context
 */
#define VL53LX_PutI2cBus(...) (void)0
#endif

uint8_t _I2CBuffer[256];
unsigned int i2creadCount = 0;
unsigned int i2cwriteCount = 0;
unsigned char SPI2C_Buffer[256];

/* Private functions prototypes */
static VL53LX_Error VL53LX_Delay(VL53LX_DEV Dev, uint32_t Delay);

int _I2CWrite(VL53LX_DEV Dev, uint8_t *pdata, uint32_t count) {
	return Dev->IO.WriteReg(Dev->IO.Address, pdata, count);
}

int _I2CRead(VL53LX_DEV Dev, uint8_t *pdata, uint32_t count) {
	return Dev->IO.ReadReg(Dev->IO.Address, pdata, count);
}

VL53LX_Error VL53LX_WriteMulti(VL53LX_DEV Dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    int status_int;
    VL53LX_Error Status = VL53LX_ERROR_NONE;
    if (count > sizeof(_I2CBuffer) - 1) {
        return VL53LX_ERROR_INVALID_PARAMS;
    }
    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    memcpy(&_I2CBuffer[2], pdata, count);
    VL53LX_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, count + 2);
    if (status_int != 0) {
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
    }
    VL53LX_PutI2cBus();
    return Status;
}

VL53LX_Error VL53LX_ReadMulti(VL53LX_DEV Dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    VL53LX_Error Status = VL53LX_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    VL53LX_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);
    if (status_int != 0) {
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
        goto done;
    }
    status_int = _I2CRead(Dev, pdata, count);
    if (status_int != 0) {
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
    }
done:
    VL53LX_PutI2cBus();
    return Status;
}

VL53LX_Error VL53LX_WrByte(VL53LX_DEV Dev, uint16_t index, uint8_t data) {
    VL53LX_Error Status = VL53LX_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    _I2CBuffer[2] = data;

    VL53LX_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 3);
    if (status_int != 0) {
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
    }
    VL53LX_PutI2cBus();
    return Status;
}

VL53LX_Error VL53LX_WrWord(VL53LX_DEV Dev, uint16_t index, uint16_t data) {
    VL53LX_Error Status = VL53LX_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    _I2CBuffer[2] = data >> 8;
    _I2CBuffer[3] = data & 0x00FF;

    VL53LX_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 4);
    if (status_int != 0) {
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
    }
    VL53LX_PutI2cBus();
    return Status;
}

VL53LX_Error VL53LX_WrDWord(VL53LX_DEV Dev, uint16_t index, uint32_t data) {
    VL53LX_Error Status = VL53LX_ERROR_NONE;
    int32_t status_int;
    _I2CBuffer[0] = index>>8;
    _I2CBuffer[1] = index&0xFF;
    _I2CBuffer[2] = (data >> 24) & 0xFF;
    _I2CBuffer[3] = (data >> 16) & 0xFF;
    _I2CBuffer[4] = (data >> 8)  & 0xFF;
    _I2CBuffer[5] = (data >> 0 ) & 0xFF;
    VL53LX_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 6);
    if (status_int != 0) {
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
    }
    VL53LX_PutI2cBus();
    return Status;
}

VL53LX_Error VL53LX_UpdateByte(VL53LX_DEV Dev, uint16_t index, uint8_t AndData, uint8_t OrData) {
    VL53LX_Error Status = VL53LX_ERROR_NONE;
    uint8_t data;

    Status = VL53LX_RdByte(Dev, index, &data);
    if (Status) {
        goto done;
    }
    data = (data & AndData) | OrData;
    Status = VL53LX_WrByte(Dev, index, data);
done:
    return Status;
}

VL53LX_Error VL53LX_RdByte(VL53LX_DEV Dev, uint16_t index, uint8_t *data) {
    VL53LX_Error Status = VL53LX_ERROR_NONE;
    int32_t status_int;

	_I2CBuffer[0] = index>>8;
	_I2CBuffer[1] = index&0xFF;
    VL53LX_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);
    if( status_int ){
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
        goto done;
    }
    status_int = _I2CRead(Dev, data, 1);
    if (status_int != 0) {
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
    }
done:
    VL53LX_PutI2cBus();
    return Status;
}

VL53LX_Error VL53LX_RdWord(VL53LX_DEV Dev, uint16_t index, uint16_t *data) {
    VL53LX_Error Status = VL53LX_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
	_I2CBuffer[1] = index&0xFF;
    VL53LX_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);

    if( status_int ){
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
        goto done;
    }
    status_int = _I2CRead(Dev, _I2CBuffer, 2);
    if (status_int != 0) {
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
        goto done;
    }

    *data = ((uint16_t)_I2CBuffer[0]<<8) + (uint16_t)_I2CBuffer[1];
done:
    VL53LX_PutI2cBus();
    return Status;
}

VL53LX_Error VL53LX_RdDWord(VL53LX_DEV Dev, uint16_t index, uint32_t *data) {
    VL53LX_Error Status = VL53LX_ERROR_NONE;
    int32_t status_int;

    _I2CBuffer[0] = index>>8;
	_I2CBuffer[1] = index&0xFF;
    VL53LX_GetI2cBus();
    status_int = _I2CWrite(Dev, _I2CBuffer, 2);
    if (status_int != 0) {
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
        goto done;
    }
    status_int = _I2CRead(Dev, _I2CBuffer, 4);
    if (status_int != 0) {
        Status = VL53LX_ERROR_CONTROL_INTERFACE;
        goto done;
    }

    *data = ((uint32_t)_I2CBuffer[0]<<24) + ((uint32_t)_I2CBuffer[1]<<16) + ((uint32_t)_I2CBuffer[2]<<8) + (uint32_t)_I2CBuffer[3];

done:
    VL53LX_PutI2cBus();
    return Status;
}

VL53LX_Error VL53LX_GetTickCount(VL53LX_DEV Dev, uint32_t *ptick_count_ms)
{

    /* Returns current tick count in [ms] */

	VL53LX_Error status  = VL53LX_ERROR_NONE;

	*ptick_count_ms = Dev->IO.GetTick();
	
#ifdef VL53LX_LOG_ENABLE
	trace_print(
		VL53LX_TRACE_LEVEL_DEBUG,
		"VL53LX_GetTickCount() = %5u ms;\n",
	*ptick_count_ms);
#endif

	return status;
}

#define trace_print(level, ...) \
	_LOG_TRACE_PRINT(VL53LX_TRACE_MODULE_PLATFORM, \
	level, VL53LX_TRACE_FUNCTION_NONE, ##__VA_ARGS__)

#define trace_i2c(...) \
	_LOG_TRACE_PRINT(VL53LX_TRACE_MODULE_NONE, \
	VL53LX_TRACE_LEVEL_NONE, VL53LX_TRACE_FUNCTION_I2C, ##__VA_ARGS__)

VL53LX_Error VL53LX_GetTimerFrequency(int32_t *ptimer_freq_hz)
{
	*ptimer_freq_hz = 0;
	
	trace_print(VL53LX_TRACE_LEVEL_INFO, "VL53LX_GetTimerFrequency: Freq : %dHz\n", *ptimer_freq_hz);
	return VL53LX_ERROR_NONE;
}

VL53LX_Error VL53LX_WaitMs(VL53LX_DEV Dev, int32_t wait_ms){
	VL53LX_Delay(Dev, wait_ms);
    return VL53LX_ERROR_NONE;
}

VL53LX_Error VL53LX_WaitUs(VL53LX_DEV Dev, int32_t wait_us){
	VL53LX_Delay(Dev, wait_us/1000);
    return VL53LX_ERROR_NONE;
}

VL53LX_Error VL53LX_WaitValueMaskEx(
	VL53LX_DEV    Dev,
	uint32_t      timeout_ms,
	uint16_t      index,
	uint8_t       value,
	uint8_t       mask,
	uint32_t      poll_delay_ms)
{

	/*
	 * Platform implementation of WaitValueMaskEx V2WReg script command
	 *
	 * WaitValueMaskEx(
	 *          duration_ms,
	 *          index,
	 *          value,
	 *          mask,
	 *          poll_delay_ms);
	 */

	VL53LX_Error status         = VL53LX_ERROR_NONE;
	uint32_t     start_time_ms = 0;
	uint32_t     current_time_ms = 0;
	uint32_t     polling_time_ms = 0;
	uint8_t      byte_value      = 0;
	uint8_t      found           = 0;
#ifdef VL53LX_LOG_ENABLE
	uint8_t      trace_functions = VL53LX_TRACE_FUNCTION_NONE;
#endif

	char   register_name[VL53LX_MAX_STRING_LENGTH];

    /* look up register name */
#ifdef PAL_EXTENDED
	VL53LX_get_register_name(
			index,
			register_name);
#else
	VL53LX_COPYSTRING(register_name, "");
#endif

	/* Output to I2C logger for FMT/DFT  */

    /*trace_i2c("WaitValueMaskEx(%5d, 0x%04X, 0x%02X, 0x%02X, %5d);\n",
    		     timeout_ms, index, value, mask, poll_delay_ms); */
    trace_i2c("WaitValueMaskEx(%5d, %s, 0x%02X, 0x%02X, %5d);\n",
    		     timeout_ms, register_name, value, mask, poll_delay_ms);

	/* calculate time limit in absolute time */

	 VL53LX_GetTickCount(Dev, &start_time_ms);

	/* remember current trace functions and temporarily disable
	 * function logging
	 */

#ifdef VL53LX_LOG_ENABLE
	trace_functions = VL53LX_get_trace_functions();
	VL53LX_set_trace_functions(VL53LX_TRACE_FUNCTION_NONE);
#endif

	/* wait until value is found, timeout reached on error occurred */

	while ((status == VL53LX_ERROR_NONE) &&
		   (polling_time_ms < timeout_ms) &&
		   (found == 0)) {

		if (status == VL53LX_ERROR_NONE)
			status = VL53LX_RdByte(
							Dev,
							index,
							&byte_value);

		if ((byte_value & mask) == value)
			found = 1;

		if (status == VL53LX_ERROR_NONE  &&
			found == 0 &&
			poll_delay_ms > 0)
			status = VL53LX_WaitMs(
					Dev,
					poll_delay_ms);

		/* Update polling time (Compare difference rather than absolute to
		negate 32bit wrap around issue) */
		VL53LX_GetTickCount(Dev, &current_time_ms);
		polling_time_ms = current_time_ms - start_time_ms;

	}

#ifdef VL53LX_LOG_ENABLE
	/* Restore function logging */
	VL53LX_set_trace_functions(trace_functions);
#endif

	if (found == 0 && status == VL53LX_ERROR_NONE)
		status = VL53LX_ERROR_TIME_OUT;

	return status;
}

/**
  * @brief This function provides accurate delay (in milliseconds)
  * @param Dev   pointer to component object
  * @param Delay  specifies the delay time length, in milliseconds
  * @retval VL53LX_Error
  */
static VL53LX_Error VL53LX_Delay(VL53LX_DEV Dev, uint32_t Delay)
{
  uint32_t tickstart;
  tickstart = Dev->IO.GetTick();

  while ((Dev->IO.GetTick() - tickstart) < Delay)
  {
  }

  return VL53LX_ERROR_NONE;
}
