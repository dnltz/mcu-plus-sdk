/*
 *   Copyright (c) Texas Instruments Incorporated 2022-2023
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

 /**
 *  \file     mtog_func.c
 *
 *  \brief    This file contains MTOG functional code. .
 *
 *  \details  MTOG Safety Example
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sdl/include/sdlr.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/HwiP.h>
#include <kernel/dpl/ClockP.h>
#include "mtog_main.h"
#include <sdl/include/sdl_types.h>
#include <sdl/sdl_mtog.h>
#include <sdl/sdl_esm.h>
#include <sdl/esm/sdl_esm.h>
#include <sdl/esm/sdl_esm_priv.h>
#include <sdl/esm/v0/esm.h>

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#define WKUP_ESM_BASE 			(SDL_WKUP_ESM0_CFG_BASE)
#define MTOG_MAX_TIMEOUT_VALUE  (1000000000u)
#define MTOG_STATUS_REG         (0x04504610U)
/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
void MTOG_datAbortExceptionHandler(void *param);
int32_t SDL_ESM_applicationCallbackFunction(SDL_ESM_Inst esmInst,
                                                   SDL_ESM_IntType esmIntrType,
                                                   uint32_t grpChannel,
                                                   uint32_t index,
                                                   uint32_t intSrc,
                                                   void *arg);
void MTOG_eventHandler( uint32_t instanceIndex );
int32_t MTOG_runTest(uint32_t instanceIndex);
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
uint32_t instanceIndex=1u;
typedef void (*MTOG_handlerPtr)(uint32_t instanceIndex);

int32_t apparg;
/** Instance name */
char instanceName[16] = "MCU MTOG0";
/** ESM error event number */
uint32_t ESMEventNumber;
/** Flag to indicate test done, will be set when interrupt event comes */
volatile bool doneFlag;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
/* Initialization structure for MCU ESM instance */
static SDL_ESM_config MTOG_Example_esmInitConfig_WKUP =
{
    .esmErrorConfig = {0u, 3u}, /* Self test error config */
    .enableBitmap = {0x04000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                },
     /**< All events enable: except timer and self test  events, */
    /*    and Main ESM output.Configured based off esmErrorConfig to test high or low priorty events.*/
    .priorityBitmap = {0x04000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                },
    /**< Configured based off esmErrorConfig to test high or low priorty events. */
    .errorpinBitmap = {0x04000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                },
};

static void IntrDisable(uint32_t intrSrc)
{
    /* Clear ESM registers. */
    uint32_t baseAddr = 0U;
    SDL_ESM_getBaseAddr(SDL_ESM_INST_WKUP_ESM0, &baseAddr);
    SDL_ESM_clrNError(SDL_ESM_INST_WKUP_ESM0);
}

void MTOG_eventHandler( uint32_t instanceIndex )
{
    int32_t status = SDL_PASS;

    /* Reset the Timeout gasket */
    status = SDL_MTOG_reset( instanceIndex );

    if (status == SDL_PASS)
    {
        DebugP_log("\n MTOG Reset done\n");
    }
    else{
        DebugP_log("\n MTOG Reset failed");
    }
    doneFlag = true;

    return;
}

int32_t MTOG_runTest(uint32_t instanceIndex)
{
    int32_t result = 0;
    int32_t status=0;
    uint64_t startTime,testStartTime,testEndTime, endTime;
    uint64_t prepTime, diffTime, restoreTime;
    volatile uint32_t timeoutCount = 0;
    uint32_t mtog_base_addr=0x0u;
#if defined(SOC_AM62AX)	|| defined (SOC_AM62PX)
	int32_t regstatus=0;
#endif
    SDL_MTOG_getBaseaddr(instanceIndex, &mtog_base_addr);
    SDL_MTOG_config config;
    config.timeOut = SDL_MTOG_VAL_1K;
    SDL_MTOG_staticRegs staticRegs;
	ESMEventNumber	   = SDLR_WKUP_R5FSS0_CORE0_INTR_GLUELOGIC_MGASKET_INTR_GLUE_OUT_0; //26
    doneFlag           = false;

    DebugP_log("\n Starting MTOG test on %s, index %d...",
                instanceName,
                instanceIndex);
#ifdef DEBUG
    char inputChar;

    DebugP_log("\n Press 'n' to skip..Press any key to continue...");
    inputChar = UART_getc();

    if (inputChar == 'n')
    {
        DebugP_log("   Skipping this test. on request \n");
        return 0;
    }
    DebugP_log("\n  HwiP_Params_init complete \n");
#endif

    /* Initialize done flag */
    doneFlag = false;

    /* Get start time of test */
    startTime = ClockP_getTimeUsec();

    /** Step 1: Configure and start Master Timeout Gasket */
    if (result == 0)
    {
        SDL_MTOG_reset(instanceIndex);
        result = SDL_MTOG_init(instanceIndex, &config);
        if (status != SDL_PASS)
        {
            DebugP_log("   SDL_MTOG_init Failed \n");
            result = -1;
        }
    }
    if (result == 0)
    {
        result = SDL_MTOG_verifyConfig(instanceIndex, &config);
        if (status != SDL_PASS)
        {
            DebugP_log("   SDL_MTOG_verifyConfig Failed \n");
            result = -1;
        }
    }
	if (result == 0)
    {
        result = SDL_MTOG_getStaticRegisters(instanceIndex, &staticRegs);
        if (status != SDL_PASS)
        {
            DebugP_log("   SDL_MTOG_getStaticRegisters Failed \n");
            result = -1;
        }
    }
    if (result == 0)
    {
        SOC_unlockAllMMR();
        /* Call SDL API to enable Timeout Gasket */
        status = SDL_MTOG_start(instanceIndex);
        if (status != SDL_PASS)
        {
            DebugP_log("   SDL_MTOG_start Failed \n");
            result = -1;
        }
    }
    /* Get start time of test */
    testStartTime = ClockP_getTimeUsec();
    /* Step 2: Inject master timeout error */
	if (result == 0)
    {
       status = SDL_MTOG_forceTimeout(instanceIndex);
      if (status != SDL_PASS)
      {
          DebugP_log("\n SDL_MTOG_forceTimeout Failed \n");
          result = -1;
      }
#if defined(SOC_AM62AX)	|| defined (SOC_AM62PX)
	  regstatus = SDL_REG32_RD(MTOG_STATUS_REG);
	  DebugP_log("\n MTOG Status Register Value for the instance%d = %d \n",instanceIndex, regstatus);
#endif
    }
    /**--- Step 2: Wait for MTOG Interrupt ---*/
    if (result == 0)
    {
        /* Timeout if exceeds time */
        while ((!doneFlag)
               && (timeoutCount++ < MTOG_MAX_TIMEOUT_VALUE))
        {

        }
        if(timeoutCount >= MTOG_MAX_TIMEOUT_VALUE)
        {
            DebugP_log("\n MTOG Timed out  \n");
            result = -1;
        }
    }

    /* Get end time of test */
     testEndTime = ClockP_getTimeUsec();

    /**--- Step 3: Disable ESM ---*/
    if (result == 0)
    {
	    status=SDL_ESM_disableIntr(WKUP_ESM_BASE, ESMEventNumber);

        if (status != SDL_PASS)
        {
            DebugP_log("   sdlAppEsmDisable Failed \n");
            result = -1;
        }
    }

    /* Here MTOG test is complete , get end time of test */
    endTime = ClockP_getTimeUsec();

    prepTime = testStartTime - startTime;
    diffTime = testEndTime - testStartTime;
    restoreTime = endTime - testEndTime;
    DebugP_log("\n Delta MTOG prep time in micro secs %d \n", (uint32_t)prepTime );
    DebugP_log(" \n Delta MTOG execution time in micro secs %d \n", (uint32_t)diffTime );
    DebugP_log(" \n  Delta MTOG restore time in micro secs %d \n", (uint32_t)restoreTime );

    DebugP_log("  MTOG complete for %s \n",instanceName);

    return (result);
}

void MTOG_datAbortExceptionHandler(void *param)
{
    /* This is a fake exception so return */
}

int32_t SDL_ESM_applicationCallbackFunction(SDL_ESM_Inst esmInst, SDL_ESM_IntType esmIntrType,
                                            uint32_t grpChannel,  uint32_t index, uint32_t intSrc, void *arg)
{
    int32_t retVal = SDL_PASS;
    MTOG_eventHandler(instanceIndex);
    DebugP_log("\nInterrupt is generated to ESM\n");
    DebugP_log("    ESM Call back function called : instType 0x%x, intType 0x%x, " \
                "grpChannel 0x%x, index 0x%x, intSrc 0x%x \n",
                esmInst, esmIntrType, grpChannel, index, intSrc);
    DebugP_log("  Take action \n");

    /* For clearing the interrupt */
    IntrDisable(intSrc);

    return retVal;
}

/* MTOG prepare for test */
int32_t MTOG_PrepareForTest(void)
{
    int32_t sdlResult = SDL_PASS;

	sdlResult = SDL_ESM_init(SDL_ESM_INST_WKUP_ESM0, &MTOG_Example_esmInitConfig_WKUP, SDL_ESM_applicationCallbackFunction, &apparg);
    if (sdlResult != SDL_PASS) {
        /* print error and quit */
        DebugP_log("\n TIMER_ESM_init: Error initializing MCU ESM: result = %d\n", sdlResult);
        sdlResult = SDL_EFAIL;
    } else {
        DebugP_log("\nTIMER_ESM_init: Init MCU ESM complete \n");
    }

    return sdlResult;
}

/* MTOG Functional test */
int32_t MTOG_func(void)
{
    int32_t    result = 0;

    result = MTOG_PrepareForTest();
    if (result != 0)
    {
        DebugP_log("   MTOG_PrepareForTest failed \n");
    }
    if (result == 0)
    {
		result = MTOG_runTest(instanceIndex);
	    if (result ==SDL_PASS)
	    {
			DebugP_log(" Sdl mtog instance 1 passed \n");
	    }
	    else
	    {
			DebugP_log(" Sdl mtog instance1 failed \n");
	    }
	    instanceIndex=instanceIndex+1;
		result = MTOG_runTest(instanceIndex);
	    if (result == SDL_PASS)
        {
			DebugP_log(" Sdl mtog instance 2 passed \n");
        }
        else
        {
			DebugP_log(" Sdl mtog instance 2 failed \n");
        }
    }

    return (result);
}
/* Nothing past this point */
