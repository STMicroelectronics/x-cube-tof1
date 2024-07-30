/**
  ******************************************************************************
  * @file          : app_tof.c
  * @author        : IMG SW Application Team
  * @brief         : This file provides code for the configuration
  *                  of the STMicroelectronics.X-CUBE-TOF1.3.4.2 instances.
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
/* uncomment following to use directly the bare driver instead of the BSP */
/* #define USE_BARE_DRIVER */
#define TIMING_BUDGET (30U) /* 5 ms < TimingBudget < 100 ms */
#define RANGING_FREQUENCY (10U) /* Ranging frequency Hz (shall be consistent with TimingBudget value) */
#define POLLING_PERIOD (1)

/* Private variables ---------------------------------------------------------*/
#ifndef USE_BARE_DRIVER
static RANGING_SENSOR_Capabilities_t Cap;
#endif
static RANGING_SENSOR_ProfileConfig_t Profile;

static int32_t status = 0;
volatile uint8_t ToF_EventDetected = 0;

/* Private function prototypes -----------------------------------------------*/
#ifdef USE_BARE_DRIVER
static uint8_t map_target_status(uint8_t status);
static int32_t convert_data_format(VL53L8CX_Object_t *pObj,
                                   VL53L8CX_ResultsData *data, RANGING_SENSOR_Result_t *pResult);
#endif
static void MX_VL53L8CX_SimpleRanging_Init(void);
static void MX_VL53L8CX_SimpleRanging_Process(void);
static void print_result(RANGING_SENSOR_Result_t *Result);
static void toggle_resolution(void);
static void toggle_signal_and_ambient(void);
static void clear_screen(void);
static void display_commands_banner(void);
static void handle_cmd(uint8_t cmd);
static uint8_t get_key(void);
static uint32_t com_has_data(void);

void MX_TOF_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN TOF_Init_PreTreatment */

  /* USER CODE END TOF_Init_PreTreatment */

  /* Initialize the peripherals and the TOF components */

  MX_VL53L8CX_SimpleRanging_Init();

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

  MX_VL53L8CX_SimpleRanging_Process();

  /* USER CODE BEGIN TOF_Process_PostTreatment */

  /* USER CODE END TOF_Process_PostTreatment */
}

static void MX_VL53L8CX_SimpleRanging_Init(void)
{
  /* Initialize Virtual COM Port */
  BSP_COM_Init(COM1);

  printf("\033[2H\033[2J");
  printf("Sensor initialization...\n");

  status = CUSTOM_RANGING_SENSOR_Init(CUSTOM_VL53L8CX);

  if (status != BSP_ERROR_NONE)
  {
    printf("CUSTOM_RANGING_SENSOR_Init failed\n");
    while (1);
  }
}

#ifdef USE_BARE_DRIVER
static void MX_VL53L8CX_SimpleRanging_Process(void)
{
  static RANGING_SENSOR_Result_t Result;
  VL53L8CX_Object_t *pL5obj = CUSTOM_RANGING_CompObj[CUSTOM_VL53L8CX];
  static VL53L8CX_ResultsData data;
  uint8_t NewDataReady = 0;

  Profile.RangingProfile = RS_PROFILE_4x4_CONTINUOUS;
  Profile.TimingBudget = TIMING_BUDGET;
  Profile.Frequency = RANGING_FREQUENCY; /* Ranging frequency Hz (shall be consistent with TimingBudget value) */
  Profile.EnableAmbient = 0; /* Enable: 1, Disable: 0 */
  Profile.EnableSignal = 0; /* Enable: 1, Disable: 0 */

  pL5obj->IsAmbientEnabled = Profile.EnableAmbient;
  pL5obj->IsSignalEnabled = Profile.EnableSignal;

  /*
     use case VL53L8CX_PROFILE_4x4_CONTINUOUS:
  */
  status = vl53l8cx_set_resolution(&(pL5obj->Dev), VL53L8CX_RESOLUTION_4X4);
  status |= vl53l8cx_set_ranging_mode(&(pL5obj->Dev), VL53L8CX_RANGING_MODE_CONTINUOUS);
  status |= vl53l8cx_set_integration_time_ms(&(pL5obj->Dev), TIMING_BUDGET);
  status |= vl53l8cx_set_ranging_frequency_hz(&(pL5obj->Dev), RANGING_FREQUENCY);

  if (status != VL53L8CX_STATUS_OK)
  {
    printf("ERROR : Configuration programming error!\n\n");
    while (1);
  }

  status = vl53l8cx_start_ranging(&(pL5obj->Dev));
  if (status != VL53L8CX_STATUS_OK)
  {
    printf("vl53l8cx_start_ranging failed\n");
    while (1);
  }

  while (1)
  {
    /* polling mode */
    (void)vl53l8cx_check_data_ready(&(pL5obj->Dev), &NewDataReady);

    if (NewDataReady != 0)
    {
      status = vl53l8cx_get_ranging_data(&(pL5obj->Dev), &data);

      if (status == VL53L8CX_STATUS_OK)
      {
        /*
         Convert the data format to Result format.
         Note that you can print directly from data format
        */
        if (convert_data_format(pL5obj, &data, &Result) < 0)
        {
          printf("convert_data_format failed\n");
          while (1);
        }
        print_result(&Result);
      }
    }

    if (com_has_data())
    {
      handle_cmd(get_key());
    }

    HAL_Delay(POLLING_PERIOD);
  }
}
#else
static void MX_VL53L8CX_SimpleRanging_Process(void)
{
  uint32_t Id;
  static RANGING_SENSOR_Result_t Result;

  CUSTOM_RANGING_SENSOR_ReadID(CUSTOM_VL53L8CX, &Id);
  CUSTOM_RANGING_SENSOR_GetCapabilities(CUSTOM_VL53L8CX, &Cap);

  Profile.RangingProfile = RS_PROFILE_4x4_CONTINUOUS;
  Profile.TimingBudget = TIMING_BUDGET;
  Profile.Frequency = RANGING_FREQUENCY; /* Ranging frequency Hz (shall be consistent with TimingBudget value) */
  Profile.EnableAmbient = 0; /* Enable: 1, Disable: 0 */
  Profile.EnableSignal = 0; /* Enable: 1, Disable: 0 */

  /* set the profile if different from default one */
  CUSTOM_RANGING_SENSOR_ConfigProfile(CUSTOM_VL53L8CX, &Profile);

  status = CUSTOM_RANGING_SENSOR_Start(CUSTOM_VL53L8CX, RS_MODE_BLOCKING_CONTINUOUS);

  while (1)
  {
    /* polling mode */
    status = CUSTOM_RANGING_SENSOR_GetDistance(CUSTOM_VL53L8CX, &Result);

    if (status == BSP_ERROR_NONE)
    {
      print_result(&Result);
    }

    if (com_has_data())
    {
      handle_cmd(get_key());
    }

    HAL_Delay(POLLING_PERIOD);
  }
}
#endif /* USE_BARE_DRIVER */

static void print_result(RANGING_SENSOR_Result_t *Result)
{
  int8_t i;
  int8_t j;
  int8_t k;
  int8_t l;
  uint8_t zones_per_line;

  zones_per_line = ((Profile.RangingProfile == RS_PROFILE_8x8_AUTONOMOUS) ||
                    (Profile.RangingProfile == RS_PROFILE_8x8_CONTINUOUS)) ? 8 : 4;

  display_commands_banner();

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
        {
          if ((long)Result->ZoneResult[j + k].Distance[l] < 500)
          {
            printf("| \033[38;5;9m%5ld\033[0m  :  %5ld ",
                   (long)Result->ZoneResult[j + k].Distance[l],
                   (long)Result->ZoneResult[j + k].Status[l]);
          }
          else
          {
            printf("| \033[38;5;10m%5ld\033[0m  :  %5ld ",
                   (long)Result->ZoneResult[j + k].Distance[l],
                   (long)Result->ZoneResult[j + k].Status[l]);
          }
        }
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

static void toggle_resolution(void)
{
  CUSTOM_RANGING_SENSOR_Stop(CUSTOM_VL53L8CX);

  switch (Profile.RangingProfile)
  {
    case RS_PROFILE_4x4_AUTONOMOUS:
      Profile.RangingProfile = RS_PROFILE_8x8_AUTONOMOUS;
      break;

    case RS_PROFILE_4x4_CONTINUOUS:
      Profile.RangingProfile = RS_PROFILE_8x8_CONTINUOUS;
      break;

    case RS_PROFILE_8x8_AUTONOMOUS:
      Profile.RangingProfile = RS_PROFILE_4x4_AUTONOMOUS;
      break;

    case RS_PROFILE_8x8_CONTINUOUS:
      Profile.RangingProfile = RS_PROFILE_4x4_CONTINUOUS;
      break;

    default:
      break;
  }

  CUSTOM_RANGING_SENSOR_ConfigProfile(CUSTOM_VL53L8CX, &Profile);
  CUSTOM_RANGING_SENSOR_Start(CUSTOM_VL53L8CX, RS_MODE_BLOCKING_CONTINUOUS);
}

static void toggle_signal_and_ambient(void)
{
  CUSTOM_RANGING_SENSOR_Stop(CUSTOM_VL53L8CX);

  Profile.EnableAmbient = (Profile.EnableAmbient) ? 0U : 1U;
  Profile.EnableSignal = (Profile.EnableSignal) ? 0U : 1U;

  CUSTOM_RANGING_SENSOR_ConfigProfile(CUSTOM_VL53L8CX, &Profile);
  CUSTOM_RANGING_SENSOR_Start(CUSTOM_VL53L8CX, RS_MODE_BLOCKING_CONTINUOUS);
}

static void clear_screen(void)
{
  /* clear Screen : 27 is ESC command */
  printf("%c[2J", 27);
}

static void display_commands_banner(void)
{
  /* clear screen */
  printf("%c[2H", 27);

  printf("VL53L8CX Simple Ranging demo application\n");
#ifdef USE_BARE_DRIVER
  printf("Using direct calls to VL53L8CX bare driver API\n");
#endif
  printf("Polling mode\n");
  printf("----------------------------------------\n\n");

  printf("Use the following keys to control application\n");
  printf(" 'r' : change resolution\n");
  printf(" 's' : enable signal and ambient\n");
  printf(" 'c' : clear screen\n");
  printf("\n");
}

static void handle_cmd(uint8_t cmd)
{
  switch (cmd)
  {
    case 'r':
      toggle_resolution();
      clear_screen();
      break;

    case 's':
      toggle_signal_and_ambient();
      clear_screen();
      break;

    case 'c':
      clear_screen();
      break;

    default:
      break;
  }
}

static uint8_t get_key(void)
{
  uint8_t cmd = 0;

  HAL_UART_Receive(&hcom_uart[COM1], &cmd, 1, HAL_MAX_DELAY);

  return cmd;
}

static uint32_t com_has_data(void)
{
  return __HAL_UART_GET_FLAG(&hcom_uart[COM1], UART_FLAG_RXNE);;
}

#ifdef USE_BARE_DRIVER
static uint8_t map_target_status(uint8_t status)
{
  uint8_t ret;

  if ((status == 5U) || (status == 9U))
  {
    ret = 0U; /* ranging is OK */
  }
  else if (status == 0U)
  {
    ret = 255U; /* no update */
  }
  else
  {
    ret = status; /* return device status otherwise */
  }

  return ret;
}

static int32_t convert_data_format(VL53L8CX_Object_t *pObj,
                                   VL53L8CX_ResultsData *data, RANGING_SENSOR_Result_t *pResult)
{
  int32_t ret;
  uint8_t i, j;
  uint8_t resolution;
  uint8_t target_status;

  if ((pObj == NULL) || (pResult == NULL))
  {
    ret = VL53L8CX_INVALID_PARAM;
  }
  else if (vl53l8cx_get_resolution(&pObj->Dev, &resolution) != VL53L8CX_STATUS_OK)
  {
    ret = VL53L8CX_ERROR;
  }
  else
  {
    pResult->NumberOfZones = resolution;

    for (i = 0; i < resolution; i++)
    {
      pResult->ZoneResult[i].NumberOfTargets = data->nb_target_detected[i];

      for (j = 0; j < data->nb_target_detected[i]; j++)
      {
        pResult->ZoneResult[i].Distance[j] = (uint32_t)data->distance_mm[(VL53L8CX_NB_TARGET_PER_ZONE * i) + j];

        /* return Ambient value if ambient rate output is enabled */
        if (pObj->IsAmbientEnabled == 1U)
        {
          /* apply ambient value to all targets in a given zone */
          pResult->ZoneResult[i].Ambient[j] = (float_t)data->ambient_per_spad[i];
        }
        else
        {
          pResult->ZoneResult[i].Ambient[j] = 0.0f;
        }

        /* return Signal value if signal rate output is enabled */
        if (pObj->IsSignalEnabled == 1U)
        {
          pResult->ZoneResult[i].Signal[j] =
            (float_t)data->signal_per_spad[(VL53L8CX_NB_TARGET_PER_ZONE * i) + j];
        }
        else
        {
          pResult->ZoneResult[i].Signal[j] = 0.0f;
        }

        target_status = data->target_status[(VL53L8CX_NB_TARGET_PER_ZONE * i) + j];
        pResult->ZoneResult[i].Status[j] = map_target_status(target_status);
      }
    }

    ret = VL53L8CX_OK;
  }

  return ret;
}
#endif

#ifdef __cplusplus
}
#endif
