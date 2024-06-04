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
#include <drivers/pinmux.h>

static Pinmux_PerCfg_t gPinMuxMainDomainCfg[] = {
            /* MMC0 pin config */
    /* MMC0_CLK -> MMC0_CLK (AB1) */
    {
        PIN_MMC0_CLK,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* MMC0_CMD -> MMC0_CMD (Y3) */
    {
        PIN_MMC0_CMD,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* MMC0_DAT0 -> MMC0_DAT0 (AA2) */
    {
        PIN_MMC0_DAT0,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* MMC0_DAT1 -> MMC0_DAT1 (AA1) */
    {
        PIN_MMC0_DAT1,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DIRECTION )
    },
    /* MMC0_DAT2 -> MMC0_DAT2 (AA3) */
    {
        PIN_MMC0_DAT2,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DIRECTION )
    },
    /* MMC0_DAT3 -> MMC0_DAT3 (Y4) */
    {
        PIN_MMC0_DAT3,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DIRECTION )
    },
    /* MMC0_DAT4 -> MMC0_DAT4 (AB2) */
    {
        PIN_MMC0_DAT4,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DIRECTION )
    },
    /* MMC0_DAT5 -> MMC0_DAT5 (AC1) */
    {
        PIN_MMC0_DAT5,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DIRECTION )
    },
    /* MMC0_DAT6 -> MMC0_DAT6 (AD2) */
    {
        PIN_MMC0_DAT6,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DIRECTION )
    },
    /* MMC0_DAT7 -> MMC0_DAT7 (AC2) */
    {
        PIN_MMC0_DAT7,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DIRECTION )
    },

            /* USART0 pin config */
    /* UART0_RXD -> UART0_RXD (D14) */
    {
        PIN_UART0_RXD,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* UART0_TXD -> UART0_TXD (E14) */
    {
        PIN_UART0_TXD,
        ( PIN_MODE(0) | PIN_PULL_DISABLE )
    },

            /* USART1 pin config */
    /* UART1_RXD -> MCASP0_AFSR (E19) */
    {
        PIN_MCASP0_AFSR,
        ( PIN_MODE(2) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* UART1_TXD -> MCASP0_ACLKR (A20) */
    {
        PIN_MCASP0_ACLKR,
        ( PIN_MODE(2) | PIN_PULL_DISABLE )
    },
    /* UART1_RTSn -> MCASP0_AXR2 (A19) */
    {
        PIN_MCASP0_AXR2,
        ( PIN_MODE(2) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* UART1_CTSn -> MCASP0_AXR3 (B19) */
    {
        PIN_MCASP0_AXR3,
        ( PIN_MODE(2) | PIN_PULL_DISABLE )
    },

    {PINMUX_END, PINMUX_END}
};

static Pinmux_PerCfg_t gPinMuxMcuDomainCfg[] = {
        
        
        
    {PINMUX_END, PINMUX_END}
};

/*
 * Pinmux
 */
void Pinmux_init(void)
{
    Pinmux_config(gPinMuxMainDomainCfg, PINMUX_DOMAIN_ID_MAIN);
    Pinmux_config(gPinMuxMcuDomainCfg, PINMUX_DOMAIN_ID_MCU);
}

