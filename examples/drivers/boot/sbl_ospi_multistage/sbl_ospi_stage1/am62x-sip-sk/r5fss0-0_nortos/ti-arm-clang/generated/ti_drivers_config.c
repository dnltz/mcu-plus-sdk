/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
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
/*
 * Auto generated file
 */

#include "ti_drivers_config.h"
#include <drivers/device_manager/sciclient.h>

/*
 * OSPI
 */


/* Regions restricted for DMA. We should use CPU memcpy in these cases */
static OSPI_AddrRegion gOspiDmaRestrictRegions[] =
{
    {
        .regionStartAddr = CSL_WKUP_R5FSS0_ATCM_BASE,
        .regionSize      = CSL_WKUP_R5FSS0_ATCM_SIZE,
    },
    {
        .regionStartAddr = CSL_WKUP_R5FSS0_BTCM_BASE,
        .regionSize      = CSL_WKUP_R5FSS0_BTCM_SIZE,
    },
    {
        .regionStartAddr = CSL_MCU_M4FSS0_IRAM_BASE,
        .regionSize      = CSL_MCU_M4FSS0_IRAM_SIZE,
    },
    {
        .regionStartAddr = CSL_MCU_M4FSS0_DRAM_BASE,
        .regionSize      = CSL_MCU_M4FSS0_DRAM_SIZE,
    },
    {
        .regionStartAddr = 0xFFFFFFFFU,
        .regionSize      = 0U,
    }
};

/* OSPI attributes */
static OSPI_Attrs gOspiAttrs[CONFIG_OSPI_NUM_INSTANCES] =
{
    {
        .baseAddr             = CSL_FSS0_OSPI0_CTRL_BASE,
        .dataBaseAddr         = CSL_FSS0_DAT_REG1_BASE,
        .inputClkFreq         = 166666666U,
        .intrNum              = 171U,
        .intrEnable           = FALSE,
        .intrPriority         = 4U,
        .dmaEnable            = TRUE,
        .phyEnable            = TRUE,
        .phySkipTuning        = FALSE,
        .dacEnable            = FALSE,
        .chipSelect           = OSPI_CS0,
        .frmFmt               = OSPI_FF_POL0_PHA0,
        .decChipSelect        = OSPI_DECODER_SELECT4,
        .baudRateDiv          = 4,
        .phaseDelayElement    = 3,
        .dmaRestrictedRegions = gOspiDmaRestrictRegions,
        .phyConfiguration     = {
            .phyControlMode = OSPI_FLASH_CFG_PHY_MASTER_CONTROL_REG_PHY_MASTER_MODE,
            .dllLockMode    = OSPI_PHY_DLL_HALF_CYCLE_LOCK,
            .tuningWindowParams = {
                .txDllLowWindowStart    = 28,
                .txDllLowWindowEnd      = 48,
                .txDllHighWindowStart   = 60,
                .txDllHighWindowEnd     = 96,
                .rxLowSearchStart       = 0,
                .rxLowSearchEnd         = 40,
                .rxHighSearchStart      = 24,
                .rxHighSearchEnd        = 107,
                .txLowSearchStart       = 16,
                .txLowSearchEnd         = 64,
                .txHighSearchStart      = 78,
                .txHighSearchEnd        = 127,
                .txDLLSearchOffset      = 8,
                .rxTxDLLSearchStep      = 8,
                .rdDelayMin             = 1,
                .rdDelayMax             = 2,
            }
        }
    },
};
/* OSPI objects - initialized by the driver */
static OSPI_Object gOspiObjects[CONFIG_OSPI_NUM_INSTANCES];
/* OSPI driver configuration */
OSPI_Config gOspiConfig[CONFIG_OSPI_NUM_INSTANCES] =
{
    {
        &gOspiAttrs[CONFIG_OSPI0],
        &gOspiObjects[CONFIG_OSPI0],
    },
};

uint32_t gOspiConfigNum = CONFIG_OSPI_NUM_INSTANCES;

#include <drivers/ospi/v0/dma/ospi_dma.h>
#include <drivers/ospi/v0/dma/udma/ospi_dma_udma.h>
#include <drivers/udma.h>

/*
 * OSPI UDMA Blockcopy Parameters
 */
#define OSPI_UDMA_BLK_COPY_CH_RING_ELEM_CNT (1U)
#define OSPI_UDMA_BLK_COPY_CH_RING_MEM_SIZE (((OSPI_UDMA_BLK_COPY_CH_RING_ELEM_CNT * 8U) + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))
#define OSPI_UDMA_BLK_COPY_CH_TRPD_MEM_SIZE (UDMA_GET_TRPD_TR15_SIZE(1U))
#define OSPI_UDMA_NUM_BLKCOPY_CH (1U)

/* OSPI UDMA Blockcopy Channel Objects */
static Udma_ChObject gOspiUdmaBlkCopyChObj[OSPI_UDMA_NUM_BLKCOPY_CH];

/* OSPI UDMA Blockcopy Channel Ring Mem */
static uint8_t gOspiUdmaBlkCopyCh0RingMem[OSPI_UDMA_BLK_COPY_CH_RING_MEM_SIZE] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/* OSPI UDMA Blockcopy Channel TRPD Mem */
static uint8_t gOspiUdmaBlkCopyCh0TrpdMem[OSPI_UDMA_BLK_COPY_CH_TRPD_MEM_SIZE] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

OspiDma_UdmaArgs gOspiUdma0Args =
{
    .drvHandle     = &gUdmaDrvObj[CONFIG_UDMA0],
    .chHandle      = &gOspiUdmaBlkCopyChObj[0],
    .trpdMem       = &gOspiUdmaBlkCopyCh0TrpdMem,
    .trpdMemSize   = OSPI_UDMA_BLK_COPY_CH_TRPD_MEM_SIZE,
    .ringMem       = &gOspiUdmaBlkCopyCh0RingMem,
    .ringMemSize   = OSPI_UDMA_BLK_COPY_CH_RING_MEM_SIZE,
    .ringElemCount = OSPI_UDMA_BLK_COPY_CH_RING_ELEM_CNT,
};
OSPI_DmaConfig gOspiDmaConfig[CONFIG_OSPI_NUM_DMA_INSTANCES] =
{
    {
        .fxns        = &gOspiDmaUdmaFxns,
        .ospiDmaArgs = (void *)&gOspiUdma0Args,
    }
};

uint32_t gOspiDmaConfigNum = CONFIG_OSPI_NUM_DMA_INSTANCES;


/*
 * BOOTLOADER
 */
/* Include ti_board_config.h for flash config macros */
#include "ti_board_config.h"

/* Bootloader boot media specific arguments */
Bootloader_FlashArgs gBootloader0Args =
{
    .flashIndex     = CONFIG_FLASH0,
    .curOffset      = 0,
    .appImageOffset = 0x100000,
    .flashType      = CONFIG_FLASH_TYPE_SERIAL_NOR,
};
Bootloader_FlashArgs gBootloader1Args =
{
    .flashIndex     = CONFIG_FLASH0,
    .curOffset      = 0,
    .appImageOffset = 0x80000,
    .flashType      = CONFIG_FLASH_TYPE_SERIAL_NOR,
};

/* Configuration option for lockstep or standalone */
void* operatingMode = NULL;


/* Bootloader DMA attributes */
#include <drivers/udma.h>
#include <drivers/bootloader/bootloader_dma.h>


/* Regions restricted for DMA. We should use CPU memcpy in these cases */
static Bootloader_AddrRegion gDmaRestrictRegions[] =
{
    {
        .regionStartAddr = CSL_WKUP_R5FSS0_ATCM_BASE,
        .regionSize      = CSL_WKUP_R5FSS0_ATCM_SIZE,
    },
    {
        .regionStartAddr = CSL_WKUP_R5FSS0_BTCM_BASE,
        .regionSize      = CSL_WKUP_R5FSS0_BTCM_SIZE,
    },
    {
        .regionStartAddr = CSL_MCU_M4FSS0_IRAM_BASE,
        .regionSize      = CSL_MCU_M4FSS0_IRAM_SIZE,
    },
    {
        .regionStartAddr = CSL_MCU_M4FSS0_DRAM_BASE,
        .regionSize      = CSL_MCU_M4FSS0_DRAM_SIZE,
    },
    {
        .regionStartAddr = 0xFFFFFFFFU,
        .regionSize      = 0U,
    }
};

#define BOOTLOADER_UDMA_BLK_COPY_CH_RING_ELEM_CNT   (1U)
#define BOOTLOADER_UDMA_BLK_COPY_CH_RING_MEM_SIZE   (((BOOTLOADER_UDMA_BLK_COPY_CH_RING_ELEM_CNT * 8U) + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))
#define BOOTLOADER_UDMA_BLK_COPY_CH_TRPD_MEM_SIZE   (UDMA_GET_TRPD_TR15_SIZE(1U))
#define BOOTLOADER_UDMA_NUM_BLKCOPY_CH              (1U)

/* BOOTLOADER UDMA Blockcopy Channel Objects */
static Udma_ChObject gBootloaderUdmaBlkCpyChObj[BOOTLOADER_UDMA_NUM_BLKCOPY_CH];

/* BOOTLOADER UDMA Blockcopy Channel Ring Mem */
static uint8_t gBootloaderUdmaBlkCpyCh0RingMem[BOOTLOADER_UDMA_BLK_COPY_CH_RING_MEM_SIZE] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/* BOOTLOADER UDMA Blockcopy Channel TRPD Mem */
static uint8_t gBootloaderUdmaBlkCpyCh0TrpdMem[BOOTLOADER_UDMA_BLK_COPY_CH_TRPD_MEM_SIZE] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

BootloaderDma_UdmaArgs gBootloaderUdmaArgs =
{
    .drvHandle     = &gUdmaDrvObj[CONFIG_UDMA0],
    .chHandle      = &gBootloaderUdmaBlkCpyChObj[0],
    .trpdMem       = &gBootloaderUdmaBlkCpyCh0TrpdMem,
    .trpdMemSize   = BOOTLOADER_UDMA_BLK_COPY_CH_TRPD_MEM_SIZE,
    .ringMem       = &gBootloaderUdmaBlkCpyCh0RingMem,
    .ringMemSize   = BOOTLOADER_UDMA_BLK_COPY_CH_RING_MEM_SIZE,
    .ringElemCount = BOOTLOADER_UDMA_BLK_COPY_CH_RING_ELEM_CNT,
    .restrictedRegions = &gDmaRestrictRegions,
};


/* Arguments to be used for driver implementation callbacks when boot media is SOC memory */
Bootloader_MemArgs gMemBootloaderArgs =
{
    .curOffset = 0,
    .appImageBaseAddr = 0,
    .enableDmaTransfer = true,
    .bootloaderDma_UdmaArgs = &gBootloaderUdmaArgs,
};

/* Bootloader driver configuration */
Bootloader_Config gBootloaderConfig[CONFIG_BOOTLOADER_NUM_INSTANCES] =
{
    {
        &gBootloaderFlashFxns,
        &gBootloader0Args,
        BOOTLOADER_MEDIA_FLASH,
        0,
        0,
        NULL,
        .socCoreOpMode= (void *)&operatingMode,
    },
    {
        &gBootloaderFlashFxns,
        &gBootloader1Args,
        BOOTLOADER_MEDIA_FLASH,
        0,
        0,
        NULL,
        .socCoreOpMode= (void *)&operatingMode,
    },
};

/* Bootloader driver configuration specifically for SOC memory */
Bootloader_Config gMemBootloaderConfig =
{
        &gBootloaderMemFxns,
        &gMemBootloaderArgs,
        BOOTLOADER_MEDIA_MEM,
        0,
        0,
        NULL,
        .socCoreOpMode= (void *)&operatingMode,
        TRUE,
};

uint32_t gBootloaderConfigNum = CONFIG_BOOTLOADER_NUM_INSTANCES;

/* DDR */

/* DDR register config .h file as generated from DDR SUBSYSTEM REGISTER CONFIGURATION tool
 * Make sure path to this file is specified in your application project/makefile include path
 */
#include "drivers/ddr/v0/soc/am62x/am62x_SIP_ddrReginit-50-800.h"


static DDR_Params gDdrParams =
{
    /* below values are set using the globals defined in drivers/ddr/v0/soc/am62x/am62x_SIP_ddrReginit-50-800.h */
    .clk1Freq              = DDRSS_PLL_FREQUENCY_1,
    .clk2Freq              = DDRSS_PLL_FREQUENCY_2,
    .ddrssCtlReg           = DDRSS_ctlReg,
    .ddrssPhyIndepReg      = DDRSS_phyIndepReg,
    .ddrssPhyReg           = DDRSS_phyReg,
    .ddrssCtlRegNum        = DDRSS_ctlRegNum,
    .ddrssPhyIndepRegNum   = DDRSS_phyIndepRegNum,
    .ddrssPhyRegNum        = DDRSS_phyRegNum,
    .ddrssCtlRegCount      = DDRSS_CTL_REG_INIT_COUNT,
    .ddrssPhyIndepRegCount = DDRSS_PHY_INDEP_REG_INIT_COUNT,
    .ddrssPhyRegCount      = DDRSS_PHY_REG_INIT_COUNT,
	.fshcount              = DDRSS_PLL_FHS_CNT,
    .sdramIdx              = 15U,
    .enableEccFlag = 0,
    .eccRegion = NULL,
};


/*
 * SDL Interface functions for BIST modules
 */

static HwiP_Object gSDLHwiObject;

static pSDL_DPL_HwipHandle SDL_registerInterrupt(SDL_DPL_HwipParams *pParams)
{
    HwiP_Params hwipParams;
    HwiP_Params_init(&hwipParams);

    hwipParams.args = (void *)pParams->callbackArg;
    hwipParams.intNum = pParams->intNum;
    hwipParams.callback = pParams->callback;

    HwiP_construct(&gSDLHwiObject, &hwipParams);

    return &gSDLHwiObject;
}

static int32_t SDL_deregisterInterrupt(pSDL_DPL_HwipHandle handle)
{
    HwiP_destruct(handle);
    return SDL_PASS;
}

static int32_t SDL_enableInterrupt(uint32_t intNum)
{
    HwiP_enableInt(intNum);
    return SDL_PASS;
}

static int32_t SDL_disableInterrupt(uint32_t intNum)
{
    HwiP_disableInt(intNum);
    return SDL_PASS;
}

static void* SDL_addrTranslate(uint64_t addr, uint32_t size)
{
    uint32_t transAddr = (uint32_t)(-1);

    transAddr = (uint32_t)AddrTranslateP_getLocalAddr(addr);

    return (void *)transAddr;
}


static SDL_DPL_Interface dpl_interface =
{
    .enableInterrupt = (pSDL_DPL_InterruptFunction) SDL_enableInterrupt,
    .disableInterrupt = (pSDL_DPL_InterruptFunction) SDL_disableInterrupt,
    .registerInterrupt = (pSDL_DPL_RegisterFunction) SDL_registerInterrupt,
    .deregisterInterrupt = (pSDL_DPL_DeregisterFunction) SDL_deregisterInterrupt,
    .delay = (pSDL_DPL_DelayFunction) ClockP_sleep,
    .addrTranslate = (pSDL_DPL_AddrTranslateFunction) SDL_addrTranslate
};
/*
 * UDMA
 */
/* UDMA driver instance object */
Udma_DrvObject          gUdmaDrvObj[CONFIG_UDMA_NUM_INSTANCES];
/* UDMA driver instance init params */
static Udma_InitPrms    gUdmaInitPrms[CONFIG_UDMA_NUM_INSTANCES] =
{
    {
        .instId             = UDMA_INST_ID_BCDMA_0,
        .skipGlobalEventReg = FALSE,
        .virtToPhyFxn       = Udma_defaultVirtToPhyFxn,
        .phyToVirtFxn       = Udma_defaultPhyToVirtFxn,
    },
};

/*
 * UART
 */

/* UART atrributes */
static UART_Attrs gUartAttrs[CONFIG_UART_NUM_INSTANCES] =
{
    {
        .baseAddr           = CSL_UART0_BASE,
        .inputClkFreq       = 48000000U,
    },
};
/* UART objects - initialized by the driver */
static UART_Object gUartObjects[CONFIG_UART_NUM_INSTANCES];
/* UART driver configuration */
UART_Config gUartConfig[CONFIG_UART_NUM_INSTANCES] =
{
    {
        .attrs = &gUartAttrs[CONFIG_UART0],
        .object = &gUartObjects[CONFIG_UART0],
        .traceInstance = FALSE,
    },
};

uint32_t gUartConfigNum = CONFIG_UART_NUM_INSTANCES;

#include <drivers/uart/v0/dma/uart_dma.h>


UART_DmaConfig gUartDmaConfig[CONFIG_UART_NUM_DMA_INSTANCES] =
{
};

uint32_t gUartDmaConfigNum = CONFIG_UART_NUM_DMA_INSTANCES;


void Drivers_uartInit(void)
{
    UART_init();
}


void Pinmux_init();
void PowerClock_init(void);
void PowerClock_deinit(void);

/*
 * Common Functions
 */
void System_init(void)
{
    /* DPL init sets up address transalation unit, on some CPUs this is needed
     * to access SCICLIENT services, hence this needs to happen first
     */
    Dpl_init();
    /* We should do sciclient init before we enable power and clock to the peripherals */
    /* SCICLIENT init */
    {

        int32_t retVal = SystemP_SUCCESS;

        retVal = Sciclient_direct_init();
        DebugP_assertNoLog(SystemP_SUCCESS == retVal);

    }

   


    PowerClock_init();
    /* Now we can do pinmux */
    Pinmux_init();
    /* finally we initialize all peripheral drivers */
    OSPI_init();
	DDR_init(&gDdrParams);

	GTC_init();

    /* BIST */
    {
        int32_t status;

        /* Initialize SDL DPL */
        status = SDL_DPL_init(&dpl_interface);
        DebugP_assert(status == SDL_PASS);

        /* Unlock MMRs */
        SOC_controlModuleUnlockMMR(SOC_DOMAIN_ID_MAIN, 4u);
        SOC_controlModuleUnlockMMR(SOC_DOMAIN_ID_MCU, 3u);


        /* Start MCU BIST only if the reset isolation is disabled */
        if (!Bootloader_socIsMCUResetIsoEnabled())
        {
            /* Start MCU PBIST */
            status = SDL_SBL_PBIST_selfTest(SDL_PBIST_INST_MCU);
            DebugP_assert(status == SDL_PASS);
        }
    }

    /* UDMA */
    {
        uint32_t        instId;
        int32_t         retVal = UDMA_SOK;

        for(instId = 0U; instId < CONFIG_UDMA_NUM_INSTANCES; instId++)
        {
            retVal += Udma_init(&gUdmaDrvObj[instId], &gUdmaInitPrms[instId]);
            DebugP_assert(UDMA_SOK == retVal);
        }
    }
    Drivers_uartInit();
}

void System_deinit(void)
{
    OSPI_deinit();
    /* UDMA */
    {
        uint32_t        instId;
        int32_t         retVal = UDMA_SOK;

        for(instId = 0U; instId < CONFIG_UDMA_NUM_INSTANCES; instId++)
        {
            retVal += Udma_deinit(&gUdmaDrvObj[instId]);
            DebugP_assert(UDMA_SOK == retVal);
        }
    }
    UART_deinit();
    PowerClock_deinit();
    /* SCICLIENT deinit */
    {
        int32_t         retVal = SystemP_SUCCESS;

        retVal = Sciclient_deinit();
        DebugP_assertNoLog(SystemP_SUCCESS == retVal);
    }
    Dpl_deinit();
}
