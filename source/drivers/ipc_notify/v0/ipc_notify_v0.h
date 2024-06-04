/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
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

#ifndef IPC_NOTIFY_V0_H_
#define IPC_NOTIFY_V0_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <drivers/ipc_notify.h>
#include <kernel/dpl/HwiP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/AddrTranslateP.h>

/**
 * \brief This structure describes the information related to one interrupt that is
 *        setup for receiving mailbox messages
 * 
 * One interrupt can be used to handle messages from multiple cores.
 */
typedef struct IpcNotify_InterruptConfig_s
{
    uint32_t intNum;    /**< interrupt number */
    uint32_t eventId;   /**< interrupt event ID, not used for ARM cores */
    HwiP_Object hwiObj; /**< HW interrupt object handle */
    uint8_t numCores;  /**< Number of remote cores attached to this interrupt. */
    uint8_t coreIdList[CSL_CORE_ID_MAX]; /**< List of Core ID's attached to this interrupt 
                                           * 
                                           *   See \ref CSL_CoreID for valid values for this field. 
                                           */  
} IpcNotify_InterruptConfig;

/**
 * \brief This structure describes the mailbox information to send a message from core A to core B
 */ 
typedef struct IpcNotify_MailboxConfig_s
{
    uint8_t mailboxId;   /**< Mailbox to use, mailbox index is specified and base address is derived using \ref gIpcNotifyMailboxBaseAddr */
    uint8_t hwFifoId;    /**< HW fifo within this mailbox to use to exchange the message, 
                            *  used by sender to write to the FIFO, recevier to read from the FIFO */
    uint8_t userId;      /**< user ID to use to receive interrupt for this message exchange, used by receiver only */

} IpcNotify_MailboxConfig;

#if defined (SOC_AM62X)
#define IPC_NOTIFY_MAILBOX_MAX_INSTANCES (1U)
#elif defined (SOC_AM62AX)|| defined (SOC_AM62PX)
#define IPC_NOTIFY_MAILBOX_MAX_INSTANCES (4U)
#endif

/**
 * \brief Array listing the base address of available mailboxes
 * 
 * This is a pre-defined global since this config typically does not need to change based 
 * on end user use-cases for this SOC.
 *
 */
extern uint32_t gIpcNotifyMailboxBaseAddr[IPC_NOTIFY_MAILBOX_MAX_INSTANCES + 1U];

/**
 * \brief Global structure that is pre-defined for this SOC to configure any CPU to any CPU mailbox communication
 * 
 * This is a pre-defined global since this config typically does not need to change based 
 * on end user use-cases for this SOC.
 */
extern IpcNotify_MailboxConfig gIpcNotifyMailboxConfig[CSL_CORE_ID_MAX][CSL_CORE_ID_MAX];

/**
 * \brief Function that is generated via SysConfig to return the core specific configuration
 * 
 * This function is invoked by \ref IpcNotify_init() to get the core specific interrupt configuration.
 * 
 * This function is generated vis SysConfig as this allows to keep the drivers library 
 * independant of the core that it is running on.
 * 
 * Any core specific config and logic is generated by SysConfig
 */
void IpcNotify_getConfig(IpcNotify_InterruptConfig **interruptConfig, uint32_t *interruptConfigNum);
void IpcNotify_syncCallback(uint16_t remoteCoreId, uint16_t localClientId, uint32_t msgValue, void *args);
void IpcNotify_isr(void *args);

#ifdef __cplusplus
}
#endif

#endif /* IPC_NOTIFY_V0_H_ */

