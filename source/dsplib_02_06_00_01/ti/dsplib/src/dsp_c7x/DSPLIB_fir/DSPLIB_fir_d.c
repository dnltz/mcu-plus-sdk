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

// include DSPLIB
#include <dsplib.h>

// include test infrastructure provided by DSPLIB
#include "../../test/DSPLIB_test.h"

// include test data for this kernel
#include "DSPLIB_fir_idat.h"

__attribute__ ((section (".l2sramaux"), aligned (64))) int8_t l2auxBuffer[DSPLIB_L2_BUFFER_SIZE];
__attribute__ ((section (".ddrData"), aligned (64))) int8_t ddrBuffer[2048 * 1024];

int16_t volatile volatileSum = 0;  // use volatile to keep compiler from removing this operation

int DSPLIB_fir_d(uint32_t *pProfile, uint8_t LevelOfFeedback)
{
   int32_t  tpi;
   int32_t  currentTestFail;
   int32_t  fail = 0;
   uint32_t repCount;
   uint32_t numReps;
   DSPLIB_bufParams2D_t bufParamsIn, bufParamsOut;
   DSPLIB_bufParams1D_t bufParamsFilter;

#if (defined(RTL_TEST) || defined(QT_TEST) || defined(EVM_TEST) )
   uint32_t k;
#endif
   uint32_t testNum;
   uint64_t archCycles, estCycles;

   fir_testParams_t   *prm;
   fir_testParams_t   currPrm;
   fir_getTestParams(&prm, &test_cases);

   DSPLIB_fir_InitArgs    kerInitArgs;

#if DSPLIB_DEBUGPRINT
   printf("DSPLIB_DEBUGPRINT  DSPLIB_fir_d CP 0\n");
#endif

   int32_t handleSize = DSPLIB_fir_getHandleSize(&kerInitArgs);
   DSPLIB_kernelHandle handle = malloc(handleSize);

#if DSPLIB_DEBUGPRINT
   printf("DSPLIB_DEBUGPRINT  DSPLIB_fir_d CP 1\n");
#endif

   TI_profile_init("DSPLIB_fir");
#if ( !defined(RTL_TEST) && !defined(QT_TEST) && !defined(EVM_TEST))
   // file IO for Loki benchmarking
   FILE *fpOutputCSV = fopen("DSPLIB_fir.csv", "w+");
   fprintf(fpOutputCSV, "Kernel, Test ID, Test type, Bit Width, Parameters, Data size, Batch size, Filter size, Arch cycles, Loki cycles, Loki/Arch cycles, Pass/Fail\n");
#endif //   #if ( !defined(RTL_TEST) && !defined(QT_TEST) && !defined(EVM_TEST))

   for (tpi=0; tpi < test_cases; tpi++) {
      numReps = prm[tpi].numReps;
      testNum = prm[tpi].testID;
      currPrm = prm[tpi];

      for (repCount = 0; repCount < numReps; repCount++){
	 int32_t      status_nat_vs_opt = TI_TEST_KERNEL_FAIL;
	 int32_t      status_ref_vs_opt = TI_TEST_KERNEL_FAIL;
	 DSPLIB_STATUS   status_init = DSPLIB_SUCCESS;
	 DSPLIB_STATUS   status_opt = DSPLIB_SUCCESS;
	 DSPLIB_STATUS   status_nat = DSPLIB_SUCCESS;

	 /* Sometimes, depending on certain compile flags, the test will be
	  * marked PASS even if no comparison is done. This flag is to detect
	  * if a comparison was done or not                                   */
	 int32_t      comparisonDone = 0;

	 currentTestFail = 0;

	 kerInitArgs.dataSize   = currPrm.dataSize;
	 kerInitArgs.batchSize  = currPrm.batchSize;
	 kerInitArgs.filterSize = currPrm.filterSize;
	 kerInitArgs.shift      = currPrm.shift;

	 bufParamsIn.data_type  = currPrm.dataType;
	 bufParamsIn.dim_y      = currPrm.batchSize;
	 bufParamsIn.stride_y   = currPrm.dataPitchIn;

	 bufParamsOut.data_type    = currPrm.dataType;
	 bufParamsOut.dim_y        = currPrm.batchSize;
	 bufParamsOut.stride_y     = currPrm.dataPitchOut;
	 bufParamsFilter.data_type = currPrm.dataType;
	 DSPLIB_fir_getSizes(&kerInitArgs,
					     &bufParamsIn,
					     &bufParamsFilter,
					     &bufParamsOut);

   void *pIn     = (void *) TI_memalign(DSPLIB_L2DATA_ALIGNMENT, bufParamsIn.stride_y* bufParamsIn.dim_y);
   void *pFilter = (void *) TI_memalign(DSPLIB_L2DATA_ALIGNMENT, bufParamsFilter.dim_x*DSPLIB_sizeof(bufParamsFilter.data_type));
   void *pOut, *pOutCn;
   if (currPrm.outputDataLocation == DSPLIB_TEST_OUTPUT_HEAP) {
      pOut   = (void *) TI_memalign(DSPLIB_L2DATA_ALIGNMENT, bufParamsOut.dim_y*bufParamsOut.stride_y);
      pOutCn = (void *) malloc(bufParamsOut.dim_y*bufParamsOut.stride_y);
   } else if(currPrm.outputDataLocation == DSPLIB_TEST_OUTPUT_MSMC) {
      pOut   = (void *) TI_memalign(DSPLIB_L2DATA_ALIGNMENT, bufParamsOut.dim_y*bufParamsOut.stride_y);
      pOutCn = (void *)ddrBuffer;
   } else {
      pOut   = (void *) TI_memalign(DSPLIB_L2DATA_ALIGNMENT, bufParamsOut.dim_y*bufParamsOut.stride_y);
      pOutCn = (void *)ddrBuffer;
   }
#if DSPLIB_DEBUGPRINT
       printf("DSPLIB_DEBUGPRINT  DSPLIB_fir_d data_type %d dataSize %d filterSize %d\n", bufParamsIn.data_type,  kerInitArgs.dataSize, kerInitArgs.filterSize);
#endif
	/* Only run the test if the buffer allocations fit in the heap */
	if( pIn && pOut && pFilter && pOutCn ) {

	    /* No need to fill anything here, we just used patter 0xAB
	     * for testing purposes.                                    */

   TI_fillBuffer_float(prm[tpi].testPattern, 0,pIn,prm[tpi].staticIn,
	                        currPrm.dataSize, currPrm.batchSize, currPrm.dataPitchIn, sizeof (DSPLIB_F32), testPatternString);


   TI_fillBuffer_float(prm[tpi].testPattern, 0,
                            pFilter,prm[tpi].staticIn,
                            currPrm.filterSize, 1,
                            currPrm.filterSize,
                            sizeof (DSPLIB_F32), testPatternString);
#if DSPLIB_DEBUGPRINT
       printf("DSPLIB_DEBUGPRINT  DSPLIB_fir_d CP 0\n");
#endif

	    status_init = DSPLIB_fir_init_checkParams(
			  handle,
			  &bufParamsIn,
			  &bufParamsFilter,
			  &bufParamsOut,
			  &kerInitArgs);

#if DSPLIB_DEBUGPRINT
       printf("DSPLIB_DEBUGPRINT  DSPLIB_fir_d CP 1 status_init %d\n", status_init);
#endif
	    if (status_init == DSPLIB_SUCCESS) {
	       TI_profile_start(TI_PROFILE_KERNEL_INIT);
	       DSPLIB_asm(" MARK 0");
	       kerInitArgs.funcStyle = DSPLIB_FUNCTION_OPTIMIZED;
	       status_init = DSPLIB_fir_init(handle,
							     &bufParamsIn,
							     &bufParamsFilter,
							     &bufParamsOut,
							     &kerInitArgs);
	       DSPLIB_asm(" MARK 1");
	       TI_profile_stop();
	    }
#if DSPLIB_DEBUGPRINT
       printf("DSPLIB_DEBUGPRINT  DSPLIB_fir_d CP 2 status_init %d\n", status_init);
#endif
	    status_opt = DSPLIB_fir_exec_checkParams(handle,
								     pIn,
								     pFilter,
								     pOut);
#if DSPLIB_DEBUGPRINT
       printf("DSPLIB_DEBUGPRINT  DSPLIB_fir_d CP 3 status_opt %d\n", status_opt);
#endif
   if (status_opt == DSPLIB_SUCCESS){
      TI_profile_start(TI_PROFILE_KERNEL_OPT);
      DSPLIB_asm(" MARK 2");
      status_opt = DSPLIB_fir_exec(handle,
                            pIn,
                            pFilter,
                            pOut);
      DSPLIB_asm(" MARK 3");
      TI_profile_stop();
   }

#if (defined(RTL_TEST) || defined(QT_TEST) || defined(EVM_TEST) )
	    /* The following for loop is to call kernel repeatedly so as to
	     * train the branch predictor                                   */
	    for (k = 0; k < 4; k++) {
	       // run warm instruction cache test
	       TI_profile_clear_cycle_count_single(TI_PROFILE_KERNEL_OPT_WARM);
	       TI_profile_start(TI_PROFILE_KERNEL_OPT_WARM);
	       DSPLIB_asm(" MARK 4");
	       status_opt = DSPLIB_fir_exec(handle,
							    pIn,
							    pFilter,
							    pOut);
	       DSPLIB_asm(" MARK 5");
	       TI_profile_stop();
	    }

	    // get output to L1D
	    int16_t outSum = 0;
	    int8_t *pOutTemp = (int8_t *)pOut;       // treat output as bytes to be data type agnostic
	    for (k = 0; k < bufParamsOut.dim_x; k++) {
		outSum += *pOutTemp;
		pOutTemp++;
	    }

	    // dummy store of outSum to insure that the compiler does not remove it.
	    volatileSum = outSum;

	    // run warm instruction cache test
	    TI_profile_start(TI_PROFILE_KERNEL_OPT_WARMWRB);

	    DSPLIB_asm(" MARK 6");
	    status_opt = DSPLIB_fir_exec(handle,
							 pIn,
							 pFilter,
							 pOut);
	    DSPLIB_asm(" MARK 7");
	    TI_profile_stop();
#endif

#if (!defined(RTL_TEST))
	    /* Test _cn kernel */
	    kerInitArgs.funcStyle = DSPLIB_FUNCTION_NATC;
#if DSPLIB_DEBUGPRINT
       printf("DSPLIB_DEBUGPRINT  DSPLIB_fir_d CP 4 status_opt %d\n", status_opt);
#endif
	    // initialize the kernel to use the natural C variant
	    DSPLIB_fir_init(handle,
					    &bufParamsIn,
					    &bufParamsFilter,
					    &bufParamsOut,
					    &kerInitArgs);
#if DSPLIB_DEBUGPRINT
       printf("DSPLIB_DEBUGPRINT  DSPLIB_fir_d CP 5\n");
#endif
	    TI_profile_start(TI_PROFILE_KERNEL_CN);
	    DSPLIB_asm(" MARK 8");
	    status_nat = DSPLIB_fir_exec(handle,
							 pIn,
							 pFilter,
							 pOutCn);
	    DSPLIB_asm(" MARK 9");
	    TI_profile_stop();
#if DSPLIB_DEBUGPRINT
       printf("DSPLIB_DEBUGPRINT  DSPLIB_fir_d CP 6 status_nat %d\n", status_nat);
#endif

        status_nat_vs_opt = TI_compare_mem_2D_float ((void *)pOut,
                                                     (void *)pOutCn,
                                                     0,
                                                     (double) powf (2, -10),
                                                     bufParamsOut.dim_x,
                                                     bufParamsOut.dim_y,
                                                     bufParamsOut.stride_y,
                                                     sizeof (DSPLIB_F32));
	    comparisonDone = 1;
#else
	    status_nat_vs_opt = TI_TEST_KERNEL_PASS;
#endif

	    if( currPrm.staticOut != NULL ) {
	       status_ref_vs_opt = TI_compare_mem_2D((void *)currPrm.staticOut, (void *)pOut,
						     2, 65535,
						     bufParamsOut.dim_x,
						     1,
						     bufParamsOut.dim_x*DSPLIB_sizeof(bufParamsOut.data_type),
						     DSPLIB_sizeof(bufParamsOut.data_type));

	       comparisonDone = 1;
	    } else {
	       /* Set to pass since it wasn't supposed to run. */
	       status_ref_vs_opt = TI_TEST_KERNEL_PASS;
	    }

	    /* Set the 'fail' flag based on test vector comparison results */
	    currentTestFail = ((status_nat_vs_opt == TI_TEST_KERNEL_FAIL) ||
			       (status_ref_vs_opt == TI_TEST_KERNEL_FAIL) ||
			       (status_init != DSPLIB_SUCCESS) ||
			       (status_opt  != DSPLIB_SUCCESS) ||
			       (status_nat  != DSPLIB_SUCCESS) ||
			       (comparisonDone == 0) ||
			       (currentTestFail == 1)) ? 1 : 0;

	    fail = ((fail == 1) || (currentTestFail == 1)) ? 1 : 0;

	    pProfile[3*tpi]   = (int32_t)TI_profile_get_cycles(TI_PROFILE_KERNEL_OPT);
	    pProfile[3*tpi+1] = (int32_t)TI_profile_get_cycles(TI_PROFILE_KERNEL_OPT_WARM);
	    pProfile[3*tpi+2] = (int32_t)TI_profile_get_cycles(TI_PROFILE_KERNEL_OPT_WARMWRB);
#if defined(QT_TEST)
	    __ESTP_GS = pProfile[3*tpi];
	    __FSR     = pProfile[3*tpi+1];
	    __ESTP_S  = pProfile[3*tpi+2];
	    __GPLY    = !fail;
	    __DBGCTXT = testNum;
#endif // #if defined(QT_TEST)

#if !defined(RTL_TEST)
	    sprintf(desc, "%s generated input | Data size=%d, Batch size=%d, Filter size=%d",
		    testPatternString, currPrm.dataSize, currPrm.batchSize, currPrm.filterSize);

	    DSPLIB_fir_perfEst(handle,
					       &bufParamsIn,
					       &bufParamsFilter,
					       &bufParamsOut,
					       &archCycles,
					       &estCycles);

#if ( !defined(RTL_TEST) && !defined(QT_TEST) && !defined(EVM_TEST))
	    // write to CSV, must happen prior to write to screen because TI_profile_formula_add clears values in counters
	    fprintf(fpOutputCSV, "FIR, %d, %d, %d, Data size:%d Batch size: %d Filter size:%d, %d, %d, %d, %lu, %lu, %.2f, %d\n", testNum, currPrm.testPattern, DSPLIB_sizeof(bufParamsIn.data_type)*8, currPrm.dataSize, currPrm.batchSize, currPrm.filterSize, currPrm.dataSize, currPrm.batchSize, currPrm.filterSize, archCycles, cycles[TI_PROFILE_KERNEL_OPT], ((DSPLIB_F32)cycles[TI_PROFILE_KERNEL_OPT])/((DSPLIB_F32)archCycles),!currentTestFail);
#endif // #if ( !defined(RTL_TEST) && !defined(QT_TEST) && !defined(EVM_TEST))

	    TI_profile_add_test(testNum++,
				currPrm.dataSize*currPrm.batchSize*currPrm.filterSize,
				0,
				0,
				currentTestFail,
				desc);
	 } else {
	    sprintf(desc, "Data size=%d, Batch size=%d, Filter size=%d", currPrm.dataSize, currPrm.batchSize, currPrm.filterSize);
	    TI_profile_skip_test(desc);
#else
	    // clear the counters between runs; normally handled by TI_profile_add_test
	    TI_profile_clear_run_stats ();
#endif
	 } // end of memory allocation successful?

	 /* Free buffers for each test vector */
         if (currPrm.outputDataLocation == DSPLIB_TEST_OUTPUT_HEAP) {
            free(pOutCn);
            TI_align_free(pOut);
         }
	 TI_align_free(pIn);

#if defined(RTL_TEST)
	 // for RTL testing, request to return after first fail
	 if(fail){
	    free(handle);
	    return fail;
	 }
#endif
      } // end repetitions
   } // end idat test cases

   free(handle);

#if ( !defined(RTL_TEST) && !defined(QT_TEST) && !defined(EVM_TEST))
   /* Close results CSV */
   fclose(fpOutputCSV);
#endif //#if ( !defined(RTL_TEST) && !defined(QT_TEST) && !defined(EVM_TEST))

   return fail;
}

int test_main(uint32_t *pProfile) {
#if !defined(_HOST_BUILD)
   if( TI_cache_init()) {
      TI_memError("DSPLIB_fir");
      return 1;
   } else
#else
      printf("_HOST_BUILD is defined.\n");
#endif
   {
      return DSPLIB_fir_d(&pProfile[0], 0);
   }
}

/* Main call for individual test projects */
#if !defined(__ONESHOTTEST) && !defined(RTL_TEST)
int main()
{
   int fail = 1;

   uint32_t profile[256*3];

   DSPLIB_TEST_init();

   fail = test_main(&profile[0]);

#if !defined(NO_PRINTF)
   if(fail == 0)
      printf("Test Pass!\n");
   else
      printf("Test Fail!\n");

   int i;
   for(i = 0; i < test_cases; i++){
      printf("Test %4d: Cold Cycles = %8d, Warm Cycles = %8d, Warm Cycles WRB = %8d\n", i, profile[3*i], profile[3*i+1], profile[3*i+2]);
   }
#endif

   return fail;
}
#endif

/* ======================================================================== */
/*  End of file:  DSPLIB_fir_d.c                          */
/* ======================================================================== */
