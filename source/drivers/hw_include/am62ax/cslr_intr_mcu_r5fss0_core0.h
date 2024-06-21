/********************************************************************
*
* MCU_R5FSS0_CORE0 INTERRUPT MAP. header file
*
* Copyright (C) 2015-2020 Texas Instruments Incorporated.
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
#ifndef CSLR_MCU_R5FSS0_CORE0_INTERRUPT_MAP_H_
#define CSLR_MCU_R5FSS0_CORE0_INTERRUPT_MAP_H_

#include <drivers/hw_include/cslr.h>
#include <drivers/hw_include/tistdtypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
* List of intr sources for receiver: MCU_R5FSS0_CORE0
*/

#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_SMS0_AESEIP38T_0_AES_SINTREQUEST_P_0                       (1U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_SMS0_AESEIP38T_0_AES_SINTREQUEST_S_0                       (2U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_R5FSS0_CORE0_CPU0_EXP_INTR_0                           (4U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_SA3_SS0_INTAGGR_0_INTAGGR_VINTR_7                          (7U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_SA3_SS0_SA_UL_0_SA_UL_PKA_0                                (16U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_SA3_SS0_SA_UL_0_SA_UL_TRNG_0                               (17U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_SMS0_TIFS_CBASS_0_FW_EXCEPTION_INTR_0                      (19U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_SMS0_COMMON_0_COMBINED_SEC_IN_0                            (20U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_SMS0_HSM_CBASS_0_FW_EXCEPTION_INTR_0                       (21U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_TIMER0_INTR_PEND_0                                     (28U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_TIMER1_INTR_PEND_0                                     (29U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_RTI0_INTR_WWD_0                                        (30U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GLUELOGIC_MCU_CBASS_INTR_OR_GLUE_OUT_0                     (31U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MAIN_GPIOMUX_INTROUTER0_OUTP_34                            (32U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MAIN_GPIOMUX_INTROUTER0_OUTP_35                            (33U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_EPWM0_EPWM_ETINT_0                                         (36U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_EPWM1_EPWM_ETINT_0                                         (37U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_EPWM2_EPWM_ETINT_0                                         (38U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GLUELOGIC_MCU_ACCESS_ERR_INTR_GLUE_OUT_0                   (39U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DSS0_DISPC_INTR_REQ_0_0                                    (40U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DSS0_DISPC_INTR_REQ_1_0                                    (41U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_MCAN0_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0                 (42U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_MCAN0_MCANSS_MCAN_LVL_INT_0                            (43U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_MCAN0_MCANSS_MCAN_LVL_INT_1                            (44U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_MCAN1_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0                 (45U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_MCAN1_MCANSS_MCAN_LVL_INT_0                            (46U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_MCAN1_MCANSS_MCAN_LVL_INT_1                            (47U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CMP_EVENT_INTROUTER0_OUTP_34                               (58U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CMP_EVENT_INTROUTER0_OUTP_35                               (59U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CMP_EVENT_INTROUTER0_OUTP_36                               (60U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CMP_EVENT_INTROUTER0_OUTP_37                               (61U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_168                    (64U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_169                    (65U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_170                    (66U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_171                    (67U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_172                    (68U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_173                    (69U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_174                    (70U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_175                    (71U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_176                    (72U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_177                    (73U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_178                    (74U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_179                    (75U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_180                    (76U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_181                    (77U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_182                    (78U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_183                    (79U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_EPWM0_EPWM_TRIPZINT_0                                      (80U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_EPWM1_EPWM_TRIPZINT_0                                      (81U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_EPWM2_EPWM_TRIPZINT_0                                      (82U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_ECAP0_ECAP_INT_0                                           (83U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_ECAP1_ECAP_INT_0                                           (84U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_ECAP2_ECAP_INT_0                                           (85U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_EQEP0_EQEP_INT_0                                           (86U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_EQEP1_EQEP_INT_0                                           (87U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_EQEP2_EQEP_INT_0                                           (88U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_R5FSS0_CORE0_COMMRX_LEVEL_0_0                          (90U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_R5FSS0_CORE0_COMMTX_LEVEL_0_0                          (91U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_R5FSS0_CORE0_CPU0_PMU_0                                (94U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_R5FSS0_CORE0_CPU0_VALFIQ_0                             (95U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_R5FSS0_CORE0_CPU0_VALIRQ_0                             (96U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_RTCSS0_RTC_EVENT_PEND_0                               (97U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_JPGENC0_IRQ_0                                              (100U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GLUELOGIC_MAINRESET_REQUEST_GLUE_MAIN_RESETZ_SYNC_STRETCH_0 (101U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GLUELOGIC_MAINRESET_REQUEST_GLUE_MAIN_PORZ_SYNC_STRETCH_0  (102U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GPMC0_GPMC_SINTERRUPT_0                                    (103U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_MCU_GPIOMUX_INTROUTER0_OUTP_4                         (104U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_MCU_GPIOMUX_INTROUTER0_OUTP_5                         (105U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_MCU_GPIOMUX_INTROUTER0_OUTP_6                         (106U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_MCU_GPIOMUX_INTROUTER0_OUTP_7                         (107U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_DCC0_INTR_DONE_LEVEL_0                                 (108U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GLUELOGIC_MAIN_DCC_DONE_GLUE_DCC_DONE_0                    (109U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_PBIST0_DFT_PBIST_CPU_0                                 (110U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_SMS0_RAT_1_EXP_INTR_0                                      (111U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_SMS0_RAT_0_EXP_INTR_0                                      (112U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_COMPUTE_CLUSTER0_PBIST_0_DFT_PBIST_CPU_0                   (113U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_PBIST0_DFT_PBIST_CPU_0                                     (114U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_PBIST0_DFT_PBIST_CPU_0                                (115U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_C7X256V0_CLEC_DFT_PBIST_CPU_0                              (116U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_PBIST3_K3_PBIST_8C28P_4BIT_WRAP__DFT_PBIST_CPU_0           (117U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_PBIST1_DFT_PBIST_CPU_0                                (118U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCRC64_0_INT_MCRC_0                                        (119U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCASP0_REC_INTR_PEND_0                                     (120U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCASP0_XMIT_INTR_PEND_0                                    (121U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCASP1_REC_INTR_PEND_0                                     (122U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCASP1_XMIT_INTR_PEND_0                                    (123U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCASP2_REC_INTR_PEND_0                                     (124U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCASP2_XMIT_INTR_PEND_0                                    (125U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GLUELOGIC_SOC_ACCESS_ERR_INTR_GLUE_OUT_0                   (128U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS1_INTAGGR_0_INTAGGR_VINTR_PEND_0                      (129U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS1_INTAGGR_0_INTAGGR_VINTR_PEND_1                      (130U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS1_INTAGGR_0_INTAGGR_VINTR_PEND_2                      (131U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS1_INTAGGR_0_INTAGGR_VINTR_PEND_3                      (132U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CODEC0_VPU_WAVE521CL_INTR_0                                (133U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CPSW0_EVNT_PEND_0                                          (134U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CPSW0_MDIO_PEND_0                                          (135U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CPSW0_STAT_PEND_0                                          (136U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_DCC1_INTR_DONE_LEVEL_0                                 (137U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_TIMER2_INTR_PEND_0                                     (138U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_TIMER3_INTR_PEND_0                                     (139U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_ESM0_ESM_INT_CFG_LVL_0                                (140U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_ESM0_ESM_INT_HI_LVL_0                                 (141U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_ESM0_ESM_INT_LOW_LVL_0                                (142U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_PSC0_PSC_ALLINT_0                                     (145U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_PSC0_PSC_ALLINT_0                                          (146U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GLUELOGIC_SOC_CBASS_ERR_INTR_GLUE_MAIN_CBASS_AGG_ERR_INTR_0 (147U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_VPAC0_K3_PBIST_8C28P_4BIT_WRAP__DFT_PBIST_CPU_0            (149U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS1_INTAGGR_0_INTAGGR_VINTR_PEND_4                      (150U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DDR32SS0_DDRSS_CONTROLLER_0                                (151U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GLUELOGIC_MGASKET_INTR_GLUE_OUT_0                          (152U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GLUELOGIC_SGASKET_INTR_GLUE_OUT_0                          (153U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS1_INTAGGR_0_INTAGGR_VINTR_PEND_5                      (158U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS1_INTAGGR_0_INTAGGR_VINTR_PEND_6                      (159U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DMASS1_INTAGGR_0_INTAGGR_VINTR_PEND_7                      (160U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MMCSD0_EMMCSDSS_INTR_0                                     (161U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MMCSD1_EMMCSDSS_INTR_0                                     (162U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MMCSD2_EMMCSDSS_INTR_0                                     (163U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_ELM0_ELM_POROCPSINTERRUPT_LVL_0                            (164U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_ESM0_ESM_INT_CFG_LVL_0                                     (167U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_ESM0_ESM_INT_HI_LVL_0                                      (168U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_ESM0_ESM_INT_LOW_LVL_0                                     (169U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_FSS0_OSPI_0_OSPI_LVL_INTR_0                                (171U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CSI_RX_IF0_CSI_FATAL_0                                     (172U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CSI_RX_IF0_CSI_IRQ_0                                       (173U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CSI_RX_IF0_CSI_LEVEL_0                                     (174U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_R5FSS0_CORE0_CPU0_CTI_0                                (175U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_CSI_RX_IF0_CSI_NONFATAL_0                                  (176U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DDPA0_DDPA_INTR_0                                          (177U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_VPAC0_VPAC_LEVEL_0                                         (178U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_VPAC0_VPAC_LEVEL_1                                         (179U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_VPAC0_VPAC_LEVEL_2                                         (180U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DDR32SS0_DDRSS_PLL_FREQ_CHANGE_REQ_0                       (181U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_VPAC0_VPAC_LEVEL_3                                         (182U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_VTM0_THERM_LVL_GT_TH1_INTR_0                          (183U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_VTM0_THERM_LVL_GT_TH2_INTR_0                          (184U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_VTM0_THERM_LVL_LT_TH0_INTR_0                          (185U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCAN0_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0                     (186U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCAN0_MCANSS_MCAN_LVL_INT_0                                (187U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCAN0_MCANSS_MCAN_LVL_INT_1                                (188U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_VPAC0_VPAC_LEVEL_4                                         (189U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_I2C0_POINTRPEND_0                                     (190U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_VPAC0_VPAC_LEVEL_5                                         (191U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_MCRC64_0_INT_MCRC_0                                    (192U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_I2C0_POINTRPEND_0                                          (193U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_I2C1_POINTRPEND_0                                          (194U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_I2C2_POINTRPEND_0                                          (195U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_I2C3_POINTRPEND_0                                          (196U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_I2C0_POINTRPEND_0                                      (197U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DEBUGSS0_AQCMPINTR_LEVEL_0                                 (201U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_DEBUGSS0_CTM_LEVEL_0                                       (202U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_GLUELOGIC_GLUE_EXT_INTN_OUT_0                              (203U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCSPI0_INTR_SPI_0                                          (204U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCSPI1_INTR_SPI_0                                          (205U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCSPI2_INTR_SPI_0                                          (206U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_MCSPI0_INTR_SPI_0                                      (207U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_MCSPI1_INTR_SPI_0                                      (208U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_UART0_USART_IRQ_0                                          (210U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_UART1_USART_IRQ_0                                          (211U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_UART2_USART_IRQ_0                                          (212U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_UART3_USART_IRQ_0                                          (213U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_UART4_USART_IRQ_0                                          (214U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_UART5_USART_IRQ_0                                          (215U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_UART6_USART_IRQ_0                                          (216U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MCU_UART0_USART_IRQ_0                                      (217U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_WKUP_UART0_USART_IRQ_0                                     (219U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB0_IRQ_0                                                 (220U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB0_IRQ_1                                                 (221U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB0_IRQ_2                                                 (222U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB0_IRQ_3                                                 (223U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB0_IRQ_4                                                 (224U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB0_IRQ_5                                                 (225U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB0_IRQ_6                                                 (226U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB0_IRQ_7                                                 (227U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB0_MISC_LEVEL_0                                          (228U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB1_IRQ_0                                                 (230U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB1_IRQ_1                                                 (231U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB1_IRQ_2                                                 (232U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB1_IRQ_3                                                 (233U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB1_IRQ_4                                                 (234U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB1_IRQ_5                                                 (235U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB1_IRQ_6                                                 (236U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB1_IRQ_7                                                 (237U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_USB1_MISC_LEVEL_0                                          (238U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MAILBOX0_MAILBOX_CLUSTER_0_MAILBOX_CLUSTER0_PEND_2         (240U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MAILBOX0_MAILBOX_CLUSTER_0_MAILBOX_CLUSTER1_PEND_2         (241U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MAILBOX0_MAILBOX_CLUSTER_0_MAILBOX_CLUSTER2_PEND_2         (242U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_MAILBOX0_MAILBOX_CLUSTER_0_MAILBOX_CLUSTER3_PEND_2         (243U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_C7X256V0_CLEC_SOC_EVENTS_OUT_LEVEL_8                       (249U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_C7X256V0_CLEC_SOC_EVENTS_OUT_LEVEL_9                       (250U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_C7X256V0_CLEC_SOC_EVENTS_OUT_LEVEL_10                      (254U)
#define CSLR_MCU_R5FSS0_CORE0_CPU0_INTR_C7X256V0_CLEC_SOC_EVENTS_OUT_LEVEL_11                      (255U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_MCU_R5FSS0_CORE0_INTERRUPT_MAP_H_ */

