/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
 *  \file     rm_checkers_app.c
 *
 *  \brief    This file contains RM safety checkers app code.
 *
 */

/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/

#include <stdint.h>
#include <safety_checkers_common.h>
#include <safety_checkers_soc.h>
#include <safety_checkers_rm.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SAFETY_CHECKERS_RM_INSUFFICIENT_SIZE	(10U)

/** \brief RM Register Change.
 *         This is to check the register change for the mismatch verification.
 */
#if defined(SOC_J721E) || defined(SOC_J7200)
#define SAFETY_CHECKERS_RM_REG_MOD_BASE_ADDR 							  (CSL_NAVSS0_UDMASS_UDMAP0_CFG_TCHAN_BASE)
#elif defined(SOC_J721S2) || defined(SOC_J784S4)
#define SAFETY_CHECKERS_RM_REG_MOD_BASE_ADDR 							  (CSL_NAVSS0_BCDMA0_CFG_TCHAN_BASE)
#elif defined(SOC_AM62AX) || defined(SOC_AM62PX) || defined(SOC_AM62X) || defined(SOC_J722S)
#define SAFETY_CHECKERS_RM_REG_MOD_BASE_ADDR 							  (CSL_DMASS0_PKTDMA_TCHAN_BASE)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void SafetyCheckersApp_rmRun(void *arg0);
extern uint64_t SafetyCheckersApp_getTimeUsec(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

static int32_t SafetyCheckersApp_rmregVerify();
static int32_t SafetyCheckersApp_rmPerfTest(void);
static int32_t SafetyCheckersApp_rmBuffCheck();
static int32_t SafetyCheckersApp_rmRegMismatch();

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void SafetyCheckersApp_rmRun(void *arg0)
{
	uint32_t status = SAFETY_CHECKERS_SOK;
   	
    status = SafetyCheckersApp_rmregVerify();
	
    if(status == SAFETY_CHECKERS_SOK)
	{
		status = SafetyCheckersApp_rmBuffCheck();
	}

	if(status == SAFETY_CHECKERS_SOK)
	{
		status = SafetyCheckersApp_rmPerfTest();
	}

	if(status == SAFETY_CHECKERS_SOK)
	{
		SAFETY_CHECKERS_log("All tests have PASSED.\r\n");
	}
	else
	{
		SAFETY_CHECKERS_log("\n One or more RM safety checkers apps have failed \r\n");
	}
	
#if defined LDRA_DYN_COVERAGE_EXIT
    SAFETY_CHECKERS_log("\n LDRA ENTRY... \r\n");
    upload_execution_history();
    SAFETY_CHECKERS_log("\n LDRA EXIT... \r\n");
#endif

	return;
}

/* ========================================================================== */
/*                       Internal Function Definitions                        */
/* ========================================================================== */

static int32_t SafetyCheckersApp_rmregVerify()
{
    int32_t      status = SAFETY_CHECKERS_SOK;
    uintptr_t    rmRegisterData[SAFETY_CHECKERS_RM_REGDUMP_SIZE];
	
    status = SafetyCheckers_rmGetRegCfg(rmRegisterData, SAFETY_CHECKERS_RM_REGDUMP_SIZE);
    if(status == SAFETY_CHECKERS_SOK)
    {
        status = SafetyCheckers_rmVerifyRegCfg(rmRegisterData, SAFETY_CHECKERS_RM_REGDUMP_SIZE);
		if (SAFETY_CHECKERS_SOK == status)
		{
			SAFETY_CHECKERS_log("\nRM register test pass\r\n\n");
		}
		else
		{
			SAFETY_CHECKERS_log("\nRM register test fail!!\r\n\n");
		}
		
    /* SafetyCheckersApp_rmRegMismatch function is not supported
     * for mcu R5 and main R5 cores in am62p and j722s.
     * Because only WakeUp R5 has write permission to change
     * DMASS0_PKTDMA_TCHAN_BASE register.
     */
#if ((!defined (SOC_J722S) && !defined(SOC_AM62PX)) || defined (BUILD_WKUP_R5))
	if(status == SAFETY_CHECKERS_SOK)
	{
		status = SafetyCheckersApp_rmRegMismatch();
	}
#endif
    }
	
    return (status);
}

static int32_t SafetyCheckersApp_rmRegMismatch(void)
{
	int32_t     status = SAFETY_CHECKERS_SOK;
	uintptr_t   rmRegisterData[SAFETY_CHECKERS_RM_REGDUMP_SIZE];
	uint32_t    readVal;

	readVal = CSL_REG32_RD(SAFETY_CHECKERS_RM_REG_MOD_BASE_ADDR);
	CSL_REG32_WR(SAFETY_CHECKERS_RM_REG_MOD_BASE_ADDR,~readVal);

	status = SafetyCheckers_rmVerifyRegCfg(rmRegisterData, SAFETY_CHECKERS_RM_REGDUMP_SIZE);

	if(SAFETY_CHECKERS_REG_DATA_MISMATCH == status)
	{
		SAFETY_CHECKERS_log("\nRM register change verified\r\n\n");
		status = SAFETY_CHECKERS_SOK;
	}
	else
	{
		SAFETY_CHECKERS_log("\nRM register change failed\r\n\n");
		status = SAFETY_CHECKERS_FAIL;
	}

	return (status);
}

static int32_t SafetyCheckersApp_rmBuffCheck(void)
{	
	int32_t      status = SAFETY_CHECKERS_FAIL;
    uintptr_t    rmRegisterData[SAFETY_CHECKERS_RM_INSUFFICIENT_SIZE];

    status = SafetyCheckers_rmGetRegCfg (NULL, SAFETY_CHECKERS_RM_INSUFFICIENT_SIZE);
    if(SAFETY_CHECKERS_FAIL == status)
    {
        status = SafetyCheckers_rmVerifyRegCfg (NULL, SAFETY_CHECKERS_RM_INSUFFICIENT_SIZE);
    }

    if(SAFETY_CHECKERS_FAIL == status)
    {
        status = SafetyCheckers_rmGetRegCfg (rmRegisterData, SAFETY_CHECKERS_RM_INSUFFICIENT_SIZE);
        if(SAFETY_CHECKERS_INSUFFICIENT_BUFF == status)
        {
            status = SafetyCheckers_rmVerifyRegCfg (rmRegisterData, SAFETY_CHECKERS_RM_INSUFFICIENT_SIZE);
        }
    }

    if(SAFETY_CHECKERS_INSUFFICIENT_BUFF == status)
    {
        SAFETY_CHECKERS_log("\nRM error condition check test passed\r\n\n");
    }

    return ( SAFETY_CHECKERS_SOK);
}

static int32_t SafetyCheckersApp_rmPerfTest(void)
{
    int32_t      status = SAFETY_CHECKERS_FAIL;
    uint64_t     startTime = 0U;
    uint64_t     endTime = 0U;
    uint32_t     timeDiff = 0U;
    uintptr_t    rmRegisterData[SAFETY_CHECKERS_RM_REGDUMP_SIZE];

    /* Get the RM register dump */
    status = SafetyCheckers_rmGetRegCfg (rmRegisterData, SAFETY_CHECKERS_RM_REGDUMP_SIZE);

    if(SAFETY_CHECKERS_SOK == status)
    {
        startTime = SafetyCheckersApp_getTimeUsec();

        /* validate register dump with current value */
        status = SafetyCheckers_rmVerifyRegCfg (rmRegisterData, SAFETY_CHECKERS_RM_REGDUMP_SIZE);

        endTime = SafetyCheckersApp_getTimeUsec();

        if (endTime < startTime)
        {
            /* Counter overflow occured */
            timeDiff = (0xFFFFFFFFU - startTime) + endTime + 1U;
        }
        timeDiff = endTime - startTime;

        SAFETY_CHECKERS_log("\nTime taken for the execution of RM register dump and readback : %d usecs\r\n", timeDiff);
    }

    return (status);
}
