/**
  ******************************************************************************
  * @file    vl53l1cb.c
  * @author  IMG SW Application Team
  * @brief   This file provides the VL53L1CB ranging sensor component driver
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

/* Includes ------------------------------------------------------------------*/
#include "vl53l1cb.h"
#include "vl53l1_platform.h"
#include "vl53l1_api.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup VL53L1CB
  * @brief     This file provides a set of functions needed to drive the
  *            VL53L1CB ranging sensor.
  * @{
  */

/** @defgroup VL53L1CB_Private_TypesDefinitions Private Types Definitions
  * @{
  */

#define VL53L1CB_POLL_TIMEOUT  (0xFFFFU)
#define UNUSED(x) (void)(x)

/**
  * @}
  */
/** @defgroup VL53L1CB_Private_Variables Private Variables
  * @{
  */

VL53L1CB_RANGING_SENSOR_Drv_t VL53L1CB_RANGING_SENSOR_Driver =
{
  VL53L1CB_Init,
  VL53L1CB_DeInit,
  VL53L1CB_ReadID,
  VL53L1CB_GetCapabilities,
  VL53L1CB_ConfigProfile,
  VL53L1CB_ConfigROI,
  VL53L1CB_ConfigIT,
  VL53L1CB_GetDistance,
  VL53L1CB_Start,
  VL53L1CB_Stop,
  VL53L1CB_SetAddress,
  VL53L1CB_GetAddress,
  VL53L1CB_SetPowerMode,
  VL53L1CB_GetPowerMode
};

/**
  * @}
  */


/** @defgroup VL53L1CB_Private_Functions_Prototypes Private Functions Prototypes
  * @{
  */
static int32_t vl53l1cb_poll_for_measurement(VL53L1CB_Object_t *pObj, uint32_t Timeout);
static int32_t vl53l1cb_get_result(VL53L1CB_Object_t *pObj, VL53L1CB_Result_t *pResult);

/**
  * @}
  */

/**
  * @brief Initializes the vl53l1cb context object.
  * @param pObj    vl53l1cb context object.
  * @param pIO     BSP IO struct.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_RegisterBusIO(VL53L1CB_Object_t *pObj, VL53L1CB_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = VL53L1CB_ERROR;
    }
  }

  return ret;
}

/**
  * @brief Initializes the vl53l1cb.
  * @param pObj    vl53l1cb context object.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_Init(VL53L1CB_Object_t *pObj)
{
  int32_t ret;

  if (pObj->IsInitialized != 0U)
  {
    ret =  VL53L1CB_ERROR;
  }
  else if (VL53L1_WaitDeviceBooted(pObj) != VL53L1_ERROR_NONE)
  {
    ret =  VL53L1CB_ERROR;
  }
  else if (VL53L1_DataInit(pObj) != VL53L1_ERROR_NONE)
  {
    ret = VL53L1CB_ERROR;
  }
  else if (VL53L1_StaticInit(pObj) != VL53L1_ERROR_NONE)
  {
    ret = VL53L1CB_ERROR;
  }
  else if (VL53L1_PerformRefSpadManagement(pObj) != VL53L1_ERROR_NONE)
  {
    ret = VL53L1CB_ERROR;
  }
  else
  {
    pObj->IsRanging = 0;
    pObj->IsBlocking = 0;
    pObj->IsContinuous = 0;
    pObj->IsAmbientEnabled = 0;
    pObj->IsSignalEnabled = 0;
    pObj->IsInitialized = 1;
    ret = VL53L1CB_OK;
  }

  return ret;
}

/**
  * @brief Deinitializes the vl53l1cb.
  * @param pObj    vl53l1cb context object.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_DeInit(VL53L1CB_Object_t *pObj)
{
  int32_t ret = VL53L1CB_ERROR;

  if (pObj->IsInitialized == 1U)
  {
    /* De-initialize the vl53l1cb interface */
    if (pObj->IO.DeInit() != 0)
    {
      ret = VL53L1CB_ERROR;
    }
    else
    {
      ret = VL53L1CB_OK;
      pObj->IsInitialized = 0;
    }
  }

  return ret;
}

/**
  * @brief Read the vl53l1cb device ID.
  * @param pObj    vl53l1cb context object.
  * @param pId    Pointer to the device ID.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_ReadID(VL53L1CB_Object_t *pObj, uint32_t *pId)
{
  int32_t ret;

  if ((pObj == NULL) || (pId == NULL))
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else
  {
    *pId = 0;
    ret = VL53L1_RdWord(pObj, VL53L1CB_ID_REG, (uint16_t *) pId);
  }

  return ret;
}

/**
  * @brief Get the vl53l1cb capabilities.
  * @param pObj    vl53l1cb context object.
  * @param pCap    Pointer to the vl53l1cb capabilities.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_GetCapabilities(VL53L1CB_Object_t *pObj, VL53L1CB_Capabilities_t *pCap)
{
  int32_t ret;

  if ((pObj == NULL) || (pCap == NULL))
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else
  {
    pCap->NumberOfZones = 1;
    pCap->MaxNumberOfTargetsPerZone = VL53L1CB_NB_TARGET_PER_ZONE;
    pCap->CustomROI = 1;
    pCap->ThresholdDetection = 0;

    ret = VL53L1CB_OK;
  }

  return ret;
}

/**
  * @brief Set the ranging configuration profile.
  * @param pObj    vl53l1cb context object.
  * @param pConfig    Pointer to the new configuration profile to be applied.
  * @note for VL53L1CB the profile corresponds to the distance modes.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_ConfigProfile(VL53L1CB_Object_t *pObj, VL53L1CB_ProfileConfig_t *pConfig)
{
  int32_t ret;
  uint8_t ranging_profile;

  if ((pObj != NULL) && (pConfig != NULL))
  {
    ranging_profile = pConfig->RangingProfile;
  }
  else
  {
    return VL53L1CB_INVALID_PARAM;
  }

  if ((ranging_profile != VL53L1CB_PROFILE_SHORT) &&
      (ranging_profile != VL53L1CB_PROFILE_MEDIUM) &&
      (ranging_profile != VL53L1CB_PROFILE_LONG))
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else if (VL53L1_SetDistanceMode(pObj, ranging_profile) != VL53L1_ERROR_NONE)
  {
    ret = VL53L1CB_ERROR;
  }
  else if (VL53L1_SetMeasurementTimingBudgetMicroSeconds(
             pObj, (1000U * pConfig->TimingBudget)) != VL53L1_ERROR_NONE)
  {
    ret = VL53L1CB_ERROR;
  }
  else
  {
    pObj->IsAmbientEnabled = (pConfig->EnableAmbient == 0U) ? 0U : 1U;
    pObj->IsSignalEnabled = (pConfig->EnableSignal == 0U) ? 0U : 1U;

    ret = VL53L1CB_OK;
  }

  return ret;
}

/**
  * @brief Configure the Region of Interest of the vl53l1cb.
  * @param pObj    vl53l1cb context object.
  * @param pROIConfig    Pointer to the ROI configuration struct.
  * @note Minimal ROI size is 4x4 spads.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_ConfigROI(VL53L1CB_Object_t *pObj, VL53L1CB_ROIConfig_t *pROIConfig)
{
  int32_t ret;
  VL53L1_RoiConfig_t roi_settings;

  if ((pObj == NULL) || (pROIConfig == NULL))
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else
  {
    roi_settings.NumberOfRoi = 1;
    roi_settings.UserRois[0].BotRightX = pROIConfig->BotRightX;
    roi_settings.UserRois[0].BotRightY = pROIConfig->BotRightY;
    roi_settings.UserRois[0].TopLeftX = pROIConfig->TopLeftX;
    roi_settings.UserRois[0].TopLeftY = pROIConfig->TopLeftY;

    if (VL53L1_SetROI(pObj, &roi_settings) != VL53L1_ERROR_NONE)
    {
      ret = VL53L1CB_ERROR;
    }
    else
    {
      ret = VL53L1CB_OK;
    }
  }

  return ret;
}

/**
  * @brief Configure the IT event generation parameters.
  * @param pObj    vl53l1cb context object.
  * @param pITConfig    Pointer to the IT configuration struct.
  * @warning This device does not support this feature.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_ConfigIT(VL53L1CB_Object_t *pObj, VL53L1CB_ITConfig_t *pITConfig)
{
  UNUSED(pObj);
  UNUSED(pITConfig);
  return VL53L1CB_NOT_IMPLEMENTED;
}

/**
  * @brief Get the last distance measurement information.
  * @param pObj    vl53l1cb context object.
  * @param pResult    Pointer to the result struct.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_GetDistance(VL53L1CB_Object_t *pObj, VL53L1CB_Result_t *pResult)
{
  int32_t ret;

  ret = VL53L1CB_OK;
  if ((pObj == NULL) || (pResult == NULL))
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else if (pObj->IsRanging == 0U)
  {
    ret = VL53L1CB_ERROR;
  }
  if (ret == VL53L1CB_OK)
  {
    if (pObj->IsBlocking == 1U)
    {
      ret = vl53l1cb_poll_for_measurement(pObj, VL53L1CB_POLL_TIMEOUT);
    }
    else
    {
      ret = vl53l1cb_poll_for_measurement(pObj, 0U);
    }
    /* a new measure is available if no error is returned by poll function */
    if (ret == VL53L1CB_OK)
    {
      /* retrieve measurements and fill result structure */
      if (vl53l1cb_get_result(pObj, pResult) != VL53L1CB_OK)
      {
        ret = VL53L1CB_ERROR;
      }
      else if (pObj->IsContinuous == 1U)
      {
        /* trigger new measurement if device configured in continuous mode */
        ret = (int32_t)VL53L1_ClearInterruptAndStartMeasurement(pObj);
      }
      else
      {
        ret = VL53L1CB_OK;
      }
    }
  }
  
  return ret;
}

/**
  * @brief Start ranging.
  * @param pObj    vl53l1cb context object.
  * @param Mode        The desired ranging mode.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_Start(VL53L1CB_Object_t *pObj, uint32_t Mode)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else if (pObj->IsRanging == 1U)
  {
    ret = VL53L1CB_ERROR;
  }
  else if (VL53L1_StartMeasurement(pObj) == VL53L1_ERROR_NONE)
  {
    pObj->IsRanging = 1;
    ret = VL53L1CB_OK;

    ret = (int32_t)VL53L1_ClearInterruptAndStartMeasurement(pObj);

    switch (Mode)
    {
    case VL53L1CB_MODE_BLOCKING_CONTINUOUS:
      pObj->IsContinuous = 1U;
      pObj->IsBlocking = 1U;
      break;

    case VL53L1CB_MODE_BLOCKING_ONESHOT:
      pObj->IsContinuous = 0U;
      pObj->IsBlocking = 1U;
      break;

    case VL53L1CB_MODE_ASYNC_CONTINUOUS:
      pObj->IsContinuous = 1U;
      pObj->IsBlocking = 0U;
      break;

    case VL53L1CB_MODE_ASYNC_ONESHOT:
      pObj->IsContinuous = 0U;
      pObj->IsBlocking = 0U;
      break;

    default:
      pObj->IsRanging = 0U;
      ret = VL53L1CB_INVALID_PARAM;
      break;
    }
  }
  else
  {
    ret = VL53L1CB_ERROR;
  }

  return ret;
}

/**
  * @brief Stop ranging.
  * @param pObj    vl53l1cb context object.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_Stop(VL53L1CB_Object_t *pObj)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else if (pObj->IsRanging == 0U)
  {
	  /* ranging not started */
	  ret = VL53L1CB_ERROR;
  }
  else if (VL53L1_StopMeasurement(pObj) == VL53L1_ERROR_NONE)
  {
    pObj->IsRanging = 0U;
    ret = VL53L1CB_OK;
  }
  else
  {
    ret = VL53L1CB_ERROR;
  }

  return ret;
}

/**
  * @brief Set the I2C address of the device.
  * @param pObj    vl53l1cb context object.
  * @param Address     New I2C address.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_SetAddress(VL53L1CB_Object_t *pObj, uint32_t Address)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else if (VL53L1_SetDeviceAddress(pObj, (uint8_t)Address) != VL53L1_ERROR_NONE)
  {
    ret = VL53L1CB_ERROR;
  }
  else
  {
    pObj->IO.Address = (uint16_t) Address;
    ret = VL53L1CB_OK;
  }

  return ret;
}

/**
  * @brief Get the current I2C address.
  * @param pObj    vl53l1cb context object.
  * @param pAddress     Pointer to the current I2C address.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_GetAddress(VL53L1CB_Object_t *pObj, uint32_t *pAddress)
{
  int32_t ret;

  if ((pObj == NULL) || (pAddress == NULL))
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else
  {
    *pAddress = pObj->IO.Address;
    ret = VL53L1CB_OK;
  }

  return ret;
}

/**
  * @brief Set the power mode.
  * @param pObj    vl53l1cb context object.
  * @param PowerMode    New power mode to be entered.
  * @note Not implemented for this device
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_SetPowerMode(VL53L1CB_Object_t *pObj, uint32_t PowerMode)
{
  UNUSED(pObj);
  UNUSED(PowerMode);
  return VL53L1CB_NOT_IMPLEMENTED;
}

/**
  * @brief Get the power mode.
  * @param pObj    vl53l1cb context object.
  * @param pPowerMode    Pointer to the current power mode.
  * @note Not implemented for this device
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_GetPowerMode(VL53L1CB_Object_t *pObj, uint32_t *pPowerMode)
{
  UNUSED(pObj);
  UNUSED(pPowerMode);
  return VL53L1CB_NOT_IMPLEMENTED;
}

/**
  * @brief Perform an offset calibration.
  * @param pObj    vl53l1cb context object.
  * @param CalDistance    Calibration distance in mm.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_OffsetCalibration(VL53L1CB_Object_t *pObj, uint32_t CalDistance)
{
  int32_t ret;

  VL53L1_OffsetCorrectionModes Mode = VL53L1_OFFSETCORRECTIONMODE_STANDARD;

  if (VL53L1_SetOffsetCorrectionMode(pObj, Mode) != VL53L1_ERROR_NONE)
  {
    ret = VL53L1CB_ERROR;
  }
  else if (VL53L1_PerformOffsetSimpleCalibration(pObj, (int32_t)CalDistance) != VL53L1_ERROR_NONE)
  {
    ret = VL53L1CB_ERROR;
  }
  else
  {
    ret = VL53L1CB_OK;
  }

  return ret;
}

/**
  * @brief Perform a xtalk calibration.
  * @param pObj    vl53l1cb context object.
  * @retval VL53L1CB status
  */
int32_t VL53L1CB_XTalkCalibration(VL53L1CB_Object_t *pObj)
{
  int32_t ret;

  if (VL53L1_PerformXTalkCalibration(pObj, VL53L1_XTALKCALIBRATIONMODE_FULL_ROI) != VL53L1_ERROR_NONE)
  {
    ret = VL53L1CB_ERROR;
  }
  else
  {
    ret = VL53L1CB_OK;
  }

  return ret;
}

/** @defgroup VL53L1CB_Private_Functions Private Functions
  * @{
  */

static int32_t vl53l1cb_poll_for_measurement(VL53L1CB_Object_t *pObj, uint32_t Timeout)
{
  uint32_t TickStart;
  uint8_t NewDataReady = 0;

  if (pObj == NULL)
  {
    return VL53L1CB_INVALID_PARAM;
  }

  TickStart = pObj->IO.GetTick();

  do
  {
    (void)VL53L1_GetMeasurementDataReady(pObj, &NewDataReady);

    if (NewDataReady == 1U)
    {
      return VL53L1CB_OK;
    }
  } while ((pObj->IO.GetTick() - TickStart) < Timeout);

  return VL53L1CB_TIMEOUT;
}

static int32_t vl53l1cb_get_result(VL53L1CB_Object_t *pObj, VL53L1CB_Result_t *pResult)
{
  int32_t ret;
  uint8_t i, j;
  uint16_t spad_count; /* number of active spads for the current measurement */
  float_t ambient_temp, signal_temp; /* temporary variables used for computation */
  static VL53L1_MultiRangingData_t data;

  if ((pObj == NULL) || (pResult == NULL))
  {
    ret = VL53L1CB_INVALID_PARAM;
  }
  else if (VL53L1_GetMultiRangingData(pObj, &data) != VL53L1_ERROR_NONE)
  {
    ret = VL53L1CB_ERROR;
  }
  else
  {
    for (i = 0; i < VL53L1CB_MAX_NB_ZONES; i++)
    {
      /* number of detected targets by the device */
      pResult->ZoneResult[i].NumberOfTargets = data.NumberOfObjectsFound;

      for (j = 0; j < data.NumberOfObjectsFound; j++)
      {
        /* clip the value if negative */
        if (data.RangeData[j].RangeMilliMeter < 0)
        {
          pResult->ZoneResult[i].Distance[j] = 0U;
        }
        else
        {
          pResult->ZoneResult[i].Distance[j] = (uint32_t)data.RangeData[j].RangeMilliMeter;
        }
        
        /* compute and return Ambient value if ambient rate output is enabled :
         * - convert value from FixPoint1616 to Mcps by dividing by 65536
         * - convert value from Mcps to Kcps by multiplying it by 1000
         * - obtain number of active spads by dividing EffectiveSpadRtnCount by 256
         * - convert ambient value from Kcps to Kcps/spad diving by the number of active spads  
         */
        if (pObj->IsAmbientEnabled == 1U)
        {
          ambient_temp = (data.RangeData[j].AmbientRateRtnMegaCps / 65536.0f) * 1000.0f;
          spad_count = data.EffectiveSpadRtnCount >> 8;
          pResult->ZoneResult[i].Ambient[j] = ambient_temp / (float_t)spad_count;
        }
        else
        {
          pResult->ZoneResult[i].Ambient[j] = 0.0f;
        }

        /* compute and return Ambient value if signal rate output is enabled :
         * - convert value from FixPoint1616 to Mcps by dividing by 65536
         * - convert value from Mcps to Kcps by multiplying it by 1000
         * - obtain number of active spads by dividing EffectiveSpadRtnCount by 256
         * - convert ambient value from Kcps to Kcps/spad diving by the number of active spads  
         */
        if (pObj->IsSignalEnabled == 1U)
        {
          signal_temp = (data.RangeData[j].SignalRateRtnMegaCps / 65536.0f) * 1000.0f;
          spad_count = data.EffectiveSpadRtnCount >> 8;
          pResult->ZoneResult[i].Signal[j] = signal_temp / (float_t)spad_count;
        }
        else
        {
          pResult->ZoneResult[i].Signal[j] = 0.0f;
        }

        pResult->ZoneResult[i].Status[j] = data.RangeData[j].RangeStatus;
      }
    }

    ret = VL53L1CB_OK;
  }

  return ret;
}

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
