/**
  ******************************************************************************
  * @file    vl53l4ed.c
  * @author  IMG SW Application Team
  * @brief   This file provides the VL53L4ED ranging sensor component driver
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

/* Includes ------------------------------------------------------------------*/
#include "vl53l4ed.h"
#include "platform.h"
#include "vl53l4ed_api.h"
#include "vl53l4ed_calibration.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup VL53L4ED
  * @brief     This file provides a set of functions needed to drive the
  *            VL53L4ED ranging sensor.
  * @{
  */

/** @defgroup VL53L4ED_Private_TypesDefinitions Private Types Definitions
  * @{
  */

#define V53L4CX_POLL_TIMEOUT  (0xFFFFU)
#define UNUSED(x) (void)(x)

#define OFFSET_CAL_SAMPLES 20 /* as per recommended in UM */
#define XTALK_CAL_SAMPLES 20 /* as per recommended in UM */

/**
  * @}
  */

/** @defgroup VL53L4ED_Private_Variables Private Variables
  * @{
  */

VL53L4ED_RANGING_SENSOR_Drv_t VL53L4ED_RANGING_SENSOR_Driver =
{
  VL53L4ED_Init,
  VL53L4ED_DeInit,
  VL53L4ED_ReadID,
  VL53L4ED_GetCapabilities,
  VL53L4ED_ConfigProfile,
  VL53L4ED_ConfigROI,
  VL53L4ED_ConfigIT,
  VL53L4ED_GetDistance,
  VL53L4ED_Start,
  VL53L4ED_Stop,
  VL53L4ED_SetAddress,
  VL53L4ED_GetAddress,
  VL53L4ED_SetPowerMode,
  VL53L4ED_GetPowerMode
};

/**
  * @}
  */


/** @defgroup VL53L4ED_Private_Functions_Prototypes Private Functions Prototypes
  * @{
  */
static int32_t vl53l4ed_poll_for_measurement(VL53L4ED_Object_t *pObj, uint32_t Timeout);
static int32_t vl53l4ed_get_result(VL53L4ED_Object_t *pObj, VL53L4ED_Result_t *pResult);
/**
  * @}
  */

/**
  * @brief Initializes the vl53l4ed context object.
  * @param pObj    vl53l4ed context object.
  * @param pIO     BSP IO struct.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_RegisterBusIO(VL53L4ED_Object_t *pObj, VL53L4ED_IO_t *pIO)
{
  int32_t ret;

  if ((pObj == NULL) || (pIO == NULL))
  {
    ret = VL53L4ED_INVALID_PARAM;
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
      ret = VL53L4ED_ERROR;
    }
  }

  return ret;
}

/**
  * @brief Initializes the vl53l4ed.
  * @param pObj    vl53l4ed context object.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_Init(VL53L4ED_Object_t *pObj)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else if (pObj->IsInitialized != 0U)
  {
    ret =  VL53L4ED_ERROR;
  }
  else if (VL53L4ED_SensorInit(pObj) != VL53L4ED_ERROR_NONE)
  {
    ret = VL53L4ED_ERROR;
  }
  else
  {
    pObj->IsRanging = 0U;
    pObj->IsBlocking = 0U;
    pObj->IsContinuous = 0U;
    pObj->IsAmbientEnabled = 0U;
    pObj->IsSignalEnabled = 0U;
    pObj->IsInitialized = 1U;
    ret = VL53L4ED_OK;
  }

  return ret;
}

/**
  * @brief Deinitializes the vl53l4ed.
  * @param pObj    vl53l4ed context object.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_DeInit(VL53L4ED_Object_t *pObj)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else if (pObj->IsInitialized == 1U)
  {
    /* De-initialize the vl53l4ed interface */
    if (pObj->IO.DeInit() != 0)
    {
      ret = VL53L4ED_ERROR;
    }
    else
    {
      ret = VL53L4ED_OK;
      pObj->IsInitialized = 0;
    }
  }
  else
  {
    /* if device not initialized return error */
    ret = VL53L4ED_ERROR;
  }

  return ret;
}

/**
  * @brief Read the vl53l4ed device ID.
  * @param pObj    vl53l4ed context object.
  * @param pId    Pointer to the device ID.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_ReadID(VL53L4ED_Object_t *pObj, uint32_t *pId)
{
  int32_t ret;
  uint16_t ReadId;

  if ((pObj == NULL) || (pId == NULL))
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else
  {
    *pId = 0;
    ret = (int32_t)VL53L4ED_RdWord(pObj, VL53L4ED_ID_REG, &ReadId);
    if (ret == (int32_t)VL53L4ED_ERROR_NONE)
    {
      *pId = ReadId;
    }
  }

  return ret;
}

/**
  * @brief Get the vl53l4ed capabilities.
  * @param pObj    vl53l4ed context object.
  * @param pCap    Pointer to the vl53l4ed capabilities.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_GetCapabilities(VL53L4ED_Object_t *pObj, VL53L4ED_Capabilities_t *pCap)
{
  int32_t ret;

  if ((pObj == NULL) || (pCap == NULL))
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else
  {
    pCap->NumberOfZones = VL53L4ED_MAX_NB_ZONES;
    pCap->MaxNumberOfTargetsPerZone = VL53L4ED_NB_TARGET_PER_ZONE;
    pCap->CustomROI = 0;
    pCap->ThresholdDetection = 1;

    ret = VL53L4ED_OK;
  }

  return ret;
}

/**
  * @brief Set the ranging configuration profile.
  * @param pObj    vl53l4ed context object.
  * @param pConfig    Pointer to the new configuration profile to be applied.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_ConfigProfile(VL53L4ED_Object_t *pObj, VL53L4ED_ProfileConfig_t *pConfig)
{
  int32_t ret;
  uint32_t intermeas_ms;

  if ((pObj != NULL) && (pConfig != NULL))
  {
    if ((pConfig->RangingProfile == VL53L4ED_PROFILE_AUTONOMOUS) &&
        (pConfig->Frequency > 0U))
    {
      intermeas_ms = 1000U / pConfig->Frequency;
    }
    else
    {
      intermeas_ms = 0;
    }
    if (VL53L4ED_SetRangeTiming(pObj,
        (pConfig->TimingBudget), intermeas_ms) != VL53L4ED_ERROR_NONE)
    {
      ret = VL53L4ED_ERROR;
    }
    else
    {
      pObj->IsAmbientEnabled = (pConfig->EnableAmbient == 0U) ? 0U : 1U;
      pObj->IsSignalEnabled = (pConfig->EnableSignal == 0U) ? 0U : 1U;
      ret = VL53L4ED_OK;
    }
  }
  else
  {
    ret = VL53L4ED_INVALID_PARAM;
  }

  return ret;
}

/**
  * @brief Configure the Region of Interest of the vl53l4ed.
  * @param pObj    vl53l4ed context object.
  * @param pROIConfig    Pointer to the ROI configuration struct.
  * @note This device does not support this function.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_ConfigROI(VL53L4ED_Object_t *pObj, VL53L4ED_ROIConfig_t *pROIConfig)
{
  UNUSED(pObj);
  UNUSED(pROIConfig);
  return VL53L4ED_NOT_IMPLEMENTED;
}

/**
  * @brief Configure the IT event generation parameters.
  * @param pObj    vl53l4ed context object.
  * @param pITConfig    Pointer to the IT configuration struct.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_ConfigIT(VL53L4ED_Object_t *pObj, VL53L4ED_ITConfig_t *pITConfig)
{
  int32_t ret;
  uint8_t window;
  uint16_t LowThreshold;
  uint16_t HighThreshold;

  if ((pObj == NULL) || (pITConfig == NULL))
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else
  {
    window = (uint8_t)pITConfig->Criteria; 
    switch (window)
    {
      case VL53L4ED_IT_DEFAULT:
        /* trigger IT for all distance set between 0 and 6m (far out of actual ranging) */
        ret = (int32_t)VL53L4ED_SetDetectionThresholds(pObj, 0, 6000, window);
      break;
      case VL53L4ED_IT_IN_WINDOW:
      case VL53L4ED_IT_OUT_OF_WINDOW:
      case VL53L4ED_IT_BELOW_LOW:
      case VL53L4ED_IT_ABOVE_HIGH:
        LowThreshold = (uint16_t)pITConfig->LowThreshold;
        HighThreshold = (uint16_t)pITConfig->HighThreshold;
        ret = (int32_t)VL53L4ED_SetDetectionThresholds(pObj,
            LowThreshold, HighThreshold, window);
      break;
      default:
        ret = VL53L4ED_INVALID_PARAM;
      break;
    }
  }

  return ret;
}

/**
  * @brief Get the last distance measurement information.
  * @param pObj    vl53l4ed context object.
  * @param pResult    Pointer to the result struct.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_GetDistance(VL53L4ED_Object_t *pObj, VL53L4ED_Result_t *pResult)
{
  int32_t ret;

  ret = VL53L4ED_OK;
  if ((pObj == NULL) || (pResult == NULL))
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else if (pObj->IsRanging == 0U)
  {
    ret = VL53L4ED_ERROR;
  }
  if (ret == VL53L4ED_OK)
  {
    if (pObj->IsBlocking == 1U)
    {
      ret = vl53l4ed_poll_for_measurement(pObj, V53L4CX_POLL_TIMEOUT);
    }
    else
    {
      ret = vl53l4ed_poll_for_measurement(pObj, 0U);
    }
  }

  /* a new measure is available if no error is returned by the poll function */
  if (ret == VL53L4ED_OK)
  {
    /* retrieve measurements and fill result structure */
    if (vl53l4ed_get_result(pObj, pResult) != VL53L4ED_OK)
    {
      ret = VL53L4ED_ERROR;
    }
    else if (pObj->IsContinuous == 1U)
    {
      /* trigger new measurement if device configured in continuous mode */
      ret = (int32_t)VL53L4ED_ClearInterrupt(pObj);
    }
    else
    {
      ret = VL53L4ED_OK;
    }
  }

  return ret;
}

/**
  * @brief Start ranging.
  * @param pObj    vl53l4ed context object.
  * @param Mode        The desired @ref RANGING_SENSOR_Mode_t
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_Start(VL53L4ED_Object_t *pObj, uint32_t Mode)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else if (VL53L4ED_StartRanging(pObj) == VL53L4ED_ERROR_NONE)
  {
    pObj->IsRanging = 1U;
    ret = VL53L4ED_OK;

    switch (Mode)
    {
      case VL53L4ED_MODE_BLOCKING_CONTINUOUS:
        pObj->IsContinuous = 1U;
        pObj->IsBlocking = 1U;
        break;

      case VL53L4ED_MODE_BLOCKING_ONESHOT:
        pObj->IsContinuous = 0U;
        pObj->IsBlocking = 1U;
        break;

      case VL53L4ED_MODE_ASYNC_CONTINUOUS:
        pObj->IsContinuous = 1U;
        pObj->IsBlocking = 0U;
        break;

      case VL53L4ED_MODE_ASYNC_ONESHOT:
        pObj->IsContinuous = 0U;
        pObj->IsBlocking = 0U;
        break;

      default:
        pObj->IsRanging = 0U;
        ret = VL53L4ED_INVALID_PARAM;
        break;
    }
  }
  else
  {
    ret = VL53L4ED_ERROR;
  }

  return ret;
}

/**
  * @brief Stop ranging.
  * @param pObj    vl53l4ed context object.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_Stop(VL53L4ED_Object_t *pObj)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else if (pObj->IsRanging == 0U)
  {
    /* ranging not started */
    ret = VL53L4ED_ERROR;
  }
  else if (VL53L4ED_StopRanging(pObj) == VL53L4ED_ERROR_NONE)
  {
    pObj->IsRanging = 0U;
    ret = VL53L4ED_OK;
  }
  else
  {
    ret = VL53L4ED_ERROR;
  }

  return ret;
}

/**
  * @brief Set The I2C address of the device.
  * @param pObj    vl53l4ed context object.
  * @param Address     New I2C address.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_SetAddress(VL53L4ED_Object_t *pObj, uint32_t Address)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else if (VL53L4ED_SetI2CAddress(pObj, (uint8_t)Address) != VL53L4ED_ERROR_NONE)
  {
    ret = VL53L4ED_ERROR;
  }
  else
  {
    pObj->IO.Address = (uint8_t)(Address & 0xFFU);
    ret = VL53L4ED_OK;
  }

  return ret;
}

/**
  * @brief Get The I2C address of the device.
  * @param pObj    vl53l4ed context object.
  * @param *pAddress     New I2C address.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_GetAddress(VL53L4ED_Object_t *pObj, uint32_t *pAddress)
{
  int32_t ret;

  if ((pObj == NULL) || (pAddress == NULL))
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else
  {
    *pAddress = pObj->IO.Address;
    ret = VL53L4ED_OK;
  }

  return ret;
}

/**
  * @brief Set the power mode.
  * @param pObj    vl53l4ed context object.
  * @param pPowerMode    Pointer to the current power mode.
  * @note Not implemented for this device
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_SetPowerMode(VL53L4ED_Object_t *pObj, uint32_t pPowerMode)
{
  UNUSED(pObj);
  UNUSED(pPowerMode);
  return VL53L4ED_NOT_IMPLEMENTED;
}

/**
  * @brief Get the power mode.
  * @param pObj    vl53l4ed context object.
  * @param pPowerMode    Pointer to the current power mode.
  * @note Not implemented for this device
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_GetPowerMode(VL53L4ED_Object_t *pObj, uint32_t *pPowerMode)
{
  UNUSED(pObj);
  UNUSED(pPowerMode);
  return VL53L4ED_NOT_IMPLEMENTED;
}

/**
  * @brief Perform an offset calibration.
  * @param pObj    vl53l4ed context object.
  * @param Distance    Target distance 17% gray at 100 mm recommended.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_OffsetCalibration(VL53L4ED_Object_t *pObj, uint32_t CalDistance)
{
  int32_t ret;
  int16_t TargetDistInMm;
  int16_t Offset;

  if (pObj == NULL)
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else
  {
    TargetDistInMm = (int16_t)CalDistance;
    ret = (int32_t)VL53L4ED_CalibrateOffset(pObj, TargetDistInMm, &Offset, OFFSET_CAL_SAMPLES);
    if (ret == (int32_t)VL53L4ED_ERROR_NONE)
    {
      ret = (int32_t)VL53L4ED_SetOffset(pObj, Offset);
    }
  }

  return ret;
}

/**
  * @brief Perform a xtalk calibration.
  * @param pObj    vl53l4ed context object.
  * @param Distance    Target distance miniumum 600 mm.
  * @note The recommended target reflectance value for Xtalk calibration is 17% gray.
  * @retval VL53L4ED status
  */
int32_t VL53L4ED_XTalkCalibration(VL53L4ED_Object_t *pObj, uint16_t Distance)
{
  int32_t ret;
  int16_t TargetDistInMm;
  uint16_t Xtalk;

  if (pObj == NULL)
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else
  {
    TargetDistInMm = (int16_t) Distance;
    ret = (int32_t)VL53L4ED_CalibrateXtalk(pObj, TargetDistInMm, &Xtalk, XTALK_CAL_SAMPLES);
    if (ret == (int32_t)VL53L4ED_ERROR_NONE)
    {
      ret = (int32_t)VL53L4ED_SetXtalk(pObj, Xtalk);
    }
  }

  return ret;
}

/** @defgroup VL53L4ED_Private_Functions Private Functions
  * @{
  */
static int32_t vl53l4ed_poll_for_measurement(VL53L4ED_Object_t *pObj, uint32_t Timeout)
{
  int32_t ret;
  uint32_t TickStart;
  uint8_t NewDataReady;

  if (pObj == NULL)
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else
  {
    ret = VL53L4ED_TIMEOUT;
    TickStart = pObj->IO.GetTick();

    do
    {
      (void)VL53L4ED_CheckForDataReady(pObj, &NewDataReady);

      if (NewDataReady == 1U)
      {
        ret = VL53L4ED_OK;
        break;
      }
    } while ((pObj->IO.GetTick() - TickStart) < Timeout);

  }

  return ret;
}

static int32_t vl53l4ed_get_result(VL53L4ED_Object_t *pObj, VL53L4ED_Result_t *pResult)
{
  int32_t ret;
  VL53L4ED_ResultsData_t data;

  if ((pObj == NULL) || (pResult == NULL))
  {
    ret = VL53L4ED_INVALID_PARAM;
  }
  else if (VL53L4ED_GetResult(pObj, &data) != VL53L4ED_ERROR_NONE)
  {
    ret = VL53L4ED_ERROR;
  }
  else
  {
    pResult->NumberOfZones = 1;
    pResult->ZoneResult[0].NumberOfTargets = 1;
    pResult->ZoneResult[0].Distance[0] = data.distance_mm;
    pResult->ZoneResult[0].Status[0] = data.range_status;
    pResult->ZoneResult[0].Ambient[0] = 0.0f;
    pResult->ZoneResult[0].Signal[0] = 0.0f;
    if (pObj->IsAmbientEnabled == 1U)
    {
      pResult->ZoneResult[0].Ambient[0] = (float_t)data.ambient_per_spad_kcps;
    }
    if (pObj->IsSignalEnabled == 1U)
    {
      pResult->ZoneResult[0].Signal[0] = (float_t)data.signal_per_spad_kcps;
    }

    ret = VL53L4ED_OK;
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
