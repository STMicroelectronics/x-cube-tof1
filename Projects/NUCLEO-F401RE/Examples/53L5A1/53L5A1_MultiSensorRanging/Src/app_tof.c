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

#include "53l5a1_ranging_sensor.h"
#include "app_tof_pin_conf.h"
#include "stm32f4xx_nucleo.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIMING_BUDGET (30U) /* 5 ms < TimingBudget < 100 ms */
#define RANGING_FREQUENCY (1U) /* Ranging frequency Hz (shall be consistent with TimingBudget value) */
#define POLLING_PERIOD (1000U/RANGING_FREQUENCY) /* refresh rate for polling mode (milliseconds) */

/* Private variables ---------------------------------------------------------*/
static RANGING_SENSOR_ProfileConfig_t Profile;
static RANGING_SENSOR_Result_t Result;

static int32_t status = 0;
static uint8_t ToF_Present[RANGING_SENSOR_INSTANCES_NBR] = {0};
volatile uint8_t ToF_EventDetected = 0;

static const char *TofDevStr[] =
{
  [VL53L5A1_DEV_LEFT] = "LEFT",
  [VL53L5A1_DEV_CENTER] = "CENTER",
  [VL53L5A1_DEV_RIGHT] = "RIGHT"
};

/* Private function prototypes -----------------------------------------------*/
static void MX_53L5A1_MultiSensorRanging_Init(void);
static void MX_53L5A1_MultiSensorRanging_Process(void);

static void print_result(RANGING_SENSOR_Result_t *Result);
static void write_lowpower_pin(uint8_t device, GPIO_PinState pin_state);
static void reset_all_sensors(void);

void MX_TOF_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN TOF_Init_PreTreatment */

  /* USER CODE END TOF_Init_PreTreatment */

  /* Initialize the peripherals and the TOF components */

  MX_53L5A1_MultiSensorRanging_Init();

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

  MX_53L5A1_MultiSensorRanging_Process();

  /* USER CODE BEGIN TOF_Process_PostTreatment */

  /* USER CODE END TOF_Process_PostTreatment */
}

static void MX_53L5A1_MultiSensorRanging_Init(void)
{
  uint8_t device;
  uint16_t i2c_addr;
  uint32_t id;

  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  printf("53L5A1 Multi Sensor Ranging demo application\n");

  reset_all_sensors();

  /* Turn off all the sensors */
  for (device = 0; device < RANGING_SENSOR_INSTANCES_NBR; device++)
  {
    write_lowpower_pin(device, GPIO_PIN_RESET);
  }

  /* initializes each device and put it in low power mode */
  for (device = 0; device < RANGING_SENSOR_INSTANCES_NBR; device++)
  {
    /* enable only one sensor */
    write_lowpower_pin(device, GPIO_PIN_SET);
    HAL_Delay(2);

    status = VL53L5A1_RANGING_SENSOR_Init(device);

    if (status != BSP_ERROR_NONE)
    {
      printf("VL53L5A1_RANGING_SENSOR_Init %d failed\n", device);
      ToF_Present[device] = 0; /* device not detected */
    }
    else
    {
      ToF_Present[device] = 1; /* device detected */
    }

    write_lowpower_pin(device, GPIO_PIN_RESET); /* turn off the device */
  }

  /* power on the devices one at a time and change their address
   * once the address is updated, the communication with the devices is checked
   * reading its ID.
   */
  for (device = 0; device < RANGING_SENSOR_INSTANCES_NBR; device++)
  {
    /* skip the sensor if init not successful */
    if (ToF_Present[device] == 0) { continue; }

    /* turn on the device */
    write_lowpower_pin(device, GPIO_PIN_SET);

    /* left: 0x54, center: 0x56, right: 0x58 */
    i2c_addr = (RANGING_SENSOR_VL53L5CX_ADDRESS + (device + 1) * 2);
    VL53L5A1_RANGING_SENSOR_SetAddress(device, i2c_addr);

    /* check the communication with the device reading the ID */
    VL53L5A1_RANGING_SENSOR_ReadID(device, &id);
    printf("ToF sensor %d - ID: %04lX\n", device, (unsigned long)id);
  }
}

static void MX_53L5A1_MultiSensorRanging_Process(void)
{
  uint8_t i;

  Profile.RangingProfile = RS_PROFILE_4x4_CONTINUOUS;
  Profile.TimingBudget = TIMING_BUDGET;
  Profile.Frequency = RANGING_FREQUENCY; /* Ranging frequency Hz (shall be consistent with TimingBudget value) */
  Profile.EnableAmbient = 0; /* Enable: 1, Disable: 0 */
  Profile.EnableSignal = 0; /* Enable: 1, Disable: 0 */

  for (i = 0; i < RANGING_SENSOR_INSTANCES_NBR; i++)
  {
    /* skip this device if not detected */
    if (ToF_Present[i] != 1) { continue; }

    VL53L5A1_RANGING_SENSOR_ConfigProfile(i, &Profile);
    status = VL53L5A1_RANGING_SENSOR_Start(i, RS_MODE_BLOCKING_CONTINUOUS);

    if (status != BSP_ERROR_NONE)
    {
      printf("VL53L5A1_RANGING_SENSOR_Start %d failed\n", i);
      while (1);
    }
  }

  while (1)
  {
    /* polling mode */
    for (i = 0; i < RANGING_SENSOR_INSTANCES_NBR; i++)
    {
      if (!ToF_Present[i]) { continue; }

      status = VL53L5A1_RANGING_SENSOR_GetDistance(i, &Result);

      if (status == BSP_ERROR_NONE)
      {
        printf("%s\n", TofDevStr[i]);
        print_result(&Result);
        HAL_Delay(POLLING_PERIOD);
      }
    }
  }
}

static void print_result(RANGING_SENSOR_Result_t *Result)
{
  int8_t i;
  int8_t j;
  int8_t k;
  int8_t l;
  uint8_t zones_per_line;

  zones_per_line = ((Profile.RangingProfile == RS_PROFILE_8x8_AUTONOMOUS) ||
                    (Profile.RangingProfile == RS_PROFILE_8x8_CONTINUOUS)) ? 8 : 4;

  printf("Cell Format :\n\n");
  for (l = 0; l < RANGING_SENSOR_NB_TARGET_PER_ZONE; l++)
  {
    printf(" \033[38;5;10m%20s\033[0m : %20s\n", "Distance [mm]", "Status");
    if ((Profile.EnableAmbient != 0) || (Profile.EnableSignal != 0))
    {
      printf(" %20s : %20s\n", "Signal [kcps/spad]", "Ambient [kcps/spad]");
    }
  }

  printf("\n\n");

  for (j = 0; j < Result->NumberOfZones; j += zones_per_line)
  {
    for (i = 0; i < zones_per_line; i++) /* number of zones per line */
    {
      printf(" -----------------");
    }
    printf("\n");

    for (i = 0; i < zones_per_line; i++)
    {
      printf("|                 ");
    }
    printf("|\n");

    for (l = 0; l < RANGING_SENSOR_NB_TARGET_PER_ZONE; l++)
    {
      /* Print distance and status */
      for (k = (zones_per_line - 1); k >= 0; k--)
      {
        if (Result->ZoneResult[j + k].NumberOfTargets > 0)
          printf("| \033[38;5;10m%5ld\033[0m  :  %5ld ",
                 (long)Result->ZoneResult[j + k].Distance[l],
                 (long)Result->ZoneResult[j + k].Status[l]);
        else
          printf("| %5s  :  %5s ", "X", "X");
      }
      printf("|\n");

      if ((Profile.EnableAmbient != 0) || (Profile.EnableSignal != 0))
      {
        /* Print Signal and Ambient */
        for (k = (zones_per_line - 1); k >= 0; k--)
        {
          if (Result->ZoneResult[j + k].NumberOfTargets > 0)
          {
            if (Profile.EnableSignal != 0)
            {
              printf("| %5ld  :  ", (long)Result->ZoneResult[j + k].Signal[l]);
            }
            else
              printf("| %5s  :  ", "X");

            if (Profile.EnableAmbient != 0)
            {
              printf("%5ld ", (long)Result->ZoneResult[j + k].Ambient[l]);
            }
            else
              printf("%5s ", "X");
          }
          else
            printf("| %5s  :  %5s ", "X", "X");
        }
        printf("|\n");
      }
    }
  }

  for (i = 0; i < zones_per_line; i++)
  {
    printf(" -----------------");
  }
  printf("\n");
}

static void write_lowpower_pin(uint8_t device, GPIO_PinState pin_state)
{
  switch (device)
  {
    case VL53L5A1_DEV_CENTER:
      HAL_GPIO_WritePin(VL53L5A1_LPn_C_PORT, VL53L5A1_LPn_C_PIN, pin_state);
      break;

    case VL53L5A1_DEV_LEFT:
      HAL_GPIO_WritePin(VL53L5A1_LPn_L_PORT, VL53L5A1_LPn_L_PIN, pin_state);
      break;

    case VL53L5A1_DEV_RIGHT:
      HAL_GPIO_WritePin(VL53L5A1_LPn_R_PORT, VL53L5A1_LPn_R_PIN, pin_state);
      break;

    default:
      break;
  }
}

static void reset_all_sensors(void)
{
  HAL_GPIO_WritePin(VL53L5A1_PWR_EN_C_PORT, VL53L5A1_PWR_EN_C_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(VL53L5A1_PWR_EN_L_PORT, VL53L5A1_PWR_EN_L_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(VL53L5A1_PWR_EN_R_PORT, VL53L5A1_PWR_EN_R_PIN, GPIO_PIN_RESET);
  HAL_Delay(2);

  HAL_GPIO_WritePin(VL53L5A1_PWR_EN_C_PORT, VL53L5A1_PWR_EN_C_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(VL53L5A1_PWR_EN_L_PORT, VL53L5A1_PWR_EN_L_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(VL53L5A1_PWR_EN_R_PORT, VL53L5A1_PWR_EN_R_PIN, GPIO_PIN_SET);
  HAL_Delay(2);

  HAL_GPIO_WritePin(VL53L5A1_LPn_C_PORT, VL53L5A1_LPn_C_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(VL53L5A1_LPn_L_PORT, VL53L5A1_LPn_L_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(VL53L5A1_LPn_R_PORT, VL53L5A1_LPn_R_PIN, GPIO_PIN_RESET);
  HAL_Delay(2);

  HAL_GPIO_WritePin(VL53L5A1_LPn_C_PORT, VL53L5A1_LPn_C_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(VL53L5A1_LPn_L_PORT, VL53L5A1_LPn_L_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(VL53L5A1_LPn_R_PORT, VL53L5A1_LPn_R_PIN, GPIO_PIN_SET);
  HAL_Delay(2);
}

#ifdef __cplusplus
}
#endif
