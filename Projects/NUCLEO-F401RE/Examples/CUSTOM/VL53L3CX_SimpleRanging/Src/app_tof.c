/**
  ******************************************************************************
  * @file          : app_tof.c
  * @author        : IMG SW Application Team
  * @brief         : This file provides code for the configuration
  *                  of the STMicroelectronics.X-CUBE-TOF1.3.4.1 instances.
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

#include "custom_ranging_sensor.h"
#include "stm32f4xx_nucleo.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIMING_BUDGET (30U) /* 16 ms < TimingBudget < 500 ms */
#define POLLING_PERIOD (250U) /* refresh rate for polling mode (ms, shall be consistent with TimingBudget value) */

/* Private variables ---------------------------------------------------------*/
static RANGING_SENSOR_Capabilities_t Cap;
static RANGING_SENSOR_ProfileConfig_t Profile;
static RANGING_SENSOR_Result_t Result;
static int32_t status = 0;
volatile uint8_t ToF_EventDetected = 0;

/* Private function prototypes -----------------------------------------------*/
static void MX_VL53L3CX_SimpleRanging_Init(void);
static void MX_VL53L3CX_SimpleRanging_Process(void);
static void print_result(RANGING_SENSOR_Result_t *Result);
static int32_t decimal_part(float_t x);

void MX_TOF_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN TOF_Init_PreTreatment */

  /* arbitrary wait after XShutdown to allow switch on */
  HAL_Delay(1);

  /* USER CODE END TOF_Init_PreTreatment */

  /* Initialize the peripherals and the TOF components */

  MX_VL53L3CX_SimpleRanging_Init();

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

  MX_VL53L3CX_SimpleRanging_Process();

  /* USER CODE BEGIN TOF_Process_PostTreatment */

  /* USER CODE END TOF_Process_PostTreatment */
}

static void MX_VL53L3CX_SimpleRanging_Init(void)
{
  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  printf("VL53L3CX Simple Ranging demo application\n");
  status = CUSTOM_RANGING_SENSOR_Init(CUSTOM_VL53L3CX);

  if (status != BSP_ERROR_NONE)
  {
    printf("CUSTOM_RANGING_SENSOR_Init failed\n");
    while (1);
  }
}

static void MX_VL53L3CX_SimpleRanging_Process(void)
{
  uint32_t Id;

  CUSTOM_RANGING_SENSOR_ReadID(CUSTOM_VL53L3CX, &Id);
  CUSTOM_RANGING_SENSOR_GetCapabilities(CUSTOM_VL53L3CX, &Cap);

  Profile.RangingProfile = RS_MULTI_TARGET_MEDIUM_RANGE;
  Profile.TimingBudget = TIMING_BUDGET;
  Profile.Frequency = 0; /* Induces intermeasurement period, set to ZERO for normal ranging */
  Profile.EnableAmbient = 1; /* Enable: 1, Disable: 0 */
  Profile.EnableSignal = 1; /* Enable: 1, Disable: 0 */

  /* set the profile if different from default one */
  CUSTOM_RANGING_SENSOR_ConfigProfile(CUSTOM_VL53L3CX, &Profile);

  status = CUSTOM_RANGING_SENSOR_Start(CUSTOM_VL53L3CX, RS_MODE_BLOCKING_CONTINUOUS);

  while (1)
  {
    /* polling mode */
    status = CUSTOM_RANGING_SENSOR_GetDistance(CUSTOM_VL53L3CX, &Result);

    if (status == BSP_ERROR_NONE)
    {
      print_result(&Result);
    }

    HAL_Delay(POLLING_PERIOD);
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
