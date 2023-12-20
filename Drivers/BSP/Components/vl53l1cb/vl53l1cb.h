/**
  ******************************************************************************
  * @file    vl53l1cb.h
  * @author  IMG SW Application Team
  * @brief   This file contains all the functions prototypes for the vl53l1cb.c
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
#ifndef VL53L1CB_H
#define VL53L1CB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <math.h>
#include "vl53l1_def.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup VL53L1CB
  * @{
  */

/** @defgroup VL53L1CB_Exported_Constants Exported Constants
  * @{
  */
#define VL53L1CB_ID                 (0xEACCU)
#define VL53L1CB_ID_REG             (0x010FU)

#define VL53L1CB_DEVICE_ADDRESS     (0x52U)
#define VL53L1CB_NB_TARGET_PER_ZONE (4U)
#define VL53L1CB_MAX_NB_ZONES       (1U)

/* vl53l1cb ranging profiles */
#define VL53L1CB_PROFILE_SHORT      (VL53L1_DISTANCEMODE_SHORT)
#define VL53L1CB_PROFILE_MEDIUM     (VL53L1_DISTANCEMODE_MEDIUM)
#define VL53L1CB_PROFILE_LONG       (VL53L1_DISTANCEMODE_LONG)

/* vl53l1cb ranging modes */
#define VL53L1CB_MODE_BLOCKING_CONTINUOUS (1U)
#define VL53L1CB_MODE_BLOCKING_ONESHOT    (2U)
#define VL53L1CB_MODE_ASYNC_CONTINUOUS    (3U)
#define VL53L1CB_MODE_ASYNC_ONESHOT       (4U)

/* vl53l1cb error codes */
#define VL53L1CB_OK                 (0)
#define VL53L1CB_ERROR              (-1)
#define VL53L1CB_INVALID_PARAM      (-2)
#define VL53L1CB_TIMEOUT            (-3)
#define VL53L1CB_NOT_IMPLEMENTED    (-4)
/**
  * @}
  */

/** @defgroup VL53L1CB_Exported_Types Exported Types
  * @{
  */
typedef int32_t (*VL53L1CB_Init_Func)(void);
typedef int32_t (*VL53L1CB_DeInit_Func)(void);
typedef int32_t (*VL53L1CB_GetTick_Func)(void);
typedef int32_t (*VL53L1CB_WriteReg_Func)(uint16_t, uint8_t *, uint16_t);
typedef int32_t (*VL53L1CB_ReadReg_Func)(uint16_t, uint8_t *, uint16_t);

typedef struct
{
  VL53L1CB_Init_Func Init;
  VL53L1CB_DeInit_Func DeInit;
  uint16_t Address;
  VL53L1CB_WriteReg_Func WriteReg;
  VL53L1CB_ReadReg_Func ReadReg;
  VL53L1CB_GetTick_Func GetTick;
} VL53L1CB_IO_t;

typedef struct
{
  VL53L1_DevData_t Data; /*!< ranging sensor context */
} VL53L1_Dev_t;

typedef struct
{
  VL53L1CB_IO_t IO;
  VL53L1_Dev_t Dev;
  uint8_t IsInitialized;    /*!< NotInitialized: 0, Initialized: 1 */
  uint8_t IsRanging;        /*!< Not Started: 0, Started: 1 */
  uint8_t IsBlocking;       /*!< Interrupt: 0, Polling: 1 */
  uint8_t IsContinuous;     /*!< One shot: 0, Continuous: 1 */
  uint8_t IsAmbientEnabled; /*!< Enabled: 0, Disabled: 1 */
  uint8_t IsSignalEnabled;  /*!< Enabled: 0, Disabled: 1 */
  uint8_t RangingProfile;
} VL53L1CB_Object_t;

typedef struct
{
  uint32_t NumberOfTargets;
  uint32_t Distance[VL53L1CB_NB_TARGET_PER_ZONE]; /*!< millimeter */
  uint32_t Status[VL53L1CB_NB_TARGET_PER_ZONE];   /*!< 0: OK, 1: NOK */
  float_t Ambient[VL53L1CB_NB_TARGET_PER_ZONE];   /*!< kcps / spad */
  float_t Signal[VL53L1CB_NB_TARGET_PER_ZONE];    /*!< kcps / spad */
} VL53L1CB_TargetResult_t;

typedef struct
{
  uint32_t NumberOfZones;
  VL53L1CB_TargetResult_t ZoneResult[VL53L1CB_MAX_NB_ZONES];
} VL53L1CB_Result_t;

typedef struct
{
  uint32_t Criteria;       /*!< interrupt generation criteria */
  uint32_t LowThreshold;   /*!< expressed in millimeters */
  uint32_t HighThreshold;  /*!< expressed in millimeters */
} VL53L1CB_ITConfig_t;

typedef struct
{
  uint8_t TopLeftX;  /*!< Top Left x coordinate:  0-15 range */
  uint8_t TopLeftY;  /*!< Top Left y coordinate:  0-15 range */
  uint8_t BotRightX; /*!< Bot Right x coordinate: 0-15 range */
  uint8_t BotRightY; /*!< Bot Right y coordinate: 0-15 range */
} VL53L1CB_ROIConfig_t;

typedef struct
{
  uint8_t RangingProfile;
  uint32_t TimingBudget;   /*!< Expressed in milliseconds */
  uint32_t Frequency;      /*!< Expressed in Hz */
  uint32_t EnableAmbient;  /*<! Enable: 1, Disable: 0 */
  uint32_t EnableSignal;   /*<! Enable: 1, Disable: 0 */
} VL53L1CB_ProfileConfig_t;

typedef struct
{
  uint32_t NumberOfZones;
  uint32_t MaxNumberOfTargetsPerZone;
  uint32_t CustomROI;           /*<! Not available: 0, Available: 1 */
  uint32_t ThresholdDetection;  /*<! Not available: 0, Available: 1 */
} VL53L1CB_Capabilities_t;

typedef struct
{
  int32_t (*Init)(VL53L1CB_Object_t *);
  int32_t (*DeInit)(VL53L1CB_Object_t *);
  int32_t (*ReadID)(VL53L1CB_Object_t *, uint32_t *);
  int32_t (*GetCapabilities)(VL53L1CB_Object_t *, VL53L1CB_Capabilities_t *);
  int32_t (*ConfigProfile)(VL53L1CB_Object_t *, VL53L1CB_ProfileConfig_t *);
  int32_t (*ConfigROI)(VL53L1CB_Object_t *, VL53L1CB_ROIConfig_t *);
  int32_t (*ConfigIT)(VL53L1CB_Object_t *, VL53L1CB_ITConfig_t *);
  int32_t (*GetDistance)(VL53L1CB_Object_t *, VL53L1CB_Result_t *);
  int32_t (*Start)(VL53L1CB_Object_t *, uint32_t);
  int32_t (*Stop)(VL53L1CB_Object_t *);
  int32_t (*SetAddress)(VL53L1CB_Object_t *, uint32_t);
  int32_t (*GetAddress)(VL53L1CB_Object_t *, uint32_t *);
  int32_t (*SetPowerMode)(VL53L1CB_Object_t *, uint32_t);
  int32_t (*GetPowerMode)(VL53L1CB_Object_t *, uint32_t *);
} VL53L1CB_RANGING_SENSOR_Drv_t;
/**
  * @}
  */

/** @defgroup VL53L1CB_Exported_Functions VL53L1CB Exported Functions
  * @{
  */
 /* RANGING_SENSOR methods */
int32_t VL53L1CB_RegisterBusIO(VL53L1CB_Object_t *pObj, VL53L1CB_IO_t *pIO);
int32_t VL53L1CB_Init(VL53L1CB_Object_t *pObj);
int32_t VL53L1CB_DeInit(VL53L1CB_Object_t *pObj);
int32_t VL53L1CB_ReadID(VL53L1CB_Object_t *pObj, uint32_t *pId);
int32_t VL53L1CB_GetCapabilities(VL53L1CB_Object_t *pObj, VL53L1CB_Capabilities_t *pCap);
int32_t VL53L1CB_ConfigProfile(VL53L1CB_Object_t *pObj, VL53L1CB_ProfileConfig_t *pConfig);
int32_t VL53L1CB_ConfigROI(VL53L1CB_Object_t *pObj, VL53L1CB_ROIConfig_t *pROIConfig);
int32_t VL53L1CB_ConfigIT(VL53L1CB_Object_t *pObj, VL53L1CB_ITConfig_t *pITConfig);
int32_t VL53L1CB_GetDistance(VL53L1CB_Object_t *pObj, VL53L1CB_Result_t *pResult);
int32_t VL53L1CB_Start(VL53L1CB_Object_t *pObj, uint32_t Mode);
int32_t VL53L1CB_Stop(VL53L1CB_Object_t *pObj);
int32_t VL53L1CB_SetAddress(VL53L1CB_Object_t *pObj, uint32_t Address);
int32_t VL53L1CB_GetAddress(VL53L1CB_Object_t *pObj, uint32_t *pAddress);
int32_t VL53L1CB_SetPowerMode(VL53L1CB_Object_t *pObj, uint32_t PowerMode);
int32_t VL53L1CB_GetPowerMode(VL53L1CB_Object_t *pObj, uint32_t *pPowerMode);

/* additional methods */
int32_t VL53L1CB_OffsetCalibration(VL53L1CB_Object_t *pObj, uint32_t CalDistance);
int32_t VL53L1CB_XTalkCalibration(VL53L1CB_Object_t *pObj);
/**
  * @}
  */

/* RANGING_SENSOR driver structure */
extern VL53L1CB_RANGING_SENSOR_Drv_t   VL53L1CB_RANGING_SENSOR_Driver;

#ifdef __cplusplus
}
#endif

#endif /* VL53L1CB_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
