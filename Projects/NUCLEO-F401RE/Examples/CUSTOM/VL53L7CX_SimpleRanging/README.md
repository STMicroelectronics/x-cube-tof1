---
pagetitle: README
lang: en
---

::: {.row}

::: {.col-sm-12 .col-lg-11}
::: {.collapse}
<div>

### <b>VL53L7CX_SimpleRanging example description</b>

This example application shows how to use the SATEL-VL53L7CX breakout board on a STM32 Nucleo board 
to perform ranging measurements and print the results on a hyper-terminal such as TeraTerm through a serial link.
Please refer to the user manual for more details.

The example application uses the following default serial settings:

  - Baud Rate: 460800
  - Data Bits: 8
  - Parity: None
  - Stop Bits: 1

This example can use directly the bare driver instead of the BSP layers if you set the #define USE_BARE_DRIVER in app_tof.c


### <b>Keywords</b>

TOF, I2C, VCOM

### <b>Hardware and Software environment</b>

  - This example runs on STM32 Nucleo boards with SATEL-VL53L7CX breakout board
  - Connect the SATEL-VL53L7CX pins to the following STM32 Nucleo board inputs:
    - SCL <---> D15
    - SDA <---> D14
    - INT <---> A2
    - I2C_RST <---> A1
    - LPn <---> A3
    - PWR_EN <---> A5
    - IOVDD <---> 3V3
    - AVDD <---> 5V
  - If you power the Nucleo board via USB 3.0 port, please check that you have flashed the last version of
    the firmware of ST-Link v2 inside the Nucleo board. In order to flash the last available firmware of the 
    ST-Link v2, you can use the STM32 ST Link Utility.
  - This example has been tested with STMicroelectronics:
    - [NUCLEO-F401RE board](https://www.st.com/en/product/nucleo-f401re.html)

ADDITIONAL_BOARD : SATEL-VL53L7CX https://www.st.com/en/product/satel-vl53l7cx.html
ADDITIONAL_COMP : VL53L7CX https://www.st.com/en/product/vl53l7cx.html

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - Open STM32CubeIDE (this firmware has been successfully tested with Version 1.10.1).
   Alternatively you can use the Keil uVision toolchain (this firmware
   has been successfully tested with V5.37.0) or the IAR toolchain (this firmware has 
   been successfully tested with Embedded Workbench V9.20.1).
 - Rebuild all files and load your image into target memory.
 - Run the example.
 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package.

### <b>License</b>

Copyright (c) 2022 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

</div>
:::

:::
:::
