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

#include <drivers/gpio.h>
#include <drivers/ipc_notify.h>
#include <drivers/ipc_rpmsg.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"


volatile uint8_t gbShutdown = 0u;
volatile uint8_t gbShutdownRemotecoreID = 0u;

void ipc_rp_mbox_callback(uint16_t remoteCoreId, uint16_t clientId, uint32_t msgValue, void *args)
{
    if (clientId == IPC_NOTIFY_CLIENT_ID_RP_MBOX)
    {
        if (msgValue == IPC_NOTIFY_RP_MBOX_SHUTDOWN) /* Shutdown request from the remotecore */
        {
            gbShutdown = 1u;
            gbShutdownRemotecoreID = remoteCoreId;
        }
    }
}

/*
 * This example configures a GPIO pin connected to an LED on the EVM in
 * output mode.
 * The application toggles the LED on/off for 10 seconds and exits.
 */

void gpio_led_blink_main(void *args)
{
    int32_t status;
    uint32_t gpioBaseAddr, pinNum;

    DebugP_log("GPIO LED Blink Test Started\r\n");

    status = RPMessage_waitForLinuxReady(SystemP_WAIT_FOREVER);
    DebugP_assert(status==SystemP_SUCCESS);

    status = IpcNotify_registerClient(IPC_NOTIFY_CLIENT_ID_RP_MBOX, &ipc_rp_mbox_callback, NULL);
    DebugP_assert(status==SystemP_SUCCESS);

    /* Get address after translation translate */
    gpioBaseAddr = (uint32_t) AddrTranslateP_getLocalAddr(GPIO_LED_BASE_ADDR);
    pinNum       = GPIO_LED_PIN;

    GPIO_setDirMode(gpioBaseAddr, pinNum, GPIO_LED_DIR);
    while(1)
    {
        GPIO_pinWriteHigh(gpioBaseAddr, pinNum);
        ClockP_usleep(150000);
        GPIO_pinWriteLow(gpioBaseAddr, pinNum);
        ClockP_usleep(50000);
        GPIO_pinWriteHigh(gpioBaseAddr, pinNum);
        ClockP_usleep(150000);
        GPIO_pinWriteLow(gpioBaseAddr, pinNum);
        ClockP_sleep(1);

        if (gbShutdown == 1u)
            break;
    }

    DebugP_log("GPIO LED Blink Test Stopped\r\n");

    Drivers_close();
    System_deinit();
    IpcNotify_sendMsg(gbShutdownRemotecoreID, IPC_NOTIFY_CLIENT_ID_RP_MBOX,
		      IPC_NOTIFY_RP_MBOX_SHUTDOWN_ACK, 1u);
    __asm__ __volatile__ ("wfi"   "\n\t": : : "memory");
}
