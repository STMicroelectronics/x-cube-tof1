/**
  ******************************************************************************
  * @file    53l1a2_ranging_sensor.h
  * @author  IMG SW Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the 53l1a2_ranging_sensor.c driver.
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
#ifndef XNUCLEO_53L1A2_RANGING_SENSOR_H
#define XNUCLEO_53L1A2_RANGING_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "53l1a2.h"
#include "ranging_sensor.h"

#include "vl53l1cb.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup XNUCLEO_53L1A2
  * @{
  */

/** @defgroup XNUCLEO_53L1A2_RANGING_SENSOR RANGING SENSOR
  * @{
  */

/** @defgroup XNUCLEO_53L1A2_RANGING_SENSOR_Exported_Constants Exported Constants
  * @{
  */
#define RANGING_SENSOR_VL53L1CB_ADDRESS         (VL53L1CB_DEVICE_ADDRESS)
#define RANGING_SENSOR_NB_TARGET_PER_ZONE       (VL53L1CB_NB_TARGET_PER_ZONE)
#define RANGING_SENSOR_MAX_NB_ZONES             (VL53L1CB_MAX_NB_ZONES)

#define RANGING_SENSOR_POWERMODE_ON   (0U)  /*!< Run */
#define RANGING_SENSOR_POWERMODE_OFF  (1U)  /*!< Standby */

#define RS_MODE_BLOCKING_CONTINUOUS   (VL53L1CB_MODE_BLOCKING_CONTINUOUS)
#define RS_MODE_BLOCKING_ONESHOT      (VL53L1CB_MODE_BLOCKING_ONESHOT)
#define RS_MODE_ASYNC_CONTINUOUS      (VL53L1CB_MODE_ASYNC_CONTINUOUS)
#define RS_MODE_ASYNC_ONESHOT         (VL53L1CB_MODE_ASYNC_ONESHOT)

#define RS_MULTI_TARGET_SHORT_RANGE   (VL53L1CB_PROFILE_SHORT)
#define RS_MULTI_TARGET_MEDIUM_RANGE  (VL53L1CB_PROFILE_MEDIUM)
#define RS_MULTI_TARGET_LONG_RANGE    (VL53L1CB_PROFILE_LONG)
/**
  * @}
  */

/** @defgroup XNUCLEO_53L1A2_RANGING_SENSOR_Exported_Types Exported Types
  * @{
  */
typedef struct
{
  uint32_t RangingProfile;
  uint32_t TimingBudget;   /*!< Expressed in milliseconds */
  uint32_t Frequency;      /*!< Expressed in Hz */
  uint32_t EnableAmbient;  /*<! Enable: 1, Disable: 0 */
  uint32_t EnableSignal;   /*<! Enable: 1, Disable: 0 */
} RANGING_SENSOR_ProfileConfig_t;

typedef struct
{
  uint32_t NumberOfZones;
  uint32_t MaxNumberOfTargetsPerZone;
  uint32_t CustomROI;          /*!< Not available: 0, Available: 1 */
  uint32_t ThresholdDetection; /*!< Not available: 0, Available: 1 */
} RANGING_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t NumberOfTargets;
  uint32_t Distance[RANGING_SENSOR_NB_TARGET_PER_ZONE];  /*!< millimeters */
  uint32_t Status[RANGING_SENSOR_NB_TARGET_PER_ZONE];    /*!< OK: 0, NOK: !0 */
  float_t Ambient[RANGING_SENSOR_NB_TARGET_PER_ZONE];    /*!< kcps / spad */
  float_t Signal[RANGING_SENSOR_NB_TARGET_PER_ZONE];     /*!< kcps / spad */
} RANGING_SENSOR_ZoneResult_t;

typedef struct
{
  uint32_t NumberOfZones;
  RANGING_SENSOR_ZoneResult_t ZoneResult[RANGING_SENSOR_MAX_NB_ZONES];
} RANGING_SENSOR_Result_t;

typedef struct
{
  uint32_t Criteria;       /*!< interrupt generation criteria */
  uint32_t LowThreshold;   /*!< expressed in millimeters */ 
  uint32_t HighThreshold;  /*!< expressed in millimeters */
} RANGING_SENSOR_ITConfig_t;

typedef struct
{
  uint8_t TopLeftX;   /*!< Top Left x coordinate:  0-15 range */
  uint8_t TopLeftY;   /*!< Top Left y coordinate:  0-15 range */
  uint8_t BotRightX;  /*!< Bot Right x coordinate: 0-15 range */
  uint8_t BotRightY;  /*!< Bot Right y coordinate: 0-15 range */
} RANGING_SENSOR_ROIConfig_t;
/**
  * @}
  */

/** @defgroup XNUCLEO_53L1A2_RANGING_SENSOR_Exported_Variables Exported Variables
  * @{
  */
extern void *VL53L1A2_RANGING_SENSOR_CompObj[RANGING_SENSOR_INSTANCES_NBR];
/**
  * @}
  */

/** @defgroup XNUCLEO_53L1A2_RANGING_SENSOR_Exported_Functions Exported Functions
  * @{
  */
int32_t VL53L1A2_RANGING_SENSOR_Init(uint32_t Instance);
int32_t VL53L1A2_RANGING_SENSOR_DeInit(uint32_t Instance);
int32_t VL53L1A2_RANGING_SENSOR_ReadID(uint32_t Instance, uint32_t *pId);
int32_t VL53L1A2_RANGING_SENSOR_GetCapabilities(uint32_t Instance, RANGING_SENSOR_Capabilities_t *pCapabilities);
int32_t VL53L1A2_RANGING_SENSOR_ConfigProfile(uint32_t Instance, RANGING_SENSOR_ProfileConfig_t *pConfig);
int32_t VL53L1A2_RANGING_SENSOR_ConfigROI(uint32_t Instance, RANGING_SENSOR_ROIConfig_t *pConfig);
int32_t VL53L1A2_RANGING_SENSOR_ConfigIT(uint32_t Instance, RANGING_SENSOR_ITConfig_t *pConfig);
int32_t VL53L1A2_RANGING_SENSOR_GetDistance(uint32_t Instance, RANGING_SENSOR_Result_t *pResult);
int32_t VL53L1A2_RANGING_SENSOR_Start(uint32_t Instance, uint32_t Mode);
int32_t VL53L1A2_RANGING_SENSOR_Stop(uint32_t Instance);
int32_t VL53L1A2_RANGING_SENSOR_SetAddress(uint32_t Instance, uint32_t Address);
int32_t VL53L1A2_RANGING_SENSOR_GetAddress(uint32_t Instance, uint32_t *pAddress);
int32_t VL53L1A2_RANGING_SENSOR_SetPowerMode(uint32_t Instance, uint32_t PowerMode);
int32_t VL53L1A2_RANGING_SENSOR_GetPowerMode(uint32_t Instance, uint32_t *pPowerMode);

int32_t VL53L1A2_RANGING_SENSOR_OffsetCalibration(uint32_t Instance, uint32_t CalDistance);
int32_t VL53L1A2_RANGING_SENSOR_XTalkCalibration(uint32_t Instance);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* XNUCLEO_53L1A2_RANGING_SENSOR_H */

