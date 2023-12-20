/**
  ******************************************************************************
  * @file    vl53l4cx.h
  * @author  IMG SW Application Team
  * @brief   This file contains all the functions prototypes for the vl53l4cx.c
  *          driver.
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
#ifndef VL53L4CX_H
#define VL53L4CX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <math.h>
#include "vl53lx_def.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup VL53L4CX
  * @{
  */

/** @defgroup VL53L4CX_Exported_Constants Exported Constants
  * @{
  */
#define VL53L4CX_ID                 (0xEBAAU)
#define VL53L4CX_ID_REG             (0x010FU)

#define VL53L4CX_DEVICE_ADDRESS     (0x52U)
#define VL53L4CX_NB_TARGET_PER_ZONE (4U)
#define VL53L4CX_MAX_NB_ZONES       (1U)

/* vl53l4cx ranging profiles */
#define VL53L4CX_PROFILE_SHORT      (VL53LX_DISTANCEMODE_SHORT)
#define VL53L4CX_PROFILE_MEDIUM     (VL53LX_DISTANCEMODE_MEDIUM)
#define VL53L4CX_PROFILE_LONG       (VL53LX_DISTANCEMODE_LONG)

/* vl53l4cx ranging modes */
#define VL53L4CX_MODE_BLOCKING_CONTINUOUS (1U)
#define VL53L4CX_MODE_BLOCKING_ONESHOT    (2U)
#define VL53L4CX_MODE_ASYNC_CONTINUOUS    (3U)
#define VL53L4CX_MODE_ASYNC_ONESHOT       (4U)

/* vl53l4cx error codes */
#define VL53L4CX_OK                 (0)
#define VL53L4CX_ERROR              (-1)
#define VL53L4CX_INVALID_PARAM      (-2)
#define VL53L4CX_TIMEOUT            (-3)
#define VL53L4CX_NOT_IMPLEMENTED    (-4)
/**
  * @}
  */

/** @defgroup VL53L4CX_Exported_Types Exported Types
  * @{
  */
typedef int32_t (*VL53L4CX_Init_Func)(void);
typedef int32_t (*VL53L4CX_DeInit_Func)(void);
typedef int32_t (*VL53L4CX_GetTick_Func)(void);
typedef int32_t (*VL53L4CX_WriteReg_Func)(uint16_t, uint8_t *, uint16_t);
typedef int32_t (*VL53L4CX_ReadReg_Func)(uint16_t, uint8_t *, uint16_t);

typedef struct
{
  VL53L4CX_Init_Func Init;
  VL53L4CX_DeInit_Func DeInit;
  uint16_t Address;
  VL53L4CX_WriteReg_Func WriteReg;
  VL53L4CX_ReadReg_Func ReadReg;
  VL53L4CX_GetTick_Func GetTick;
} VL53L4CX_IO_t;

typedef struct
{
  VL53LX_DevData_t Data; /*!< ranging sensor context */
} VL53LX_Dev_t;

typedef struct
{
  VL53L4CX_IO_t IO;
  VL53LX_Dev_t Dev;
  uint8_t IsInitialized;    /*!< NotInitialized: 0, Initialized: 1 */
  uint8_t IsRanging;        /*!< Not Started: 0, Started: 1 */
  uint8_t IsBlocking;       /*!< Interrupt: 0, Polling: 1 */
  uint8_t IsContinuous;     /*!< One shot: 0, Continuous: 1 */
  uint8_t IsAmbientEnabled; /*!< Enabled: 0, Disabled: 1 */
  uint8_t IsSignalEnabled;  /*!< Enabled: 0, Disabled: 1 */
  uint8_t RangingProfile;
} VL53L4CX_Object_t;

typedef struct
{
  uint32_t NumberOfTargets;
  uint32_t Distance[VL53L4CX_NB_TARGET_PER_ZONE]; /*!< millimeter */
  uint32_t Status[VL53L4CX_NB_TARGET_PER_ZONE];   /*!< 0: OK, 1: NOK */
  float_t Ambient[VL53L4CX_NB_TARGET_PER_ZONE];   /*!< kcps / spad */
  float_t Signal[VL53L4CX_NB_TARGET_PER_ZONE];    /*!< kcps / spad */
} VL53L4CX_TargetResult_t;

typedef struct
{
  uint32_t NumberOfZones;
  VL53L4CX_TargetResult_t ZoneResult[VL53L4CX_MAX_NB_ZONES];
} VL53L4CX_Result_t;

typedef struct
{
  uint32_t Criteria;       /*!< interrupt generation criteria */
  uint32_t LowThreshold;   /*!< expressed in millimeters */
  uint32_t HighThreshold;  /*!< expressed in millimeters */
} VL53L4CX_ITConfig_t;

typedef struct
{
  uint8_t TopLeftX;  /*!< Top Left x coordinate:  0-15 range */
  uint8_t TopLeftY;  /*!< Top Left y coordinate:  0-15 range */
  uint8_t BotRightX; /*!< Bot Right x coordinate: 0-15 range */
  uint8_t BotRightY; /*!< Bot Right y coordinate: 0-15 range */
} VL53L4CX_ROIConfig_t;

typedef struct
{
  uint8_t RangingProfile;
  uint32_t TimingBudget;   /*!< Expressed in milliseconds */
  uint32_t Frequency;      /*!< Expressed in Hz */
  uint32_t EnableAmbient;  /*<! Enable: 1, Disable: 0 */
  uint32_t EnableSignal;   /*<! Enable: 1, Disable: 0 */
} VL53L4CX_ProfileConfig_t;

typedef struct
{
  uint32_t NumberOfZones;
  uint32_t MaxNumberOfTargetsPerZone;
  uint32_t CustomROI;           /*<! Not available: 0, Available: 1 */
  uint32_t ThresholdDetection;  /*<! Not available: 0, Available: 1 */
} VL53L4CX_Capabilities_t;

typedef struct
{
  int32_t (*Init)(VL53L4CX_Object_t *);
  int32_t (*DeInit)(VL53L4CX_Object_t *);
  int32_t (*ReadID)(VL53L4CX_Object_t *, uint32_t *);
  int32_t (*GetCapabilities)(VL53L4CX_Object_t *, VL53L4CX_Capabilities_t *);
  int32_t (*ConfigProfile)(VL53L4CX_Object_t *, VL53L4CX_ProfileConfig_t *);
  int32_t (*ConfigROI)(VL53L4CX_Object_t *, VL53L4CX_ROIConfig_t *);
  int32_t (*ConfigIT)(VL53L4CX_Object_t *, VL53L4CX_ITConfig_t *);
  int32_t (*GetDistance)(VL53L4CX_Object_t *, VL53L4CX_Result_t *);
  int32_t (*Start)(VL53L4CX_Object_t *, uint32_t);
  int32_t (*Stop)(VL53L4CX_Object_t *);
  int32_t (*SetAddress)(VL53L4CX_Object_t *, uint32_t);
  int32_t (*GetAddress)(VL53L4CX_Object_t *, uint32_t *);
  int32_t (*SetPowerMode)(VL53L4CX_Object_t *, uint32_t);
  int32_t (*GetPowerMode)(VL53L4CX_Object_t *, uint32_t *);
} VL53L4CX_RANGING_SENSOR_Drv_t;
/**
  * @}
  */

/** @defgroup VL53L4CX_Exported_Functions VL53L4CX Exported Functions
  * @{
  */
 /* RANGING_SENSOR methods */
int32_t VL53L4CX_RegisterBusIO(VL53L4CX_Object_t *pObj, VL53L4CX_IO_t *pIO);
int32_t VL53L4CX_Init(VL53L4CX_Object_t *pObj);
int32_t VL53L4CX_DeInit(VL53L4CX_Object_t *pObj);
int32_t VL53L4CX_ReadID(VL53L4CX_Object_t *pObj, uint32_t *pId);
int32_t VL53L4CX_GetCapabilities(VL53L4CX_Object_t *pObj, VL53L4CX_Capabilities_t *pCap);
int32_t VL53L4CX_ConfigProfile(VL53L4CX_Object_t *pObj, VL53L4CX_ProfileConfig_t *pConfig);
int32_t VL53L4CX_ConfigROI(VL53L4CX_Object_t *pObj, VL53L4CX_ROIConfig_t *pROIConfig);
int32_t VL53L4CX_ConfigIT(VL53L4CX_Object_t *pObj, VL53L4CX_ITConfig_t *pITConfig);
int32_t VL53L4CX_GetDistance(VL53L4CX_Object_t *pObj, VL53L4CX_Result_t *pResult);
int32_t VL53L4CX_Start(VL53L4CX_Object_t *pObj, uint32_t Mode);
int32_t VL53L4CX_Stop(VL53L4CX_Object_t *pObj);
int32_t VL53L4CX_SetAddress(VL53L4CX_Object_t *pObj, uint32_t Address);
int32_t VL53L4CX_GetAddress(VL53L4CX_Object_t *pObj, uint32_t *pAddress);
int32_t VL53L4CX_SetPowerMode(VL53L4CX_Object_t *pObj, uint32_t PowerMode);
int32_t VL53L4CX_GetPowerMode(VL53L4CX_Object_t *pObj, uint32_t *pPowerMode);

/* additional methods */
int32_t VL53L4CX_OffsetCalibration(VL53L4CX_Object_t *pObj, uint32_t CalDistance);
int32_t VL53L4CX_XTalkCalibration(VL53L4CX_Object_t *pObj);
/**
  * @}
  */

/* RANGING_SENSOR driver structure */
extern VL53L4CX_RANGING_SENSOR_Drv_t   VL53L4CX_RANGING_SENSOR_Driver;

#ifdef __cplusplus
}
#endif

#endif /* VL53L4CX_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
