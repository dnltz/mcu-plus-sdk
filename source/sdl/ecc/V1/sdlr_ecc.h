/********************************************************************
 * Copyright (C) 2022 Texas Instruments Incorporated.
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
 *  Name        : sdlr_ecc.h
*/
#ifndef SDLR_ECC_AGGR_H_
#define SDLR_ECC_AGGR_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <sdl/sdlr.h>
#include <stdint.h>

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t REV;                       /* Aggregator Revision Register */
    volatile uint8_t  Resv_8[4];
    volatile uint32_t VECTOR;                    /* ECC Vector Register */
    volatile uint32_t STAT;                      /* Misc Status */
    volatile uint32_t RESERVED_SVBUS[8];         /* Reserved Area for Serial VBUS Registers */
    volatile uint8_t  Resv_60[12];
    volatile uint32_t SEC_EOI_REG;               /* EOI Register */
    volatile uint32_t SEC_STATUS_REG0;           /* Interrupt Status Register 0 */
    volatile uint8_t  Resv_128[60];
    volatile uint32_t SEC_ENABLE_SET_REG0;       /* Interrupt Enable Set Register 0 */
    volatile uint8_t  Resv_192[60];
    volatile uint32_t SEC_ENABLE_CLR_REG0;       /* Interrupt Enable Clear Register 0 */
    volatile uint8_t  Resv_316[120];
    volatile uint32_t DED_EOI_REG;               /* EOI Register */
    volatile uint32_t DED_STATUS_REG0;           /* Interrupt Status Register 0 */
    volatile uint8_t  Resv_384[60];
    volatile uint32_t DED_ENABLE_SET_REG0;       /* Interrupt Enable Set Register 0 */
    volatile uint8_t  Resv_448[60];
    volatile uint32_t DED_ENABLE_CLR_REG0;       /* Interrupt Enable Clear Register 0 */
    volatile uint8_t  Resv_512[60];
    volatile uint32_t AGGR_ENABLE_SET;           /* AGGR interrupt enable set Register */
    volatile uint32_t AGGR_ENABLE_CLR;           /* AGGR interrupt enable clear Register */
    volatile uint32_t AGGR_STATUS_SET;           /* AGGR interrupt status set Register */
    volatile uint32_t AGGR_STATUS_CLR;           /* AGGR interrupt status clear Register */
} SDL_ecc_aggrRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define SDL_ECC_AGGR_REV                                                       (0x00000000U)
#define SDL_ECC_AGGR_VECTOR                                                    (0x00000008U)
#define SDL_ECC_AGGR_STAT                                                      (0x0000000CU)
#define SDL_ECC_AGGR_RESERVED_SVBUS(RESERVED_SVBUS)                            (0x00000010U+((RESERVED_SVBUS)*0x4U))
#define SDL_ECC_AGGR_SEC_EOI_REG                                               (0x0000003CU)
#define SDL_ECC_AGGR_SEC_STATUS_REG0                                           (0x00000040U)
#define SDL_ECC_AGGR_SEC_ENABLE_SET_REG0                                       (0x00000080U)
#define SDL_ECC_AGGR_SEC_ENABLE_CLR_REG0                                       (0x000000C0U)
#define SDL_ECC_AGGR_DED_EOI_REG                                               (0x0000013CU)
#define SDL_ECC_AGGR_DED_STATUS_REG0                                           (0x00000140U)
#define SDL_ECC_AGGR_DED_ENABLE_SET_REG0                                       (0x00000180U)
#define SDL_ECC_AGGR_DED_ENABLE_CLR_REG0                                       (0x000001C0U)
#define SDL_ECC_AGGR_AGGR_ENABLE_SET                                           (0x00000200U)
#define SDL_ECC_AGGR_AGGR_ENABLE_CLR                                           (0x00000204U)
#define SDL_ECC_AGGR_AGGR_STATUS_SET                                           (0x00000208U)
#define SDL_ECC_AGGR_AGGR_STATUS_CLR                                           (0x0000020CU)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* REV */

#define SDL_ECC_AGGR_REV_SCHEME_MASK                                           (0xC0000000U)
#define SDL_ECC_AGGR_REV_SCHEME_SHIFT                                          (0x0000001EU)
#define SDL_ECC_AGGR_REV_SCHEME_MAX                                            (0x00000003U)

#define SDL_ECC_AGGR_REV_BU_MASK                                               (0x30000000U)
#define SDL_ECC_AGGR_REV_BU_SHIFT                                              (0x0000001CU)
#define SDL_ECC_AGGR_REV_BU_MAX                                                (0x00000003U)

#define SDL_ECC_AGGR_REV_MODULE_ID_MASK                                        (0x0FFF0000U)
#define SDL_ECC_AGGR_REV_MODULE_ID_SHIFT                                       (0x00000010U)
#define SDL_ECC_AGGR_REV_MODULE_ID_MAX                                         (0x00000FFFU)

#define SDL_ECC_AGGR_REV_REVRTL_MASK                                           (0x0000F800U)
#define SDL_ECC_AGGR_REV_REVRTL_SHIFT                                          (0x0000000BU)
#define SDL_ECC_AGGR_REV_REVRTL_MAX                                            (0x0000001FU)

#define SDL_ECC_AGGR_REV_REVMAJ_MASK                                           (0x00000700U)
#define SDL_ECC_AGGR_REV_REVMAJ_SHIFT                                          (0x00000008U)
#define SDL_ECC_AGGR_REV_REVMAJ_MAX                                            (0x00000007U)

#define SDL_ECC_AGGR_REV_CUSTOM_MASK                                           (0x000000C0U)
#define SDL_ECC_AGGR_REV_CUSTOM_SHIFT                                          (0x00000006U)
#define SDL_ECC_AGGR_REV_CUSTOM_MAX                                            (0x00000003U)

#define SDL_ECC_AGGR_REV_REVMIN_MASK                                           (0x0000003FU)
#define SDL_ECC_AGGR_REV_REVMIN_SHIFT                                          (0x00000000U)
#define SDL_ECC_AGGR_REV_REVMIN_MAX                                            (0x0000003FU)

/* VECTOR */

#define SDL_ECC_AGGR_VECTOR_ECC_VECTOR_MASK                                    (0x000007FFU)
#define SDL_ECC_AGGR_VECTOR_ECC_VECTOR_SHIFT                                   (0x00000000U)
#define SDL_ECC_AGGR_VECTOR_ECC_VECTOR_MAX                                     (0x000007FFU)

#define SDL_ECC_AGGR_VECTOR_RD_SVBUS_MASK                                      (0x00008000U)
#define SDL_ECC_AGGR_VECTOR_RD_SVBUS_SHIFT                                     (0x0000000FU)
#define SDL_ECC_AGGR_VECTOR_RD_SVBUS_MAX                                       (0x00000001U)

#define SDL_ECC_AGGR_VECTOR_RD_SVBUS_ADDRESS_MASK                              (0x00FF0000U)
#define SDL_ECC_AGGR_VECTOR_RD_SVBUS_ADDRESS_SHIFT                             (0x00000010U)
#define SDL_ECC_AGGR_VECTOR_RD_SVBUS_ADDRESS_MAX                               (0x000000FFU)

#define SDL_ECC_AGGR_VECTOR_RD_SVBUS_DONE_MASK                                 (0x01000000U)
#define SDL_ECC_AGGR_VECTOR_RD_SVBUS_DONE_SHIFT                                (0x00000018U)
#define SDL_ECC_AGGR_VECTOR_RD_SVBUS_DONE_MAX                                  (0x00000001U)

/* STAT */

#define SDL_ECC_AGGR_STAT_NUM_RAMS_MASK                                        (0x000007FFU)
#define SDL_ECC_AGGR_STAT_NUM_RAMS_SHIFT                                       (0x00000000U)
#define SDL_ECC_AGGR_STAT_NUM_RAMS_MAX                                         (0x000007FFU)

/* RESERVED_SVBUS */

#define SDL_ECC_AGGR_RESERVED_SVBUS_DATA_MASK                                  (0xFFFFFFFFU)
#define SDL_ECC_AGGR_RESERVED_SVBUS_DATA_SHIFT                                 (0x00000000U)
#define SDL_ECC_AGGR_RESERVED_SVBUS_DATA_MAX                                   (0xFFFFFFFFU)

/* SEC_EOI_REG */

#define SDL_ECC_AGGR_SEC_EOI_REG_EOI_WR_MASK                                   (0x00000001U)
#define SDL_ECC_AGGR_SEC_EOI_REG_EOI_WR_SHIFT                                  (0x00000000U)
#define SDL_ECC_AGGR_SEC_EOI_REG_EOI_WR_MAX                                    (0x00000001U)

/* SEC_STATUS_REG0 */

#define SDL_ECC_AGGR_SEC_STATUS_REG0_RAMECC0_PEND_MASK                         (0x00000001U)
#define SDL_ECC_AGGR_SEC_STATUS_REG0_RAMECC0_PEND_SHIFT                        (0x00000000U)
#define SDL_ECC_AGGR_SEC_STATUS_REG0_RAMECC0_PEND_MAX                          (0x00000001U)

#define SDL_ECC_AGGR_SEC_STATUS_REG0_RAMECC1_PEND_MASK                         (0x00000002U)
#define SDL_ECC_AGGR_SEC_STATUS_REG0_RAMECC1_PEND_SHIFT                        (0x00000001U)
#define SDL_ECC_AGGR_SEC_STATUS_REG0_RAMECC1_PEND_MAX                          (0x00000001U)

#define SDL_ECC_AGGR_SEC_STATUS_REG0_RAMECC2_PEND_MASK                         (0x00000004U)
#define SDL_ECC_AGGR_SEC_STATUS_REG0_RAMECC2_PEND_SHIFT                        (0x00000002U)
#define SDL_ECC_AGGR_SEC_STATUS_REG0_RAMECC2_PEND_MAX                          (0x00000001U)

/* SEC_ENABLE_SET_REG0 */

#define SDL_ECC_AGGR_SEC_ENABLE_SET_REG0_RAMECC0_ENABLE_SET_MASK               (0x00000001U)
#define SDL_ECC_AGGR_SEC_ENABLE_SET_REG0_RAMECC0_ENABLE_SET_SHIFT              (0x00000000U)
#define SDL_ECC_AGGR_SEC_ENABLE_SET_REG0_RAMECC0_ENABLE_SET_MAX                (0x00000001U)

#define SDL_ECC_AGGR_SEC_ENABLE_SET_REG0_RAMECC1_ENABLE_SET_MASK               (0x00000002U)
#define SDL_ECC_AGGR_SEC_ENABLE_SET_REG0_RAMECC1_ENABLE_SET_SHIFT              (0x00000001U)
#define SDL_ECC_AGGR_SEC_ENABLE_SET_REG0_RAMECC1_ENABLE_SET_MAX                (0x00000001U)

#define SDL_ECC_AGGR_SEC_ENABLE_SET_REG0_RAMECC2_ENABLE_SET_MASK               (0x00000004U)
#define SDL_ECC_AGGR_SEC_ENABLE_SET_REG0_RAMECC2_ENABLE_SET_SHIFT              (0x00000002U)
#define SDL_ECC_AGGR_SEC_ENABLE_SET_REG0_RAMECC2_ENABLE_SET_MAX                (0x00000001U)

/* SEC_ENABLE_CLR_REG0 */

#define SDL_ECC_AGGR_SEC_ENABLE_CLR_REG0_RAMECC0_ENABLE_CLR_MASK               (0x00000001U)
#define SDL_ECC_AGGR_SEC_ENABLE_CLR_REG0_RAMECC0_ENABLE_CLR_SHIFT              (0x00000000U)
#define SDL_ECC_AGGR_SEC_ENABLE_CLR_REG0_RAMECC0_ENABLE_CLR_MAX                (0x00000001U)

#define SDL_ECC_AGGR_SEC_ENABLE_CLR_REG0_RAMECC1_ENABLE_CLR_MASK               (0x00000002U)
#define SDL_ECC_AGGR_SEC_ENABLE_CLR_REG0_RAMECC1_ENABLE_CLR_SHIFT              (0x00000001U)
#define SDL_ECC_AGGR_SEC_ENABLE_CLR_REG0_RAMECC1_ENABLE_CLR_MAX                (0x00000001U)

#define SDL_ECC_AGGR_SEC_ENABLE_CLR_REG0_RAMECC2_ENABLE_CLR_MASK               (0x00000004U)
#define SDL_ECC_AGGR_SEC_ENABLE_CLR_REG0_RAMECC2_ENABLE_CLR_SHIFT              (0x00000002U)
#define SDL_ECC_AGGR_SEC_ENABLE_CLR_REG0_RAMECC2_ENABLE_CLR_MAX                (0x00000001U)

/* DED_EOI_REG */

#define SDL_ECC_AGGR_DED_EOI_REG_EOI_WR_MASK                                   (0x00000001U)
#define SDL_ECC_AGGR_DED_EOI_REG_EOI_WR_SHIFT                                  (0x00000000U)
#define SDL_ECC_AGGR_DED_EOI_REG_EOI_WR_MAX                                    (0x00000001U)

/* DED_STATUS_REG0 */

#define SDL_ECC_AGGR_DED_STATUS_REG0_RAMECC0_PEND_MASK                         (0x00000001U)
#define SDL_ECC_AGGR_DED_STATUS_REG0_RAMECC0_PEND_SHIFT                        (0x00000000U)
#define SDL_ECC_AGGR_DED_STATUS_REG0_RAMECC0_PEND_MAX                          (0x00000001U)

#define SDL_ECC_AGGR_DED_STATUS_REG0_RAMECC1_PEND_MASK                         (0x00000002U)
#define SDL_ECC_AGGR_DED_STATUS_REG0_RAMECC1_PEND_SHIFT                        (0x00000001U)
#define SDL_ECC_AGGR_DED_STATUS_REG0_RAMECC1_PEND_MAX                          (0x00000001U)

#define SDL_ECC_AGGR_DED_STATUS_REG0_RAMECC2_PEND_MASK                         (0x00000004U)
#define SDL_ECC_AGGR_DED_STATUS_REG0_RAMECC2_PEND_SHIFT                        (0x00000002U)
#define SDL_ECC_AGGR_DED_STATUS_REG0_RAMECC2_PEND_MAX                          (0x00000001U)

/* DED_ENABLE_SET_REG0 */

#define SDL_ECC_AGGR_DED_ENABLE_SET_REG0_RAMECC0_ENABLE_SET_MASK               (0x00000001U)
#define SDL_ECC_AGGR_DED_ENABLE_SET_REG0_RAMECC0_ENABLE_SET_SHIFT              (0x00000000U)
#define SDL_ECC_AGGR_DED_ENABLE_SET_REG0_RAMECC0_ENABLE_SET_MAX                (0x00000001U)

#define SDL_ECC_AGGR_DED_ENABLE_SET_REG0_RAMECC1_ENABLE_SET_MASK               (0x00000002U)
#define SDL_ECC_AGGR_DED_ENABLE_SET_REG0_RAMECC1_ENABLE_SET_SHIFT              (0x00000001U)
#define SDL_ECC_AGGR_DED_ENABLE_SET_REG0_RAMECC1_ENABLE_SET_MAX                (0x00000001U)

#define SDL_ECC_AGGR_DED_ENABLE_SET_REG0_RAMECC2_ENABLE_SET_MASK               (0x00000004U)
#define SDL_ECC_AGGR_DED_ENABLE_SET_REG0_RAMECC2_ENABLE_SET_SHIFT              (0x00000002U)
#define SDL_ECC_AGGR_DED_ENABLE_SET_REG0_RAMECC2_ENABLE_SET_MAX                (0x00000001U)

/* DED_ENABLE_CLR_REG0 */

#define SDL_ECC_AGGR_DED_ENABLE_CLR_REG0_RAMECC0_ENABLE_CLR_MASK               (0x00000001U)
#define SDL_ECC_AGGR_DED_ENABLE_CLR_REG0_RAMECC0_ENABLE_CLR_SHIFT              (0x00000000U)
#define SDL_ECC_AGGR_DED_ENABLE_CLR_REG0_RAMECC0_ENABLE_CLR_MAX                (0x00000001U)

#define SDL_ECC_AGGR_DED_ENABLE_CLR_REG0_RAMECC1_ENABLE_CLR_MASK               (0x00000002U)
#define SDL_ECC_AGGR_DED_ENABLE_CLR_REG0_RAMECC1_ENABLE_CLR_SHIFT              (0x00000001U)
#define SDL_ECC_AGGR_DED_ENABLE_CLR_REG0_RAMECC1_ENABLE_CLR_MAX                (0x00000001U)

#define SDL_ECC_AGGR_DED_ENABLE_CLR_REG0_RAMECC2_ENABLE_CLR_MASK               (0x00000004U)
#define SDL_ECC_AGGR_DED_ENABLE_CLR_REG0_RAMECC2_ENABLE_CLR_SHIFT              (0x00000002U)
#define SDL_ECC_AGGR_DED_ENABLE_CLR_REG0_RAMECC2_ENABLE_CLR_MAX                (0x00000001U)

/* AGGR_ENABLE_SET */

#define SDL_ECC_AGGR_AGGR_ENABLE_SET_PARITY_MASK                               (0x00000001U)
#define SDL_ECC_AGGR_AGGR_ENABLE_SET_PARITY_SHIFT                              (0x00000000U)
#define SDL_ECC_AGGR_AGGR_ENABLE_SET_PARITY_MAX                                (0x00000001U)

#define SDL_ECC_AGGR_AGGR_ENABLE_SET_TIMEOUT_MASK                              (0x00000002U)
#define SDL_ECC_AGGR_AGGR_ENABLE_SET_TIMEOUT_SHIFT                             (0x00000001U)
#define SDL_ECC_AGGR_AGGR_ENABLE_SET_TIMEOUT_MAX                               (0x00000001U)

/* AGGR_ENABLE_CLR */

#define SDL_ECC_AGGR_AGGR_ENABLE_CLR_PARITY_MASK                               (0x00000001U)
#define SDL_ECC_AGGR_AGGR_ENABLE_CLR_PARITY_SHIFT                              (0x00000000U)
#define SDL_ECC_AGGR_AGGR_ENABLE_CLR_PARITY_MAX                                (0x00000001U)

#define SDL_ECC_AGGR_AGGR_ENABLE_CLR_TIMEOUT_MASK                              (0x00000002U)
#define SDL_ECC_AGGR_AGGR_ENABLE_CLR_TIMEOUT_SHIFT                             (0x00000001U)
#define SDL_ECC_AGGR_AGGR_ENABLE_CLR_TIMEOUT_MAX                               (0x00000001U)

/* AGGR_STATUS_SET */

#define SDL_ECC_AGGR_AGGR_STATUS_SET_PARITY_MASK                               (0x00000003U)
#define SDL_ECC_AGGR_AGGR_STATUS_SET_PARITY_SHIFT                              (0x00000000U)
#define SDL_ECC_AGGR_AGGR_STATUS_SET_PARITY_MAX                                (0x00000003U)

#define SDL_ECC_AGGR_AGGR_STATUS_SET_TIMEOUT_MASK                              (0x0000000CU)
#define SDL_ECC_AGGR_AGGR_STATUS_SET_TIMEOUT_SHIFT                             (0x00000002U)
#define SDL_ECC_AGGR_AGGR_STATUS_SET_TIMEOUT_MAX                               (0x00000003U)

/* AGGR_STATUS_CLR */

#define SDL_ECC_AGGR_AGGR_STATUS_CLR_PARITY_MASK                               (0x00000003U)
#define SDL_ECC_AGGR_AGGR_STATUS_CLR_PARITY_SHIFT                              (0x00000000U)
#define SDL_ECC_AGGR_AGGR_STATUS_CLR_PARITY_MAX                                (0x00000003U)

#define SDL_ECC_AGGR_AGGR_STATUS_CLR_TIMEOUT_MASK                              (0x0000000CU)
#define SDL_ECC_AGGR_AGGR_STATUS_CLR_TIMEOUT_SHIFT                             (0x00000002U)
#define SDL_ECC_AGGR_AGGR_STATUS_CLR_TIMEOUT_MAX                               (0x00000003U)

#ifdef __cplusplus
}
#endif
#endif
