/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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

#include "pit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIT_IRQ_ID PIT0_IRQn

#define PIT_IRQ_ID2 PIT1_IRQn

/* Get source clock for PIT driver */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
void PIT_ConfigAndStart(uint8_t channel,uint32_t Fs)
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;

    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 0 */
    uint32_t PIT_SOURCE_CLOCK = CLOCK_GetFreq(kCLOCK_BusClk);
    float counts_for_pit = PIT_SOURCE_CLOCK / Fs;
    uint32_t counts = (uint32_t) counts_for_pit;
    if (kPIT_Chnl_0 == channel)
    {
        PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, counts + 11);

        /* Enable timer interrupts for channel 0 */
        PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

        /* Enable at the NVIC */
        EnableIRQ(PIT_IRQ_ID);

        /* Start channel 0 */
        PIT_StartTimer(PIT, kPIT_Chnl_0);

    } else if (kPIT_Chnl_1 == channel)
    {
        PIT_SetTimerPeriod(PIT, kPIT_Chnl_1,MSEC_TO_COUNT(1000, CLOCK_GetFreq(kCLOCK_BusClk)));
        /* Enable timer interrupts for channel 0 */
        PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
        /* Enable at the NVIC */
        EnableIRQ(PIT_IRQ_ID2);
        NVIC_SetPriority(PIT_IRQ_ID2, 3);
        /* Start channel 0 */
        PIT_StartTimer(PIT, kPIT_Chnl_1);
    }

}
