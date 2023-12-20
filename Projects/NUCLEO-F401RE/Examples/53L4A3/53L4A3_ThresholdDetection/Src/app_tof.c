/**
  ******************************************************************************
  * @file          : app_tof.c
  * @author        : IMG SW Application Team
  * @brief         : This file provides code for the configuration
  *                  of the STMicroelectronics.X-CUBE-TOF1.3.4.0 instances.
  ******************************************************************************
  *
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

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_tof.h"
#include "main.h"
#include <stdio.h>

#include "53l4a3_ranging_sensor.h"
#include "app_tof_pin_conf.h"
#include "stm32f4xx_nucleo.h"

/*
 * The purpose of this sample application is to showcase the threshold detection
 * functionality of this device.
 *
 * When the device detects a target that match the configuration an IT is generated
 * and the host will start printing the measurement information on the serial connection (UART).
 *
 * Here is the default configuration:
 *
 * ITConfig.Criteria = RS_IT_IN_WINDOW;
 * ITConfig.LowThreshold = 200; // distance in mm
 * ITConfig.HighThreshold = 600; // distance in mm
 *
 * Other availables interrupt generation criteria for this device are:
 *
 * - RS_IT_DEFAULT          // IT if new measurement is ready (no thresholds)
 * - RS_IT_IN_WINDOW        // IT if distance >= LowThreshold AND distance <= HighThreshold
 * - RS_IT_OUT_OF_WINDOW    // IT if distance < LowThreshold OR distance > HighThreshold
 * - RS_IT_BELOW_LOW        // IT if distance < LowThreshold
 * - RS_IT_ABOVE_HIGH       // IT if distance > HighThreshold
 */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIMING_BUDGET (100U) /* 10 ms < TimingBudget < 200 ms */
#define RANGING_FREQUENCY (5U) /* Ranging frequency Hz (shall be consistent with TimingBudget value) */

#define LOW_THRESHOLD  (200U)
#define HIGH_THRESHOLD (600U)

/* Private variables ---------------------------------------------------------*/
static RANGING_SENSOR_Capabilities_t Cap;
static RANGING_SENSOR_ProfileConfig_t Profile;
static int32_t status = 0;
volatile uint8_t ToF_EventDetected = 0;

/* Private function prototypes -----------------------------------------------*/
static void MX_53L4A3_ThresholdDetection_Init(void);
static void MX_53L4A3_ThresholdDetection_Process(void);
static void print_result(RANGING_SENSOR_Result_t *Result);
static int32_t decimal_part(float_t x);

void MX_TOF_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN TOF_Init_PreTreatment */

  /* USER CODE END TOF_Init_PreTreatment */

  /* Initialize the peripherals and the TOF components */

  MX_53L4A3_ThresholdDetection_Init();

  /* USER CODE BEGIN TOF_Init_PostTreatment */

  /* USER CODE END TOF_Init_PostTreatment */
}

/*
 * LM background task
 */
void MX_TOF_Process(void)
{
  /* USER CODE BEGIN TOF_Process_PreTreatment */

  /* USER CODE END TOF_Process_PreTreatment */

  MX_53L4A3_ThresholdDetection_Process();

  /* USER CODE BEGIN TOF_Process_PostTreatment */

  /* USER CODE END TOF_Process_PostTreatment */
}

static void MX_53L4A3_ThresholdDetection_Init(void)
{
  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  /* reset XSHUT (XSDN) pin */
  HAL_GPIO_WritePin(VL53L4A3_XSHUT_C_PORT, VL53L4A3_XSHUT_C_PIN, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(VL53L4A3_XSHUT_C_PORT, VL53L4A3_XSHUT_C_PIN, GPIO_PIN_SET);
  HAL_Delay(2);

  printf("53L4A3 Threshold detection demo application\n");
  status = VL53L4A3_RANGING_SENSOR_Init(VL53L4A3_DEV_CENTER);

  if (status != BSP_ERROR_NONE)
  {
    printf("VL53L4A3_RANGING_SENSOR_Init failed\n");
    while (1);
  }
}

static void MX_53L4A3_ThresholdDetection_Process(void)
{
  uint32_t Id;

  RANGING_SENSOR_ITConfig_t ITConfig;
  RANGING_SENSOR_Result_t Result;

  VL53L4A3_RANGING_SENSOR_ReadID(VL53L4A3_DEV_CENTER, &Id);
  VL53L4A3_RANGING_SENSOR_GetCapabilities(VL53L4A3_DEV_CENTER, &Cap);

  Profile.RangingProfile = VL53L4ED_PROFILE_AUTONOMOUS;
  Profile.TimingBudget = TIMING_BUDGET;
  Profile.Frequency = RANGING_FREQUENCY; /* Ranging frequency Hz (shall be consistent with TimingBudget value) */
  Profile.EnableAmbient = 0; /* Enable: 1, Disable: 0 */
  Profile.EnableSignal = 0; /* Enable: 1, Disable: 0 */

  /* set the profile if different from default one */
  VL53L4A3_RANGING_SENSOR_ConfigProfile(VL53L4A3_DEV_CENTER, &Profile);

  /* threshold parameters */
  ITConfig.Criteria = RS_IT_IN_WINDOW;
  ITConfig.LowThreshold = LOW_THRESHOLD; /* mm */
  ITConfig.HighThreshold = HIGH_THRESHOLD; /* mm */

  VL53L4A3_RANGING_SENSOR_ConfigIT(VL53L4A3_DEV_CENTER, &ITConfig);

  status = VL53L4A3_RANGING_SENSOR_Start(VL53L4A3_DEV_CENTER, RS_MODE_ASYNC_CONTINUOUS);

  if (status != BSP_ERROR_NONE)
  {
    printf("VL53L4A3_RANGING_SENSOR_Start failed\n");
    while (1);
  }

  while (1)
  {
    /* interrupt mode */
    if (ToF_EventDetected != 0)
    {
      ToF_EventDetected = 0;

      status = VL53L4A3_RANGING_SENSOR_GetDistance(VL53L4A3_DEV_CENTER, &Result);

      if (status == BSP_ERROR_NONE)
      {
        print_result(&Result);
      }
    }
  }
}

static void print_result(RANGING_SENSOR_Result_t *Result)
{
  uint8_t i;
  uint8_t j;

  for (i = 0; i < RANGING_SENSOR_MAX_NB_ZONES; i++)
  {
    printf("\nTargets = %lu", (unsigned long)Result->ZoneResult[i].NumberOfTargets);

    for (j = 0; j < Result->ZoneResult[i].NumberOfTargets; j++)
    {
      printf("\n |---> ");

      printf("Status = %ld, Distance = %5ld mm ",
             (long)Result->ZoneResult[i].Status[j],
             (long)Result->ZoneResult[i].Distance[j]);

      if (Profile.EnableAmbient)
        printf(", Ambient = %ld.%02ld kcps/spad",
               (long)Result->ZoneResult[i].Ambient[j],
               (long)decimal_part(Result->ZoneResult[i].Ambient[j]));

      if (Profile.EnableSignal)
        printf(", Signal = %ld.%02ld kcps/spad",
               (long)Result->ZoneResult[i].Signal[j],
               (long)decimal_part(Result->ZoneResult[i].Signal[j]));
    }
  }
  printf("\n");
}

static int32_t decimal_part(float_t x)
{
  int32_t int_part = (int32_t) x;
  return (int32_t)((x - int_part) * 100);
}

#ifdef __cplusplus
}
#endif
