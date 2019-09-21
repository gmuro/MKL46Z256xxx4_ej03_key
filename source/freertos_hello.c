/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS kernel includes. */
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"

#include "board_dsi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct
{
    board_ledId_enum idLed;
    board_swId_enum idSw;
    uint32_t semiPeriodo;
}paramBlinkLed_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void blinky_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */

static const paramBlinkLed_t paramBlinkLed_rojo =
{
    .idLed = BOARD_LED_ID_ROJO,
    .idSw = BOARD_SW_ID_1,
    .semiPeriodo = 500,
};

static const paramBlinkLed_t paramBlinkLed_verde =
{
    .idLed = BOARD_LED_ID_VERDE,
    .idSw = BOARD_SW_ID_3,
    .semiPeriodo = 400,
};

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    board_init();

    key_init();

    xTaskCreate(blinky_task, "blinkR", 100, (void * const)&paramBlinkLed_rojo, 1, NULL);
    xTaskCreate(blinky_task, "blinkV", 100, (void * const)&paramBlinkLed_verde, 1, NULL);

    vTaskStartScheduler();

    for (;;);
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void blinky_task(void *pvParameters)
{
    paramBlinkLed_t *paramBlinkLed;

    paramBlinkLed = (paramBlinkLed_t*)pvParameters;

    for (;;)
    {
        key_waitForPressEv(paramBlinkLed->idSw, portMAX_DELAY);

        board_setLed(paramBlinkLed->idLed, BOARD_LED_MSG_ON);

        vTaskDelay(paramBlinkLed->semiPeriodo / portTICK_PERIOD_MS);

        board_setLed(paramBlinkLed->idLed, BOARD_LED_MSG_OFF);
    }
}

void vApplicationTickHook(void)
{
    key_periodicTask1ms();
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while(1);
}









