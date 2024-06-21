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

#ifndef DSPLIB_sqrAdd_IXX_IXX_OXX_IDAT_H

#define DSPLIB_sqrAdd_IXX_IXX_OXX_IDAT_H

#include <dsplib.h>

// include test infrastructure provided by MMALIB
#include "../../test/DSPLIB_test.h"

typedef struct {
   uint8_t testPattern;   /* 0: conssqrAddt, 1: sequential, 2: random, 3: static array, 4: file, etc */
   void *staticIn;
   void *staticOut;
   uint32_t dataTypeIn;
   uint32_t dataTypeOut;
   uint32_t dataSize;
   uint32_t shift; // 0-> for float, double, required for integer types
   uint32_t outputDataLocation; // 0 -> HEAP (probably L2SRAM), 1 -> MSMC
   uint32_t numReps;
   uint32_t testID;
} sqrAdd_testParams_t;

void sqrAdd_getTestParams(sqrAdd_testParams_t * *params, int32_t *numTests);

#endif /* define DSPLIB_sqrAdd_IXX_IXX_OXX_IDAT_H */

/* ======================================================================== */
/*  End of file:  DSPLIB_sqrAdd_idat.h                                     */
/* ======================================================================== */

