/**
  ******************************************************************************
  * @file    custom_ranging_sensor.c
  * @author  IMG SW Application Team
  * @brief   This file provides BSP Ranging Sensors interface for custom boards
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
#include "custom_ranging_sensor.h"

void *CUSTOM_RANGING_CompObj[CUSTOM_RANGING_INSTANCES_NBR] = {0};
static RANGING_SENSOR_Drv_t *CUSTOM_RANGING_Drv[CUSTOM_RANGING_INSTANCES_NBR];
static RANGING_SENSOR_Capabilities_t RANGING_SENSOR_Cap[CUSTOM_RANGING_INSTANCES_NBR];

static void reset_device(void);

#if (USE_CUSTOM_RANGING_VL53L1CB == 1U)
static int32_t VL53L1CB_Probe(uint32_t Instance);
#endif /* Use custom ranging */

/**
  * @brief Initializes the ranging sensor.
  * @param Instance    Ranging sensor instance.
  * @retval BSP status
  */
int32_t CUSTOM_RANGING_SENSOR_Init(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    reset_device();

    switch (Instance)
    {
#if (USE_CUSTOM_RANGING_VL53L1CB == 1U)
      case CUSTOM_VL53L1CB:
        if (VL53L1CB_Probe(Instance) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_NO_INIT;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
        break;
#endif /* Use custom ranging */
      default:
        ret = BSP_ERROR_UNKNOWN_COMPONENT;
        break;
    }
  }

  return ret;
}

/**
  * @brief Deinitializes the ranging sensor.
  * @param Instance    Ranging sensor instance.
  * @retval BSP status
  */
int32_t CUSTOM_RANGING_SENSOR_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->DeInit(CUSTOM_RANGING_CompObj[Instance]) < 0)
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
int32_t CUSTOM_RANGING_SENSOR_ReadID(uint32_t Instance, uint32_t *pId)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->ReadID(CUSTOM_RANGING_CompObj[Instance], pId) < 0)
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
  * @note This function should be called after the init.
  * @retval BSP status
  */
int32_t CUSTOM_RANGING_SENSOR_GetCapabilities(uint32_t Instance, RANGING_SENSOR_Capabilities_t *pCapabilities)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->GetCapabilities(CUSTOM_RANGING_CompObj[Instance], pCapabilities) < 0)
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
int32_t CUSTOM_RANGING_SENSOR_ConfigProfile(uint32_t Instance, RANGING_SENSOR_ProfileConfig_t *pConfig)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->ConfigProfile(CUSTOM_RANGING_CompObj[Instance], pConfig) < 0)
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
  * @note Should be called only if the device supports CustomROI.
  * @retval BSP status
  */
int32_t CUSTOM_RANGING_SENSOR_ConfigROI(uint32_t Instance, RANGING_SENSOR_ROIConfig_t *pConfig)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (RANGING_SENSOR_Cap[Instance].CustomROI == 0)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->ConfigROI(CUSTOM_RANGING_CompObj[Instance], pConfig) < 0)
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
  * @note The threshold modes can be used only if supported by the device (check the capabilities)
  * @retval BSP status
  */
int32_t CUSTOM_RANGING_SENSOR_ConfigIT(uint32_t Instance, RANGING_SENSOR_ITConfig_t *pConfig)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->ConfigIT(CUSTOM_RANGING_CompObj[Instance], pConfig) < 0)
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
int32_t CUSTOM_RANGING_SENSOR_GetDistance(uint32_t Instance, RANGING_SENSOR_Result_t *pResult)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->GetDistance(CUSTOM_RANGING_CompObj[Instance], pResult) < 0)
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
  * @param Mode        The desired ranging mode.
  * @retval BSP status
  */
int32_t CUSTOM_RANGING_SENSOR_Start(uint32_t Instance, uint8_t Mode)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->Start(CUSTOM_RANGING_CompObj[Instance], Mode) < 0)
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
int32_t CUSTOM_RANGING_SENSOR_Stop(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->Stop(CUSTOM_RANGING_CompObj[Instance]) < 0)
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
  * @brief Set The I2C address of the device.
  * @param Instance    Ranging sensor instance.
  * @param Address     New I2C address.
  * @retval BSP status
  */
int32_t CUSTOM_RANGING_SENSOR_SetAddress(uint32_t Instance, uint16_t Address)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->SetAddress(CUSTOM_RANGING_CompObj[Instance], Address) < 0)
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
int32_t CUSTOM_RANGING_SENSOR_GetAddress(uint32_t Instance, uint16_t *pAddress)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->GetAddress(CUSTOM_RANGING_CompObj[Instance], pAddress) < 0)
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
int32_t CUSTOM_RANGING_SENSOR_SetPowerMode(uint32_t Instance, uint32_t PowerMode)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->SetPowerMode(CUSTOM_RANGING_CompObj[Instance], PowerMode) < 0)
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
  * @brief Get the power mode.
  * @param Instance    Ranging sensor instance.
  * @param pPowerMode    Pointer to the current power mode.
  * @retval BSP status
  */
int32_t CUSTOM_RANGING_SENSOR_GetPowerMode(uint32_t Instance, uint32_t *pPowerMode)
{
  int32_t ret;

  if (Instance >= CUSTOM_RANGING_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (CUSTOM_RANGING_Drv[Instance]->GetPowerMode(CUSTOM_RANGING_CompObj[Instance], pPowerMode) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

#if (USE_CUSTOM_RANGING_VL53L1CB == 1U)
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
  static VL53L1CB_Object_t   VL53L1CBObj;

  /* Configure the ranging sensor driver */
  IOCtx.Address     = RANGING_SENSOR_VL53L1CB_ADDRESS;
  IOCtx.Init        = CUSTOM_VL53L1CB_I2C_INIT;
  IOCtx.DeInit      = CUSTOM_VL53L1CB_I2C_DEINIT;
  IOCtx.WriteReg    = CUSTOM_VL53L1CB_I2C_WRITEREG;
  IOCtx.ReadReg     = CUSTOM_VL53L1CB_I2C_READREG;
  IOCtx.GetTick     = BSP_GetTick;

  if (VL53L1CB_RegisterBusIO(&VL53L1CBObj, &IOCtx) != VL53L1CB_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (VL53L1CB_ReadID(&VL53L1CBObj, &id) != VL53L1CB_OK)
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
      CUSTOM_RANGING_Drv[Instance] = (RANGING_SENSOR_Drv_t *) &VL53L1CB_RANGING_SENSOR_Driver;
      CUSTOM_RANGING_CompObj[Instance] = &VL53L1CBObj;

      if (CUSTOM_RANGING_Drv[Instance]->Init(CUSTOM_RANGING_CompObj[Instance]) != VL53L1CB_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else if (CUSTOM_RANGING_Drv[Instance]->GetCapabilities(CUSTOM_RANGING_CompObj[Instance],
                                                             &RANGING_SENSOR_Cap[Instance]) != VL53L1CB_OK)
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
#endif /* Use custom ranging */

static void reset_device(void)
{
#if (USE_CUSTOM_RANGING_VL53L1CB == 1U)
  HAL_GPIO_WritePin(CUSTOM_VL53L1CB_XSHUT_PORT, CUSTOM_VL53L1CB_XSHUT_PIN, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(CUSTOM_VL53L1CB_XSHUT_PORT, CUSTOM_VL53L1CB_XSHUT_PIN, GPIO_PIN_SET);
  HAL_Delay(2);
#endif /* Use custom ranging */
}
