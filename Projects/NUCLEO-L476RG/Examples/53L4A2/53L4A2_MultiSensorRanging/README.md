---
pagetitle: README
lang: en
---

::: {.row}

::: {.col-sm-12 .col-lg-11}
::: {.collapse}
<div>

### <b>53L4A2_MultiSensorRanging example description</b>

This example application illustrates how to use the X-NUCLEO-53L4A2 expansion board with a STM32 Nucleo board 
to perform ranging measurements simultaneously on three sensors.
This example outputs the results on a hyper-terminal like TeraTerm through a serial link.
For further details, please refer to the user manual for more details.

The example application uses the following default serial settings:

  - Baud Rate: 460800
  - Data Bits: 8
  - Parity: None
  - Stop Bits: 1

### <b>Keywords</b>

TOF, I2C, VCOM

### <b>Hardware and Software environment</b>

  - This example runs on STM32 Nucleo boards with X-NUCLEO-53L4A2 STM32 expansion board
  - Make sure that two SATEL-VL53L4CX breakout boards are plugged correctly on the X-NUCLEO-53L4A2 expansion board.
  - If you power the Nucleo board via USB 3.0 port, check that you have flashed the last firmware version of
    the ST-Link v2 on the Nucleo board. In order to flash the latest firmware version of the 
	ST-Link v2, you can use the STM32 ST Link Utility.
  - This example has been tested with STMicroelectronics:
    - [NUCLEO-L476RG board](https://www.st.com/en/product/nucleo-l476rg.html)

ADDITIONAL_BOARD : X-NUCLEO-53L4A2 https://www.st.com/en/product/x-nucleo-53l4a2.html
ADDITIONAL_COMP : VL53L4CX https://www.st.com/en/product/vl53l4cx.html
ADDITIONAL_COMP : SATEL-VL53L4CX https://www.st.com/en/product/satel-vl53l4cx.html

### <b>How to use it ?</b>

To ensure the program operates correctly, execute the following steps:

 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - Open STM32CubeIDE.
   Alternatively you can use the Keil uVision toolchain or the IAR toolchain.
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
