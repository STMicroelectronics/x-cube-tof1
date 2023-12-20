/**
  ******************************************************************************
  * @file    53l3a2.c
  * @author  IMG SW Application Team
  * @brief   This file contains the X-NUCLEO-53L3A2 BSP implementation.
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
#include "53l3a2.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup XNUCLEO_53L3A2
  * @{
  */

/** @addtogroup XNUCLEO_53L3A2_COMMON
  * @{
  */

/* This macro can be overloaded by the user to report error log messages with printf format */
#define VL53L3A2_ErrLog(...) (void)0

/* These macros can be overloaded by the user to enforce i2c sharing in RTOS context */
#define VL53L3A2_GetI2cBus(...) (void)0
#define VL53L3A2_PutI2cBus(...) (void)0

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
  * @defgroup VL53L3A2_7Segment 7 segment display
  *
  * macro use for human readable segment building
  * @code
  *  --s0--
  *  s    s
  *  5    1
  *  --s6--
  *  s    s
  *  4    2
  *  --s3-- . s7 (dp)
  * @endcode
  *
  * @{
  */
#define DP (1U << 7)   /*!< decimal point bit mapping */
#define S0 (1U << 3)   /*!< segment s0 bit mapping */
#define S1 (1U << 5)   /*!< segment s1 bit mapping */
#define S2 (1U << 6)   /*!< segment s2 bit mapping */
#define S3 (1U << 4)   /*!< segment s3 bit mapping */
#define S4 (1U << 0)   /*!< segment s4 bit mapping */
#define S5 (1U << 1)   /*!< segment s5 bit mapping */
#define S6 (1U << 2)   /*!< segment s6 bit mapping */

/**
  * build a character by defining the non lighted segment (not one and no DP)
  *
  * @param  ... literal sum and or combine of any macro to define any segment #S0 .. #S6
  *
  * example '9' is all segment on but S4
  * @code
  *   ['9']=           NOT_7_NO_DP(S4),
  * @endcode
  */
#define NOT_7_NO_DP( ... ) (uint8_t) ~( __VA_ARGS__ + DP )

/**
  * Ascii to 7 segment  lookup table
  *
  * Most common character are supported and follow http://www.twyman.org.uk/Fonts/
  * few extra special \@ ^~ ... etc are present for specific demo purpose
  */
static const uint8_t ascii_to_display_lut[256] =
{
  [' '] =           0,
  ['-'] =           S6,
  ['_'] =           S3,
  ['='] =           S3 + S6,
  ['~'] =           S0 + S3 + S6, /* 3 h bar */
  ['^'] =           S0, /* use as top bar */

  ['?'] =           NOT_7_NO_DP(S5 + S3 + S2),
  ['*'] =           NOT_7_NO_DP(),
  ['['] =           S0 + S3 + S4 + S5,
  [']'] =           S0 + S3 + S2 + S1,
  ['@'] =           S0 + S3,

  ['0'] =           NOT_7_NO_DP(S6),
  ['1'] =           S1 + S2,
  ['2'] =           S0 + S1 + S6 + S4 + S3,
  ['3'] =           NOT_7_NO_DP(S4 + S5),
  ['4'] =           S5 + S1 + S6 + S2,
  ['5'] =           NOT_7_NO_DP(S1 + S4),
  ['6'] =           NOT_7_NO_DP(S1),
  ['7'] =           S0 + S1 + S2,
  ['8'] =           NOT_7_NO_DP(0),
  ['9'] =           NOT_7_NO_DP(S4),

  ['a'] =           S2 + S3 + S4 + S6,
  ['b'] =           NOT_7_NO_DP(S0 + S1),
  ['c'] =           S6 + S4 + S3,
  ['d'] =           NOT_7_NO_DP(S0 + S5),
  ['e'] =           NOT_7_NO_DP(S2),
  ['f'] =           S6 + S5 + S4 + S0, /* same as F */
  ['g'] =           NOT_7_NO_DP(S4), /* same as 9 */
  ['h'] =           S6 + S5 + S4 + S2,
  ['i'] =           S4,
  ['j'] =           S1 + S2 + S3 + S4,
  ['k'] =           S6 + S5 + S4 + S2, /* a h */
  ['l'] =           S3 + S4,
  ['m'] =           S0 + S4 + S2, /* same as  */
  ['n'] =           S2 + S4 + S6,
  ['o'] =           S6 + S4 + S3 + S2,
  ['p'] =           NOT_7_NO_DP(S3 + S2), /* same as P */
  ['q'] =           S0 + S1 + S2 + S5 + S6,
  ['r'] =           S4 + S6,
  ['s'] =           NOT_7_NO_DP(S1 + S4),
  ['t'] =           NOT_7_NO_DP(S0 + S1 + S2),
  ['u'] =           S4 + S3 + S2 + S5 + S1, /* U */
  ['v'] =           S4 + S3 + S2, /* is u but u use U */
  ['w'] =           S1 + S3 + S5,
  ['x'] =           NOT_7_NO_DP(S0 + S3), /* similar to H */
  ['y'] =           NOT_7_NO_DP(S0 + S4),
  ['z'] =           S0 + S1 + S6 + S4 + S3, /* same as 2 */

  ['A'] =           NOT_7_NO_DP(S3),
  ['B'] =           NOT_7_NO_DP(S0 + S1), /* as b  */
  ['C'] =           S0 + S3 + S4 + S5, /* same as [ */
  ['E'] =           NOT_7_NO_DP(S1 + S2),
  ['F'] =           S6 + S5 + S4 + S0,
  ['G'] =           NOT_7_NO_DP(S4), /* same as 9 */
  ['H'] =           NOT_7_NO_DP(S0 + S3),
  ['I'] =           S1 + S2,
  ['J'] =           S1 + S2 + S3 + S4,
  ['K'] =           NOT_7_NO_DP(S0 + S3), /* same as H */
  ['L'] =           S3 + S4 + S5,
  ['M'] =           S0 + S4 + S2, /* same as  m*/
  ['N'] =           S2 + S4 + S6, /* same as n*/
  ['O'] =           NOT_7_NO_DP(S6),
  ['P'] =           NOT_7_NO_DP(S3 + S2),
  ['Q'] =           NOT_7_NO_DP(S3 + S2),
  ['R'] =           S4 + S6,
  ['S'] =           NOT_7_NO_DP(S1 + S4), /* sasme as 5 */
  ['T'] =           NOT_7_NO_DP(S0 + S1 + S2), /* sasme as t */
  ['U'] =           NOT_7_NO_DP(S6 + S0),
  ['V'] =           S4 + S3 + S2, /* is u but u use U */
  ['W'] =           S1 + S3 + S5,
  ['X'] =           NOT_7_NO_DP(S0 + S3), /* similar to H */
  ['Y'] =           NOT_7_NO_DP(S0 + S4),
  ['Z'] =           S0 + S1 + S6 + S4 + S3, /* same as 2 */
};

#undef S0
#undef S1
#undef S2
#undef S3
#undef S4
#undef S5
#undef S6
#undef DP
/**
  * @}
  */

/** @defgroup XNUCLEO_53L3A2_COMMON_Private_Variables Private Variables
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

/** @defgroup XNUCLEO_53L3A2_COMMON_Private_Functions_Prototypes Private Functions Prototypes
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
  * @brief Initialize X-NUCLEO-53L3A2 STM32 expansion board
  * @note All devices XSDN are asserted and display is turned off
  * @return 0 on success
  */
int32_t VL53L3A2_Init(void)
{
  int32_t status = 0;
  uint8_t ExpanderData[2];

  if (InitCounter++ == 0U)
  {
    status |= _I2cFailRecover();
    status |= VL53L3A2_I2C_INIT();

    if (status != BSP_ERROR_NONE)
    {
      goto done_err;
    }

	  status = _ExpanderRd(I2C_EXPANDER_ADDR0, 0, ExpanderData, 2);

	  if ((status != 0) || (ExpanderData[0] != 0x00U) || (ExpanderData[1] != 0x16U))
	  {
      VL53L3A2_ErrLog("I2C Expander @0x%02X not detected", (int)I2C_EXPANDER_ADDR0);
      goto done_err;
	  }

	  status = _ExpanderRd(I2C_EXPANDER_ADDR1, 0, ExpanderData, 2);

	  if ((status != 0) || (ExpanderData[0] != 0x00U) || (ExpanderData[1] != 0x16U))
	  {
		  VL53L3A2_ErrLog("I2C Expander @0x%02X not detected", (int)I2C_EXPANDER_ADDR1);
		  goto done_err;
	  }

	  CurIOVal.u32 = 0x0U;

	  /* setup expander   i/o direction  all output but exp1 bit 14*/
	  ExpanderData[0] = 0xFFU;
	  ExpanderData[1] = 0xFFU;

	  status = _ExpanderWR(I2C_EXPANDER_ADDR0, GPDR, ExpanderData, 2);

	  if (status)
	  {
		VL53L3A2_ErrLog("Set Expander @0x%02X DR", I2C_EXPANDER_ADDR0);
		goto done_err;
	  }

	  ExpanderData[0] = 0xFFU;
	  ExpanderData[1] = 0xBFU; /* all but bit 14-15 that is pb1 and xhurt */

	  status = _ExpanderWR(I2C_EXPANDER_ADDR1, GPDR, ExpanderData, 2);

	  if (status)
	  {
		VL53L3A2_ErrLog("Set Expander @0x%02X DR", I2C_EXPANDER_ADDR1);
		goto done_err;
	  }

	  /* shut down all segment and all device */
	  CurIOVal.u32 = 0x7FU + (0x7FU << 7) + (0x7FU << 16) + (0x7FU << (16 + 7));

	  status = _ExpandersSetAllIO();

	  if (status)
	  {
		VL53L3A2_ErrLog("Set initial i/o ");
	  }
  }

done_err:
  return status;
}

/**
  * @brief De-initialize X-NUCLEO-53L3A2 STM32 expansion board
  * @return 0 on success
  */
int32_t VL53L3A2_DeInit(void)
{
  int32_t status = 0;

  if (InitCounter > 0U)
  {
	if (--InitCounter == 0U)
	{
      status = VL53L3A2_I2C_DEINIT();
	}
  }

  return status;
}

/**
  * @brief Set Reset (XSDN) state of a given "id" device
  * @param  DevNo The device number, use @ref VL53L3A2_dev_e.
  * @param  state  State of the device reset (xsdn) pin @warning reset pin is active low
  * @return 0 on success
  */
int32_t VL53L3A2_ResetId(uint8_t DevNo, uint8_t state)
{
  int32_t status;

  switch (DevNo)
  {
    case VL53L3A2_DEV_CENTER :
      CurIOVal.bytes[3] &= ~0x80U; /* bit 15 expander 1  => byte #3 */

      if (state)
      {
        CurIOVal.bytes[3] |= 0x80U;  /* bit 15 expander 1  => byte #3 */
      }

      status = _ExpanderWR(I2C_EXPANDER_ADDR1, GPSR + 1, &CurIOVal.bytes[3], 1);
      break;

    case VL53L3A2_DEV_LEFT :
      CurIOVal.bytes[1] &= ~0x40U; /* bit 14 expander 0 => byte #1*/

      if (state)
      {
        CurIOVal.bytes[1] |= 0x40U;  /* bit 14 expander 0 => byte #1*/
      }

      status = _ExpanderWR(I2C_EXPANDER_ADDR0, GPSR + 1, &CurIOVal.bytes[1], 1);
      break;

    case VL53L3A2_DEV_RIGHT :
      CurIOVal.bytes[1] &= ~0x80U; /* bit 15 expander 0  => byte #1 */

      if (state)
      {
        CurIOVal.bytes[1] |= 0x80U;  /* bit 15 expander 0 => byte #1*/
      }

      status = _ExpanderWR(I2C_EXPANDER_ADDR0, GPSR + 1, &CurIOVal.bytes[1], 1);
      break;

    default:
      VL53L3A2_ErrLog("Invalid DevNo %d", DevNo);
      status = -1;
      goto done;
  }

  /* error with valid id */
  if (status)
  {
    VL53L3A2_ErrLog("expander i/o error for DevNo %d state %d ", DevNo, state);
  }

done:
  return status;
}

/**
  * @brief Set the 7 segments display
  * @param str  String to set on display
  * @warning When string is less than 4 digits, display is left-justified and lower digits are blanked.
  *          To display a 2 digits value, left justified on the 4 digits, use "%4d" formatting
  * @warning When more than 4 char are present only first 4 are displayed
  * @note    Characters that do not have 7 segment font matching in ascii_to_display_lut are left blank
  * @return 0 on success
  */
int32_t VL53L3A2_SetDisplayString(const char *str)
{
  int32_t status;
  uint32_t BitPos;
  uint32_t i;
  uint32_t Segments;

  /* lookup table for for digit  to bit position in @a CurIOVal u32 */
  uint8_t DisplayBitPos[4] = {0, 7, 16, 16 + 7};

  for (i = 0; (i < 4U) && (str[i] != 0U); i++)
  {
    Segments = (uint32_t)ascii_to_display_lut[(uint8_t)str[i]];
    Segments = (~Segments) & 0x7FU;
    BitPos = DisplayBitPos[i];
    CurIOVal.u32 &= ~(0x7FU << BitPos);
    CurIOVal.u32 |= Segments << BitPos;
  }

  /* clear unused digit */
  for (; i < 4U; i++)
  {
    BitPos = DisplayBitPos[i];
    CurIOVal.u32 |= 0x7FU << BitPos;
  }

  status = _ExpandersSetAllIO();

  if (status)
  {
    VL53L3A2_ErrLog("Set i/o");
  }

  return status;
}

/** @defgroup XNUCLEO_53L3A2_COMMON_Private_Functions Private Functions
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

  GPIO_InitStruct.Pin = VL53L3A2_I2C_SCL_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(VL53L3A2_I2C_SCL_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = VL53L3A2_I2C_SDA_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(VL53L3A2_I2C_SDA_GPIO_PORT, &GPIO_InitStruct);

  HAL_GPIO_WritePin(VL53L3A2_I2C_SCL_GPIO_PORT, VL53L3A2_I2C_SCL_GPIO_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(VL53L3A2_I2C_SDA_GPIO_PORT, VL53L3A2_I2C_SDA_GPIO_PIN, GPIO_PIN_SET);

  do
  {
    for (i = 0; i < 10U; i++)
    {
      HAL_GPIO_WritePin(VL53L3A2_I2C_SCL_GPIO_PORT, VL53L3A2_I2C_SCL_GPIO_PIN, GPIO_PIN_RESET);
      HAL_Delay(1);
      HAL_GPIO_WritePin(VL53L3A2_I2C_SCL_GPIO_PORT, VL53L3A2_I2C_SCL_GPIO_PIN, GPIO_PIN_SET);
      HAL_Delay(1);
    }
    retry_cnt++;
  } while ((HAL_GPIO_ReadPin(VL53L3A2_I2C_SDA_GPIO_PORT, VL53L3A2_I2C_SDA_GPIO_PIN) == GPIO_PIN_RESET) && (retry_cnt < 7U));

  if (HAL_GPIO_ReadPin(VL53L3A2_I2C_SCL_GPIO_PORT, VL53L3A2_I2C_SDA_GPIO_PIN) == GPIO_PIN_RESET)
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
  VL53L3A2_GetI2cBus();

  do
  {
    status = HAL_I2C_Master_Transmit(&VL53L3A2_HI2C, I2cExpAddr, &RegAddr, 1, 100);

    if (status)
    {
      break;
    }

    status = HAL_I2C_Master_Receive(&VL53L3A2_HI2C, I2cExpAddr, data, n_data, n_data * 100);
  } while (0);

  VL53L3A2_PutI2cBus();

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

  VL53L3A2_GetI2cBus();
  status = HAL_I2C_Master_Transmit(&VL53L3A2_HI2C, I2cExpAddr, RegAddr, n_data + 1, 100);
  VL53L3A2_PutI2cBus();

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

