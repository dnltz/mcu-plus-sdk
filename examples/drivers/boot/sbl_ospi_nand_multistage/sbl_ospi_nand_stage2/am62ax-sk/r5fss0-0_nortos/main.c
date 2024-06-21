/*
 *  Copyright (C) 2018-2024 Texas Instruments Incorporated
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
 */

#include <stdlib.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <drivers/device_manager/sciclient.h>
#include <drivers/bootloader.h>
#include <drivers/pinmux.h>
#include <drivers/gtc.h>
#include <kernel/dpl/CacheP.h>


/* This start address and length depends upon the linker memory for second stage SBL.
   It is necessary to change the below start address and length if in case the linker
   memory region for second stage SBL is changed.
*/

#define BOOTLOADER_SECOND_STAGE_RESERVED_MEMORY_START       0xB0340000
#define BOOTLOADER_SECOND_STAGE_RESERVED_MEMORY_LENGTH      0x200000

CacheP_Config gCacheConfig = {};

/* This buffer needs to be defined for OSPI nand boot in case of HS device for
   image authentication
   The size of the buffer should be large enough to accomodate the appimage */
uint8_t gAppimage[0x1900000] __attribute__ ((section (".bss.app"), aligned (128)));

/*  In this sample bootloader, we load appimages for RTOS/Baremetal at different offset
    i.e the appimage for RTOS/Baremetal (for R5, MCU R5, A53) is flashed at different offset in eMMC

    Here at one eMMC offset, there is a multi-core .appimage that holds RPRC for MCU R5 and R5
    and another .appimage that holds the A53 binaries at another offset.

    When flashing make sure to flash images to below offset using the flash tool.

    RTOS/Baremetal appimage (for HSM) flash at offset 0x800000 of flash
    RTOS/Baremetal appimage (for A53) flash at offset 0xC00000 of flash
*/

/* call this API to stop the booting process and spin, do that you can connect
 * debugger, load symbols and then make the 'loop' variable as 0 to continue execution
 * with debugger connected.
 */
void loop_forever()
{
    volatile uint32_t loop = 1;
    while(loop)
        ;
}

int32_t App_loadImages(Bootloader_Handle bootHandle, Bootloader_BootImageInfo *bootImageInfo)
{
	int32_t status = SystemP_FAILURE;

    if(bootHandle != NULL)
    {
        status = Bootloader_parseMultiCoreAppImage(bootHandle, bootImageInfo);

        /* Load CPUs */
        if(status == SystemP_SUCCESS)
        {
            bootImageInfo->cpuInfo[CSL_CORE_ID_HSM_M4FSS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_HSM_M4FSS0_0);
            Bootloader_profileAddCore(CSL_CORE_ID_HSM_M4FSS0_0);
            status = Bootloader_loadCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_HSM_M4FSS0_0]));
        }
    }

    return status;
}

int32_t App_loadDSPImage(Bootloader_Handle bootHandle, Bootloader_BootImageInfo *bootImageInfo)
{
	int32_t status = SystemP_FAILURE;

    if(bootHandle != NULL)
    {
        status = Bootloader_parseMultiCoreAppImage(bootHandle, bootImageInfo);

        /* Load CPUs */
        if(status == SystemP_SUCCESS)
        {
            bootImageInfo->cpuInfo[CSL_CORE_ID_C75SS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_C75SS0_0);
            Bootloader_profileAddCore(CSL_CORE_ID_C75SS0_0);
            status = Bootloader_loadCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_C75SS0_0]));
        }
    }

    return status;
}

int32_t App_loadSelfcoreImage(Bootloader_Handle bootHandle, Bootloader_BootImageInfo *bootImageInfo)
{
	int32_t status = SystemP_FAILURE;

    if(bootHandle != NULL)
    {
        status = Bootloader_parseMultiCoreAppImage(bootHandle, bootImageInfo);

        if(status == SystemP_SUCCESS)
        {
            bootImageInfo->cpuInfo[CSL_CORE_ID_R5FSS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_R5FSS0_0);
            Bootloader_profileAddCore(CSL_CORE_ID_R5FSS0_0);
            status = Bootloader_loadSelfCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_R5FSS0_0]));
        }
    }

    return status;
}

int32_t App_loadA53Images(Bootloader_Handle bootHandle, Bootloader_BootImageInfo *bootImageInfo)
{
	int32_t status = SystemP_FAILURE;

    if(bootHandle != NULL)
    {
        status = Bootloader_parseMultiCoreAppImage(bootHandle, bootImageInfo);

		if(status == SystemP_SUCCESS)
		{
            if(bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS0_0].smpEnable == true)
            {
                bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_A53SS0_0);
                bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS0_1].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_A53SS0_1);
                bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS1_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_A53SS1_0);
                bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS1_1].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_A53SS1_1);

                Bootloader_profileAddCore(CSL_CORE_ID_A53SS0_0);
                Bootloader_profileAddCore(CSL_CORE_ID_A53SS0_1);
                Bootloader_profileAddCore(CSL_CORE_ID_A53SS1_0);
                Bootloader_profileAddCore(CSL_CORE_ID_A53SS1_1);
                status = Bootloader_loadCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS0_0]));
            }
            else
            {
			    bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS0_0].clkHz = Bootloader_socCpuGetClkDefault(CSL_CORE_ID_A53SS0_0);
			    Bootloader_profileAddCore(CSL_CORE_ID_A53SS0_0);
                status = Bootloader_loadCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS0_0]));
            }
		}
	}

	return status;
}

int32_t App_runCpus(Bootloader_Handle bootHandle, Bootloader_BootImageInfo *bootImageInfo)
{
	int32_t status = SystemP_FAILURE;

	status = Bootloader_runCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_HSM_M4FSS0_0]));

	return status;
}

int32_t App_runDSPCpu(Bootloader_Handle bootHandle, Bootloader_BootImageInfo *bootImageInfo)
{
	int32_t status = SystemP_FAILURE;

	status = Bootloader_runCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_C75SS0_0]));

	return status;
}

int32_t App_runA53Cpu(Bootloader_Handle bootHandle, Bootloader_BootImageInfo *bootImageInfo)
{
	int32_t status = SystemP_FAILURE;

    if(bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS0_0].smpEnable == true)
    {
        status = Bootloader_runCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS0_0]));
        status = Bootloader_runCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS0_1]));
        status = Bootloader_runCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS1_0]));
        status = Bootloader_runCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS1_1]));
    }
    else
    {
        status = Bootloader_runCpu(bootHandle, &(bootImageInfo->cpuInfo[CSL_CORE_ID_A53SS0_0]));
    }

	return status;
}

int main()
{
    int32_t status;

    Bootloader_profileReset();

    System_init();
    Bootloader_profileAddProfilePoint("System_init");

    Board_init();
    Bootloader_profileAddProfilePoint("Board_init");

    Drivers_open();
    Bootloader_profileAddProfilePoint("Drivers_open");

    status = Board_driversOpen();
    {
        Bootloader_BootImageInfo bootImageInfo;
		Bootloader_Params bootParams;
        Bootloader_Handle bootHandle;

        Bootloader_BootImageInfo bootImageInfoDM;
		Bootloader_Params bootParamsDM;
        Bootloader_Handle bootHandleDM;

		Bootloader_BootImageInfo bootImageInfoA53;
		Bootloader_Params bootParamsA53;
        Bootloader_Handle bootHandleA53;

        Bootloader_BootImageInfo bootImageInfoDSP;
		Bootloader_Params bootParamsDSP;
        Bootloader_Handle bootHandleDSP;

        Bootloader_Params_init(&bootParams);
        Bootloader_Params_init(&bootParamsDM);
		Bootloader_Params_init(&bootParamsA53);
        Bootloader_Params_init(&bootParamsDSP);

		Bootloader_BootImageInfo_init(&bootImageInfo);
        Bootloader_BootImageInfo_init(&bootImageInfoDM);
		Bootloader_BootImageInfo_init(&bootImageInfoA53);
        Bootloader_BootImageInfo_init(&bootImageInfoDSP);

        bootHandle = Bootloader_open(CONFIG_BOOTLOADER_FLASH_HSM, &bootParams);
        bootHandleDM = Bootloader_open(CONFIG_BOOTLOADER_FLASH_DM, &bootParamsDM);
		bootHandleA53 = Bootloader_open(CONFIG_BOOTLOADER_FLASH_LINUX, &bootParamsA53);
        bootHandleDSP = Bootloader_open(CONFIG_BOOTLOADER_FLASH_DSP, &bootParamsDSP);

        Bootloader_ReservedMemInit(BOOTLOADER_SECOND_STAGE_RESERVED_MEMORY_START, \
                                    BOOTLOADER_SECOND_STAGE_RESERVED_MEMORY_LENGTH);

        if(bootHandle != NULL)
        {
           ((Bootloader_Config *)bootHandle)->scratchMemPtr = gAppimage;
			status = App_loadImages(bootHandle, &bootImageInfo);
            Bootloader_profileAddProfilePoint("App_loadImages");
        }

        if(SystemP_SUCCESS == status)
		{
            if(bootHandleDM != NULL)
            {
                ((Bootloader_Config *)bootHandleDM)->scratchMemPtr = gAppimage;
                status = App_loadSelfcoreImage(bootHandleDM, &bootImageInfoDM);
                Bootloader_profileAddProfilePoint("App_loadSelfcoreImage");
            }
        }

		if(SystemP_SUCCESS == status)
		{
			if(bootHandleA53 != NULL)
			{
                ((Bootloader_Config *)bootHandleA53)->scratchMemPtr = gAppimage;
				status = App_loadA53Images(bootHandleA53, &bootImageInfoA53);
                Bootloader_profileAddProfilePoint("App_loadA53Images");
			}
        }

        if(SystemP_SUCCESS == status)
		{
			if(bootHandleDSP != NULL)
			{
                ((Bootloader_Config *)bootHandleDSP)->scratchMemPtr = gAppimage;
				status = App_loadDSPImage(bootHandleDSP, &bootImageInfoDSP);
                Bootloader_profileAddProfilePoint("App_loadDSPImages");
			}
        }

        Bootloader_profileUpdateAppimageSize(Bootloader_getMulticoreImageSize(bootHandleDM) + \
                                            Bootloader_getMulticoreImageSize(bootHandle) + \
                                            Bootloader_getMulticoreImageSize(bootHandleA53) + \
                                            Bootloader_getMulticoreImageSize(bootHandleDSP));
        Bootloader_profileUpdateMediaAndClk(BOOTLOADER_MEDIA_FLASH, OSPI_getInputClk(gOspiHandle[CONFIG_OSPI0]));

		if(SystemP_SUCCESS == status)
		{
			/* Print SBL log to the same UART port */
			Bootloader_profilePrintProfileLog();
			DebugP_log("Image loading done, switching to application ...\r\n");
			DebugP_log("Starting RTOS/Baremetal applications\r\n");
			UART_flushTxFifo(gUartHandle[CONFIG_UART0]);
		}

        /* Deinitialise the flash peripherial before starting other cores,
           so that other systems can access and reinitialise it.*/
        Board_driversClose();
        Drivers_close();

		if(SystemP_SUCCESS == status)
		{
			status = App_runA53Cpu(bootHandleA53, &bootImageInfoA53);
		}

        Bootloader_close(bootHandleA53);

        if(SystemP_SUCCESS == status)
		{
			status = App_runCpus(bootHandle, &bootImageInfo);
		}

        Bootloader_close(bootHandle);

        if(SystemP_SUCCESS == status)
		{
			status = App_runDSPCpu(bootHandleDSP, &bootImageInfoDSP);
		}

        Bootloader_close(bootHandleDSP);
    }

    if(status != SystemP_SUCCESS )
    {
        DebugP_log("Some tests have failed!!\r\n");
    }

    Bootloader_JumpSelfCpu();

    Board_deinit();
    System_deinit();

    return 0;
}
