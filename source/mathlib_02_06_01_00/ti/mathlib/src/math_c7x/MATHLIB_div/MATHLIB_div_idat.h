/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 2016 Texas Instruments Incorporated                |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission to use, copy, modify, or distribute this software,            |**
**| whether in part or in whole, for any purpose is forbidden without        |**
**| a signed licensing agreement and NDA from Texas Instruments              |**
**| Incorporated (TI).                                                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

#ifndef MATHLIB_div_IXX_IXX_OXX_IDAT_H

#define MATHLIB_div_IXX_IXX_OXX_IDAT_H

#include <mathlib.h>

// include test infrastructure provided by MMALIB
#include "../../test/MATHLIB_test.h"

typedef struct {
   uint8_t testPattern;   /* 0: consdivt, 1: sequential, 2: random, 3: static array, 4: file, etc */
   void *staticIn;
   void *staticOut;
   uint32_t dataType;
   uint32_t dataSize;
   uint32_t outputDataLocation; // 0 -> HEAP (probably L2SRAM), 1 -> MSMC
   uint32_t numReps;
   uint32_t testID;
} div_testParams_t;

void div_getTestParams(div_testParams_t * *params, int32_t *numTests);

#endif /* define MATHLIB_div_IXX_IXX_OXX_IDAT_H */

/* ======================================================================== */
/*  End of file:  MATHLIB_div_idat.h                                     */
/* ======================================================================== */

