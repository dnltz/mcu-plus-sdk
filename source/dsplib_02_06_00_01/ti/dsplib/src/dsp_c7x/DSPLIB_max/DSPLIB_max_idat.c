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

#include "DSPLIB_max_idat.h"


static max_testParams_t    testParams[]=
{
   /********************************************
    {
    testPattern,
    staticIn,
    staticOut,
    dataType,
    dataSize,
    shift,
    outputDataLocation,
    numReps, testID
    },
    *********************************************/


#if (defined(ALL_TEST_CASES) || (EXTENDED_TEST_CASE == 1) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_FLOAT32,          //dataType
      256,                     //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      1                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 2) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_FLOAT32,          //dataType
      64,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      2                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 3) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_FLOAT32,          //dataType
      512,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      3                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 4) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_FLOAT32,          //dataType
      1024,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      4                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 5) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_FLOAT32,          //dataType
      576,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      5                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 6) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_FLOAT32,          //dataType
      1152,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      6                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 7) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_INT8,          //dataType
      1152,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      7                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 8) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_UINT8,          //dataType
      1152,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      8                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 9) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_INT16,          //dataType
      1152,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      9                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 10) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_UINT16,          //dataType
      1152,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      10                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 11) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_INT32,            //dataType
      1152,                    //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      11                       //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 12) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_UINT32,           //dataType
      1152,                    //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      12                       //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 13) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_FLOAT32,          //dataType
      51200,                   //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      13                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 14) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_INT8,          //dataType
      51200,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      14                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 15) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_UINT8,          //dataType
      51200,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      15                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 16) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_INT16,          //dataType
      51200,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      16                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 17) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_UINT16,          //dataType
      51200,                      //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      17                        //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 18) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_INT32,            //dataType
      51200,                    //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      18                       //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 19) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_UINT32,           //dataType
      51200,                    //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      19                       //testID
   },
#endif
#if (defined(ALL_TEST_CASES)  || (EXTENDED_TEST_CASE == 20) || (TEST_CATEGORY == 100) )
   {
      RANDOM,
      NULL,                    //*staticIn
      NULL,                    //*staticOut
      DSPLIB_FLOAT32,          //dataType
      51200,                   //dataSize
      0,                       //shift
      DSPLIB_TEST_OUTPUT_MSMC, //outputDataLocation
      1,                       //numReps
      20                        //testID
   },
#endif
};

/*
 *  Sends the test parameter structure and number of tests
 */
void max_getTestParams(max_testParams_t * *params, int32_t *numTests)
{
   *params = testParams;
   *numTests = sizeof(testParams) / sizeof(max_testParams_t);
}

/* ======================================================================== */
/*                       End of file                                        */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2016 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

