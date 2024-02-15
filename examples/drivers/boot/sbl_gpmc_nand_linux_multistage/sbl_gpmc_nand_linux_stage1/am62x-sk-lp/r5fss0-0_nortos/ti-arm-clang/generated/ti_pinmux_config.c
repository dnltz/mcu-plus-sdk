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
            /* GPMC0 pin config */
    /* GPMC0_CSn0 -> GPMC0_CSn0 (M21) */
    {
        PIN_GPMC0_CSN0,
        ( PIN_MODE(0) | PIN_PULL_DISABLE )
    },
    /* GPMC0_OEn_REn -> GPMC0_OEn_REn (L24) */
    {
        PIN_GPMC0_OEN_REN,
        ( PIN_MODE(0) | PIN_PULL_DISABLE )
    },
    /* GPMC0_WAIT0 -> GPMC0_WAIT0 (U23) */
    {
        PIN_GPMC0_WAIT0,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* GPMC0_WEn -> GPMC0_WEn (L25) */
    {
        PIN_GPMC0_WEN,
        ( PIN_MODE(0) | PIN_PULL_DISABLE )
    },
    /* GPMC0_BE0n_CLE -> GPMC0_BE0n_CLE (M24) */
    {
        PIN_GPMC0_BE0N_CLE,
        ( PIN_MODE(0) | PIN_PULL_DISABLE )
    },
    /* GPMC0_ADVn_ALE -> GPMC0_ADVn_ALE (L23) */
    {
        PIN_GPMC0_ADVN_ALE,
        ( PIN_MODE(0) | PIN_PULL_DISABLE )
    },
    /* GPMC0_AD0 -> GPMC0_AD0 (M25) */
    {
        PIN_GPMC0_AD0,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* GPMC0_AD1 -> GPMC0_AD1 (N23) */
    {
        PIN_GPMC0_AD1,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* GPMC0_AD2 -> GPMC0_AD2 (N24) */
    {
        PIN_GPMC0_AD2,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* GPMC0_AD3 -> GPMC0_AD3 (N25) */
    {
        PIN_GPMC0_AD3,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* GPMC0_AD4 -> GPMC0_AD4 (P24) */
    {
        PIN_GPMC0_AD4,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* GPMC0_AD5 -> GPMC0_AD5 (P22) */
    {
        PIN_GPMC0_AD5,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* GPMC0_AD6 -> GPMC0_AD6 (P21) */
    {
        PIN_GPMC0_AD6,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
    },
    /* GPMC0_AD7 -> GPMC0_AD7 (R23) */
    {
        PIN_GPMC0_AD7,
        ( PIN_MODE(0) | PIN_INPUT_ENABLE | PIN_PULL_DISABLE )
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

