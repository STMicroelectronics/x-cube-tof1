/**
  ******************************************************************************
  * @file    53l1a2.c
  * @author  IMG SW Application Team
  * @brief   This file contains the X-NUCLEO-53L1A2 BSP implementation.
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
#include <string.h>
#include "53l1a2.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup XNUCLEO_53L1A2
  * @{
  */

/** @addtogroup XNUCLEO_53L1A2_COMMON
  * @{
  */

/* This macro can be overloaded by the user to report error log messages with printf format */
#define VL53L1A2_ErrLog(...) (void)0

/* These macros can be overloaded by the user to enforce i2c sharing in RTOS context */
#define VL53L1A2_GetI2cBus(...) (void)0
#define VL53L1A2_PutI2cBus(...) (void)0

#define I2C_EXPANDER_ADDR0 ((int)(0x43*2))   /*!< Expander 0 i2c address[7..0] format */
#define I2C_EXPANDER_ADDR1 ((int)(0x42*2))   /*!< Expander 1 i2c address[7..0] format */

/**
  * GPIO monitor pin state register
  * 16 bit register LSB at lowest offset (little endian)
  */
#define GPMR    (0x10)
/**
  * STMPE1600 GPIO set pin state register
  * 16 bit register LSB at lowest offset (little endian)
  */
#define GPSR    (0x12)
/**
  * STMPE1600 GPIO set pin direction register
  * 16 bit register LSB at lowest offset
  */
#define GPDR    (0x14)

/**
  * @}
  */

/** @defgroup XNUCLEO_53L1A2_COMMON_Private_Variables Private Variables
  * @{
  */
static uint32_t InitCounter = 0;

/* cache the full set of expanded GPIO values to avoid i2c reading */
static union CurIOVal_u
{
  uint8_t bytes[4];   /*!<  4 bytes array i/o view */
  uint32_t u32;       /*!<  single dword i/o view */
} CurIOVal; /* cache the extended IO values */
/**
  * @}
  */

/** @defgroup XNUCLEO_53L1A2_COMMON_Private_Functions_Prototypes Private Functions Prototypes
  * @{
  */
static int32_t _I2cFailRecover(void);
static int32_t _ExpanderRd(uint32_t I2cExpAddr, uint32_t index, uint8_t *data, uint32_t n_data);
static int32_t _ExpanderWR(uint32_t I2cExpAddr, uint32_t index, uint8_t *data, uint32_t n_data);
static int32_t _ExpandersSetAllIO(void);
/**
  * @}
  */

/**
  * @brief Initialize X-NUCLEO-53L1A2 STM32 expansion board
  * @note All devices XSDN are asserted and display is turned off
  * @return 0 on success
  */
int32_t VL53L1A2_Init(void)
{
  int32_t status = 0;
  uint8_t ExpanderData[2];

  if (InitCounter++ == 0U)
  {
    status |= _I2cFailRecover();
    status |= VL53L1A2_I2C_INIT();

    if (status != BSP_ERROR_NONE)
    {
      goto done_err;
    }

	  status = _ExpanderRd(I2C_EXPANDER_ADDR0, 0, ExpanderData, 2);

	  if ((status != 0) || (ExpanderData[0] != 0x00U) || (ExpanderData[1] != 0x16U))
	  {
      VL53L1A2_ErrLog("I2C Expander @0x%02X not detected", (int)I2C_EXPANDER_ADDR0);
      goto done_err;
	  }

	  status = _ExpanderRd(I2C_EXPANDER_ADDR1, 0, ExpanderData, 2);

	  if ((status != 0) || (ExpanderData[0] != 0x00U) || (ExpanderData[1] != 0x16U))
	  {
		  VL53L1A2_ErrLog("I2C Expander @0x%02X not detected", (int)I2C_EXPANDER_ADDR1);
		  goto done_err;
	  }

	  CurIOVal.u32 = 0x0U;

	  /* setup expander   i/o direction  all output but exp1 bit 14*/
	  ExpanderData[0] = 0xFFU;
	  ExpanderData[1] = 0xFFU;

	  status = _ExpanderWR(I2C_EXPANDER_ADDR0, GPDR, ExpanderData, 2);

	  if (status)
	  {
		VL53L1A2_ErrLog("Set Expander @0x%02X DR", I2C_EXPANDER_ADDR0);
		goto done_err;
	  }

	  ExpanderData[0] = 0xFFU;
	  ExpanderData[1] = 0xBFU; /* all but bit 14-15 that is pb1 and xhurt */

	  status = _ExpanderWR(I2C_EXPANDER_ADDR1, GPDR, ExpanderData, 2);

	  if (status)
	  {
		VL53L1A2_ErrLog("Set Expander @0x%02X DR", I2C_EXPANDER_ADDR1);
		goto done_err;
	  }

	  /* shut down all segment and all device */
	  CurIOVal.u32 = 0x7FU + (0x7FU << 7) + (0x7FU << 16) + (0x7FU << (16 + 7));

	  status = _ExpandersSetAllIO();

	  if (status)
	  {
		VL53L1A2_ErrLog("Set initial i/o ");
	  }
  }

done_err:
  return status;
}

/**
  * @brief De-initialize X-NUCLEO-53L1A2 STM32 expansion board
  * @return 0 on success
  */
int32_t VL53L1A2_DeInit(void)
{
  int32_t status = 0;

  if (InitCounter > 0U)
  {
	if (--InitCounter == 0U)
	{
      status = VL53L1A2_I2C_DEINIT();
	}
  }

  return status;
}

/**
  * @brief Set Reset (XSDN) state of a given "id" device
  * @param  DevNo The device number, use @ref VL53L1A2_dev_e.
  * @param  state  State of the device reset (xsdn) pin @warning reset pin is active low
  * @return 0 on success
  */
int32_t VL53L1A2_ResetId(uint8_t DevNo, uint8_t state)
{
  int32_t status;

  switch (DevNo)
  {
    case VL53L1A2_DEV_CENTER :
      CurIOVal.bytes[3] &= ~0x80U; /* bit 15 expander 1  => byte #3 */

      if (state)
      {
        CurIOVal.bytes[3] |= 0x80U;  /* bit 15 expander 1  => byte #3 */
      }

      status = _ExpanderWR(I2C_EXPANDER_ADDR1, GPSR + 1, &CurIOVal.bytes[3], 1);
      break;

    case VL53L1A2_DEV_LEFT :
      CurIOVal.bytes[1] &= ~0x40U; /* bit 14 expander 0 => byte #1*/

      if (state)
      {
        CurIOVal.bytes[1] |= 0x40U;  /* bit 14 expander 0 => byte #1*/
      }

      status = _ExpanderWR(I2C_EXPANDER_ADDR0, GPSR + 1, &CurIOVal.bytes[1], 1);
      break;

    case VL53L1A2_DEV_RIGHT :
      CurIOVal.bytes[1] &= ~0x80U; /* bit 15 expander 0  => byte #1 */

      if (state)
      {
        CurIOVal.bytes[1] |= 0x80U;  /* bit 15 expander 0 => byte #1*/
      }

      status = _ExpanderWR(I2C_EXPANDER_ADDR0, GPSR + 1, &CurIOVal.bytes[1], 1);
      break;

    default:
      VL53L1A2_ErrLog("Invalid DevNo %d", DevNo);
      status = -1;
      goto done;
  }

  /* error with valid id */
  if (status)
  {
    VL53L1A2_ErrLog("expander i/o error for DevNo %d state %d ", DevNo, state);
  }

done:
  return status;
}

/** @defgroup XNUCLEO_53L1A2_COMMON_Private_Functions Private Functions
  * @{
  */

/**
  * @brief Expansion board i2c bus recovery
  * We may get reset in middle of an i2c access (h/w reset button, debug or f/w load)
  * hence some agent on bus may be in middle of a transaction and can create issue or even prevent starting (SDA is low)
  * this routine does use gpio to manipulate and recover i2c bus line in all cases.
  */
static int32_t _I2cFailRecover(void)
{
  /* We can't assume bus state based on SDA and SCL state (we may be in a data or NAK bit so SCL=SDA=1)
  * by setting SDA high and toggling SCL at least 10 time we ensure whatever agent and state
  * all agent should end up seeing a "stop" and bus get back to an known idle i2c  bus state */

  uint8_t i;
  uint8_t retry_cnt = 0;
  static uint8_t is_already_init = 0U;
  GPIO_InitTypeDef GPIO_InitStruct;

  if (is_already_init == 1U)
  {
    return BSP_ERROR_NONE;
  }

  /* Enable I/O */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = VL53L1A2_I2C_SCL_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(VL53L1A2_I2C_SCL_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = VL53L1A2_I2C_SDA_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(VL53L1A2_I2C_SDA_GPIO_PORT, &GPIO_InitStruct);

  HAL_GPIO_WritePin(VL53L1A2_I2C_SCL_GPIO_PORT, VL53L1A2_I2C_SCL_GPIO_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(VL53L1A2_I2C_SDA_GPIO_PORT, VL53L1A2_I2C_SDA_GPIO_PIN, GPIO_PIN_SET);

  do
  {
    for (i = 0; i < 10U; i++)
    {
      HAL_GPIO_WritePin(VL53L1A2_I2C_SCL_GPIO_PORT, VL53L1A2_I2C_SCL_GPIO_PIN, GPIO_PIN_RESET);
      HAL_Delay(1);
      HAL_GPIO_WritePin(VL53L1A2_I2C_SCL_GPIO_PORT, VL53L1A2_I2C_SCL_GPIO_PIN, GPIO_PIN_SET);
      HAL_Delay(1);
    }
    retry_cnt++;
  } while ((HAL_GPIO_ReadPin(VL53L1A2_I2C_SDA_GPIO_PORT, VL53L1A2_I2C_SDA_GPIO_PIN) == GPIO_PIN_RESET) && (retry_cnt < 7U));

  if (HAL_GPIO_ReadPin(VL53L1A2_I2C_SCL_GPIO_PORT, VL53L1A2_I2C_SDA_GPIO_PIN) == GPIO_PIN_RESET)
  {
    /* We are still in a bad i2c state, return error */
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  is_already_init = 1U;

  return BSP_ERROR_NONE;
}

/**
  * @brief Set all i2c expended gpio in one go
  * @return i/o operation status
  */
static int32_t _ExpandersSetAllIO(void)
{
  int32_t status;

  status = _ExpanderWR(I2C_EXPANDER_ADDR0, GPSR, &CurIOVal.bytes[0], 2);

  if (status)
  {
    goto done_err;
  }

  status = _ExpanderWR(I2C_EXPANDER_ADDR1, GPSR, &CurIOVal.bytes[2], 2);

done_err:
  return status;
}

/**
  * @brief STMPE1600 i2c Expander register read
  * @param I2cExpAddr Expander address
  * @param index      register index
  * @param data       read data buffer
  * @param n_data     number of byte to read
  * @return           of if ok else i2c I/O operation status
  */
static int32_t _ExpanderRd(uint32_t I2cExpAddr, uint32_t index, uint8_t *data, uint32_t n_data)
{
  int32_t status;
  uint8_t RegAddr;

  RegAddr = index;
  VL53L1A2_GetI2cBus();

  do
  {
    status = HAL_I2C_Master_Transmit(&VL53L1A2_HI2C, I2cExpAddr, &RegAddr, 1, 100);

    if (status)
    {
      break;
    }

    status = HAL_I2C_Master_Receive(&VL53L1A2_HI2C, I2cExpAddr, data, n_data, n_data * 100);
  } while (0);

  VL53L1A2_PutI2cBus();

  return status;
}

/**
  * @brief STMPE1600 i2c Expander register write
  * @param I2cExpAddr Expander address
  * @param index      register index
  * @param data       data buffer
  * @param n_data     number of byte to write
  * @return           of if ok else i2c I/O operation status
  */
static int32_t _ExpanderWR(uint32_t I2cExpAddr, uint32_t index, uint8_t *data, uint32_t n_data)
{
  int32_t status;
  uint8_t RegAddr[0x10];

  RegAddr[0] = index;
  memcpy(RegAddr + 1, data, n_data);

  VL53L1A2_GetI2cBus();
  status = HAL_I2C_Master_Transmit(&VL53L1A2_HI2C, I2cExpAddr, RegAddr, n_data + 1, 100);
  VL53L1A2_PutI2cBus();

  return status;
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
