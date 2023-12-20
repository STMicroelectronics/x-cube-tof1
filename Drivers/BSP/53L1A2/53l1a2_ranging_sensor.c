/**
  ******************************************************************************
  * @file    53l1a2_ranging_sensor.c
  * @author  IMG SW Application Team
  * @brief   This file includes the driver for Ranging Sensor modules mounted on
  *          X-NUCLEO 53L1A2 expansion board.
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

/* Includes ------------------------------------------------------------------ */

#include "53l1a2_ranging_sensor.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup XNUCLEO_53L1A2
  * @{
  */

/** @addtogroup XNUCLEO_53L1A2_RANGING_SENSOR
  * @{
  */

/** @addtogroup XNUCLEO_53L1A2_RANGING_SENSOR_Exported_Variables
  * @{
  */
void *VL53L1A2_RANGING_SENSOR_CompObj[RANGING_SENSOR_INSTANCES_NBR] = {0};
/**
  * @}
  */

/** @defgroup XNUCLEO_53L1A2_RANGING_SENSOR_Private_Variables Private Variables
  * @{
  */
static RANGING_SENSOR_Drv_t *VL53L1A2_RANGING_SENSOR_Drv = NULL;
static RANGING_SENSOR_Capabilities_t VL53L1A2_RANGING_SENSOR_Cap;
/**
  * @}
  */

/** @defgroup XNUCLEO_53L1A2_RANGING_SENSOR_Private_Functions_Prototypes Private Functions Prototypes
  * @{
  */
static int32_t VL53L1CB_Probe(uint32_t Instance);
/**
  * @}
  */

/**
  * @brief Initialize the ranging sensor.
  * @param Instance    Ranging sensor instance.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_Init(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_Init() != 0)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    /* reset XSHUT (XSDN) pin */
    (void)VL53L1A2_RANGING_SENSOR_SetPowerMode(Instance, RANGING_SENSOR_POWERMODE_OFF);
    (void)VL53L1A2_RANGING_SENSOR_SetPowerMode(Instance, RANGING_SENSOR_POWERMODE_ON);

    ret = VL53L1CB_Probe(Instance);
  }

  return ret;
}

/**
  * @brief De-initialize the ranging sensor.
  * @param Instance    Ranging sensor instance.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_DeInit() != 0)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->DeInit(VL53L1A2_RANGING_SENSOR_CompObj[Instance]) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Read the ranging sensor device ID.
  * @param Instance    Ranging sensor instance.
  * @param pId    Pointer to the device ID.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_ReadID(uint32_t Instance, uint32_t *pId)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->ReadID(VL53L1A2_RANGING_SENSOR_CompObj[Instance], pId) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Get the ranging sensor capabilities.
  * @param Instance    Ranging sensor instance.
  * @param pCapabilities    Pointer to the ranging sensor capabilities.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_GetCapabilities(uint32_t Instance, RANGING_SENSOR_Capabilities_t *pCapabilities)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->GetCapabilities(VL53L1A2_RANGING_SENSOR_CompObj[Instance], pCapabilities) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Set the ranging configuration profile.
  * @param Instance    Ranging sensor instance.
  * @param pConfig    Pointer to the new configuration profile to be applied.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_ConfigProfile(uint32_t Instance, RANGING_SENSOR_ProfileConfig_t *pConfig)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->ConfigProfile(VL53L1A2_RANGING_SENSOR_CompObj[Instance], pConfig) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Configure the Region of Interest of the ranging sensor.
  * @param Instance    Ranging sensor instance.
  * @param pConfig    Pointer to the ROI configuration struct.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_ConfigROI(uint32_t Instance, RANGING_SENSOR_ROIConfig_t *pConfig)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Cap.CustomROI == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->ConfigROI(VL53L1A2_RANGING_SENSOR_CompObj[Instance], pConfig) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Configure the IT event generation parameters.
  * @param Instance    Ranging sensor instance.
  * @param pConfig    Pointer to the IT configuration struct.
  * @note The threshold modes can be used only if supported by the device (check the capabilities).
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_ConfigIT(uint32_t Instance, RANGING_SENSOR_ITConfig_t *pConfig)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->ConfigIT(VL53L1A2_RANGING_SENSOR_CompObj[Instance], pConfig) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Get the last distance measurement information.
  * @param Instance    Ranging sensor instance.
  * @param pResult    Pointer to the result struct.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_GetDistance(uint32_t Instance, RANGING_SENSOR_Result_t *pResult)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->GetDistance(VL53L1A2_RANGING_SENSOR_CompObj[Instance], pResult) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Start ranging.
  * @param Instance    Ranging sensor instance.
  * @param Mode        The desired RANGING_SENSOR_Mode_t
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_Start(uint32_t Instance, uint32_t Mode)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->Start(VL53L1A2_RANGING_SENSOR_CompObj[Instance], Mode) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Stop ranging.
  * @param Instance    Ranging sensor instance.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_Stop(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->Stop(VL53L1A2_RANGING_SENSOR_CompObj[Instance]) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Set the I2C address of the device.
  * @param Instance    Ranging sensor instance.
  * @param Address     New I2C address.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_SetAddress(uint32_t Instance, uint32_t Address)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->SetAddress(VL53L1A2_RANGING_SENSOR_CompObj[Instance], Address) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Get the I2C address of the device.
  * @param Instance    Ranging sensor instance.
  * @param pAddress    Pointer to the current I2C address.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_GetAddress(uint32_t Instance, uint32_t *pAddress)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->GetAddress(VL53L1A2_RANGING_SENSOR_CompObj[Instance], pAddress) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Set the power mode.
  * @param Instance    Ranging sensor instance.
  * @param PowerMode    New power mode to be entered.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_SetPowerMode(uint32_t Instance, uint32_t PowerMode)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (PowerMode == RANGING_SENSOR_POWERMODE_ON)
  {
    if (VL53L1A2_ResetId((uint8_t)Instance, 1) < 0)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      HAL_Delay(2);
      ret = BSP_ERROR_NONE;
    }
  }
  else if (PowerMode == RANGING_SENSOR_POWERMODE_OFF)
  {
    if (VL53L1A2_ResetId((uint8_t)Instance, 0) < 0)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      HAL_Delay(2);
      ret = BSP_ERROR_NONE;
    }
  }
  else
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }

  return ret;
}

/**
  * @brief Set the power mode.
  * @param Instance    Ranging sensor instance.
  * @param pPowerMode    Pointer to the current power mode.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_GetPowerMode(uint32_t Instance, uint32_t *pPowerMode)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1A2_RANGING_SENSOR_Drv->GetPowerMode(VL53L1A2_RANGING_SENSOR_CompObj[Instance], pPowerMode) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Perform an offset calibration.
  * @param Instance    Ranging sensor instance.
  * @param CalDistance   Calibration distance in mm.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_OffsetCalibration(uint32_t Instance, uint32_t CalDistance)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1CB_OffsetCalibration(VL53L1A2_RANGING_SENSOR_CompObj[Instance], CalDistance) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief Perform a xtalk calibration.
  * @param Instance    Ranging sensor instance.
  * @retval BSP status
  */
int32_t VL53L1A2_RANGING_SENSOR_XTalkCalibration(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= RANGING_SENSOR_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (VL53L1CB_XTalkCalibration(VL53L1A2_RANGING_SENSOR_CompObj[Instance]) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/** @defgroup XNUCLEO_53L1A2_RANGING_SENSOR_Private_Functions Private Functions
  * @{
  */

/**
  * @brief Register Bus IOs if component ID is OK.
  * @param Instance    Ranging sensor instance.
  * @retval BSP status
  */
static int32_t VL53L1CB_Probe(uint32_t Instance)
{
  int32_t ret;
  VL53L1CB_IO_t              IOCtx;
  uint32_t                   id;
  static VL53L1CB_Object_t   VL53L1CBObj[RANGING_SENSOR_INSTANCES_NBR];

  /* Configure the ranging sensor driver */
  IOCtx.Address     = RANGING_SENSOR_VL53L1CB_ADDRESS;
  IOCtx.Init        = VL53L1A2_I2C_INIT;
  IOCtx.DeInit      = VL53L1A2_I2C_DEINIT;
  IOCtx.WriteReg    = VL53L1A2_I2C_WRITEREG;
  IOCtx.ReadReg     = VL53L1A2_I2C_READREG;
  IOCtx.GetTick     = VL53L1A2_GETTICK;

  if (VL53L1CB_RegisterBusIO(&(VL53L1CBObj[Instance]), &IOCtx) != VL53L1CB_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (VL53L1CB_ReadID(&(VL53L1CBObj[Instance]), &id) != VL53L1CB_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    if (id != VL53L1CB_ID)
    {
      ret = BSP_ERROR_UNKNOWN_COMPONENT;
    }
    else
    {
      VL53L1A2_RANGING_SENSOR_Drv = (RANGING_SENSOR_Drv_t *) &VL53L1CB_RANGING_SENSOR_Driver;
      VL53L1A2_RANGING_SENSOR_CompObj[Instance] = &(VL53L1CBObj[Instance]);

      if (VL53L1A2_RANGING_SENSOR_Drv->Init(VL53L1A2_RANGING_SENSOR_CompObj[Instance]) != VL53L1CB_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else if (VL53L1A2_RANGING_SENSOR_Drv->GetCapabilities(VL53L1A2_RANGING_SENSOR_CompObj[Instance], &VL53L1A2_RANGING_SENSOR_Cap) != VL53L1CB_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
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

