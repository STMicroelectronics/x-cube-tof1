/**
 ******************************************************************************
 * Copyright (c) 2020, STMicroelectronics - All Rights Reserved
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */



#ifndef _VL53LX_PLATFORM_USER_DATA_H_
#define _VL53LX_PLATFORM_USER_DATA_H_

#ifndef __KERNEL__
#include <stdlib.h>
#endif

#include "vl53lx_def.h"
#include "vl53l3cx.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef VL53L3CX_Object_t* VL53LX_DEV;

#define VL53LXDevDataGet(Obj, field) 			(Obj->Dev.Data.field)
#define VL53LXDevDataSet(Obj, field, data) 		((Obj->Dev.Data.field) = (data))
#define PALDevDataGet(Obj, field) 				(Obj->Dev.Data.field)
#define PALDevDataSet(Obj, field, value) 		(Obj->Dev.Data.field)=(value)
#define VL53LXDevStructGetLLDriverHandle(Obj) 	(&Obj->Dev.Data.LLData)
#define VL53LXDevStructGetLLResultsHandle(Obj) 	(&Obj->Dev.Data.llresults)

#ifdef __cplusplus
}
#endif

#endif


