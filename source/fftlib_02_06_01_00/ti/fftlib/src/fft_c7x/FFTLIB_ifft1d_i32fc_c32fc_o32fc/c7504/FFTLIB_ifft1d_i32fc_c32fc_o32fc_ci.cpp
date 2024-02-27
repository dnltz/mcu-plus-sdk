/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 2017 Texas Instruments Incorporated                |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission to use, copy, modify, or distribute this software,            |**
**| whether in part or in whole, for any purpose is forbidden without        |**
**| a signed licensing agreement and NDA from Texas Instruments              |**
**| Incorporated (TI).                                                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

#include "../../../common/c71/FFTLIB_debug.h"
#include "../FFTLIB_ifft1d_i32fc_c32fc_o32fc.h"

#define TRACE_ON (0)
#define LAST_LOOP_UNROLL 0

#if TRACE_ON
#include "../../../common/printv.h"
#include <stdio.h>
#endif

// CODE_SECTION(FFTLIB_ifft1d,              ".text:optimized")
// CODE_SECTION(FFTLIB_ifft1d_core,         ".text:optimized")
// CODE_SECTION(FFTLIB_ifft1d_checkParams,  ".text:optimized")

#define SE_PARAM_BASE (0x0000)
#define SE_LOOP1_PARAM_OFFSET (SE_PARAM_BASE)
#define SE_LOOP2_PARAM_OFFSET (SE_LOOP1_PARAM_OFFSET + SE_PARAM_SIZE)
#define SE_LOOP3_PARAM_OFFSET (SE_LOOP2_PARAM_OFFSET + SE_PARAM_SIZE)
#define SE_LOOP4_PARAM_OFFSET (SE_LOOP3_PARAM_OFFSET + SE_PARAM_SIZE)
#define SE_LOOP5_PARAM_OFFSET (SE_LOOP4_PARAM_OFFSET + SE_PARAM_SIZE)
#define SE_TWID_PARAM_OFFSET (SE_LOOP5_PARAM_OFFSET + SE_PARAM_SIZE)
#define SA_LOOP1_PARAM_OFFSET (SE_TWID_PARAM_OFFSET + SE_PARAM_SIZE)
#define SA_LOOP2_PARAM_OFFSET (SA_LOOP1_PARAM_OFFSET + SA_PARAM_SIZE)
#define SA_LOOP3_PARAM_OFFSET (SA_LOOP2_PARAM_OFFSET + SA_PARAM_SIZE)
#define SE_CONJ_LOOP_PARAM_OFFSET (SA_LOOP3_PARAM_OFFSET + SE_PARAM_SIZE)
#define SA_CONJ_LOOP_PARAM_OFFSET (SE_CONJ_LOOP_PARAM_OFFSET + SE_PARAM_SIZE)

typedef typename c7x::cfloat_vec CV;
typedef CV *                     CVP;

typedef typename c7x::float_vec V;
typedef V *                     VP;

void ifft_i32fc_o32fc_conjugate_init_ci (void *pX, uint32_t size, void *pBlock)
{
   __SE_TEMPLATE_v1 se0_param = __gen_SE_TEMPLATE_v1 ();
   __SA_TEMPLATE_v1 sa0_param = __gen_SA_TEMPLATE_v1 ();
   /* cfloat *restrict pXLocal   = (cfloat *) pX; */

   se0_param.ELETYPE = __SE_ELETYPE_32BIT_CMPLX_SWAP;
   se0_param.VECLEN  = c7x::se_veclen<c7x::cfloat_vec>::value;
   se0_param.DIMFMT  = __SE_DIMFMT_1D;
   se0_param.ICNT0   = size;

   *((__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_CONJ_LOOP_PARAM_OFFSET)) =
       se0_param;

   sa0_param.ICNT0  = size;
   sa0_param.VECLEN = c7x::sa_veclen<c7x::cfloat_vec>::value;
   sa0_param.DIMFMT = __SA_DIMFMT_1D;

   *((__SA_TEMPLATE_v1 *) ((uint8_t *) pBlock + SA_CONJ_LOOP_PARAM_OFFSET)) =
       sa0_param;
}

FFTLIB_STATUS
FFTLIB_ifft1d_i32fc_c32fc_o32fc_init (FFTLIB_F32 *          pX,
                                      FFTLIB_bufParams1D_t *bufParamsX,
                                      FFTLIB_F32 *          pW,
                                      FFTLIB_bufParams1D_t *bufParamsW,
                                      FFTLIB_F32 *          pY,
                                      FFTLIB_bufParams1D_t *bufParamsY,
                                      void *                pBlock)
{
   FFTLIB_STATUS status = FFTLIB_SUCCESS;

#if defined(FFTLIB_CHECK_PARAMS) ||                                            \
    defined(FFTLIB_IFFT1D_I32FC_C32FC_O32FC_CHECK_PARAMS)
   status = FFTLIB_ifft1d_i32fc_c32fc_o32fc_checkParams (
       pX, bufParamsX, pW, bufParamsW, pY, bufParamsY, pBlock);
   if (status == FFTLIB_SUCCESS)
#endif
   {
      uint32_t         numPoints;
      uint32_t         numPointsPerDft;
      uint32_t         seCnt1, seCnt2, seCnt3, seCnt4;
      __SE_TEMPLATE_v1 se0_param = __gen_SE_TEMPLATE_v1 ();
      __SE_TEMPLATE_v1 se1_param = __gen_SE_TEMPLATE_v1 ();
      __SA_TEMPLATE_v1 sa0_param = __gen_SA_TEMPLATE_v1 ();

      numPoints       = bufParamsX->dim_x >> 1;
      numPointsPerDft = numPoints;
      seCnt1          = numPoints >> 2;
      seCnt2          = numPoints >> 4;
      seCnt3          = 1;
      seCnt4          = numPoints >> 2;

      ifft_i32fc_o32fc_conjugate_init_ci (pX, numPoints, pBlock);

      uint32_t elementSize = c7x::element_count_of<c7x::cfloat_vec>::value;
      /* printf ("elementSize: %d\n", elementSize); */
      /* se0_param = (0); */
      se0_param.ICNT0 = elementSize;
      se0_param.ICNT1 = 4; /* 4 quarters(Offsets: 0, N/4, N/2, 3N/4) */
      se0_param.DIM1  = seCnt1;
      se0_param.ICNT2 = seCnt2; /* Number of 8-point fetches within each  */
      se0_param.DIM2  = elementSize;
      se0_param.ICNT3 = seCnt3; /* Number of DFT's  */
      se0_param.DIM3  = numPointsPerDft;

      se0_param.ELETYPE = __SE_ELETYPE_32BIT_CMPLX_SWAP;
      se0_param.VECLEN  = c7x::se_veclen<c7x::cfloat_vec>::value;
      se0_param.DIMFMT  = __SE_DIMFMT_4D;
      *((__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_LOOP1_PARAM_OFFSET)) =
          se0_param;

      /* se1_param = (0); */
      se1_param.ICNT0 = elementSize;
      se1_param.ICNT1 = 3;
      se1_param.DIM1  = seCnt1; /* Twiddle factors for x1, x2 and x3      */
      se1_param.ICNT2 = seCnt2; /* Number of 8-point fetches within each  */
      se1_param.DIM2  = elementSize;
      se1_param.ICNT3 = seCnt3; /* Number of DFT's                        */
      se1_param.DIM3  = 0;

      se1_param.ELETYPE = __SE_ELETYPE_32BIT_CMPLX_SWAP;
      se1_param.VECLEN  = c7x::se_veclen<c7x::cfloat_vec>::value;
      se1_param.DIMFMT  = __SE_DIMFMT_4D;
      *((__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_TWID_PARAM_OFFSET)) =
          se1_param;

      /* sa0_param = (0); */
      sa0_param.ICNT0 = elementSize;
      sa0_param.ICNT1 = 4;
      sa0_param.DIM1  = seCnt1; /* Save to each of the 4 quarters         */
      sa0_param.ICNT2 = seCnt2; /* Number of 8-point stores within each   */
      sa0_param.DIM2  = elementSize;
      sa0_param.ICNT3 = seCnt3;
      sa0_param.DIM3  = numPointsPerDft; /* Number of DFT's                */

      sa0_param.VECLEN = c7x::sa_veclen<c7x::cfloat_vec>::value;
      sa0_param.DIMFMT = __SA_DIMFMT_4D;
      *((__SA_TEMPLATE_v1 *) ((uint8_t *) pBlock + SA_LOOP1_PARAM_OFFSET)) =
          sa0_param;

      /* se0_param = (0); */
      se0_param.ICNT0 = elementSize; /* Fetch first two quarters */
      se0_param.ICNT1 = 1;
      se0_param.DIM1  = 16; /* Process two 16-point DFTs in one shot  */
      se0_param.ICNT2 = seCnt2;
      se0_param.DIM2  = 16; /* Half the number of DFT's               */

      se0_param.ELETYPE = __SE_ELETYPE_32BIT_CMPLX_SWAP;
      /* se0_param.TRANSPOSE = */
      /*     __SE_TRANSPOSE_128BIT; /\* Using 256BIT transpose required  *\/ */
      /*                            /\* 16-byte alignment on pX          *\/ */
      se0_param.VECLEN = c7x::se_veclen<c7x::cfloat_vec>::value;
      se0_param.DIMFMT = __SE_DIMFMT_3D;
      *((__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_LOOP2_PARAM_OFFSET)) =
          se0_param;

      /* sa0_param = (0); */
      sa0_param.ICNT0 = numPoints;

      sa0_param.VECLEN = c7x::sa_veclen<c7x::cfloat_vec>::value;
      sa0_param.DIMFMT = __SA_DIMFMT_1D;
      *((__SA_TEMPLATE_v1 *) ((uint8_t *) pBlock + SA_LOOP2_PARAM_OFFSET)) =
          sa0_param;

      /* se0_param = (0); */
      se0_param       = __gen_SE_TEMPLATE_v1 ();
      se0_param.ICNT0 = numPoints;

      se0_param.ELETYPE = __SE_ELETYPE_32BIT_CMPLX_SWAP;
      se0_param.VECLEN  = c7x::se_veclen<c7x::cfloat_vec>::value;
      se0_param.DIMFMT  = __SE_DIMFMT_1D;
      *((__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_LOOP3_PARAM_OFFSET)) =
          se0_param;

      /* sa0_param = (0); */
      sa0_param.ICNT0 = numPoints;

      sa0_param.VECLEN = c7x::sa_veclen<c7x::cfloat_vec>::value;
      sa0_param.DIMFMT = __SA_DIMFMT_1D;
      *((__SA_TEMPLATE_v1 *) ((uint8_t *) pBlock + SA_LOOP3_PARAM_OFFSET)) =
          sa0_param;

      /* se0_param = (0); */
      se0_param.ICNT0 = seCnt4; /* Fetch consecutive four points for DFT  */
      se0_param.ICNT1 = elementSize;
      se0_param.DIM1  = seCnt4;
      /* Fetch 8 points separated by            */ /* (numPoints >>
                                                             3). This fetch
                                                             pattern   */
      /* can be used for bit reversal           */

      se0_param.ELETYPE   = __SE_ELETYPE_32BIT_CMPLX_SWAP;
      se0_param.TRANSPOSE = __SE_TRANSPOSE_64BIT;
      se0_param.VECLEN    = c7x::se_veclen<c7x::cfloat_vec>::value;
      se0_param.DIMFMT    = __SE_DIMFMT_2D;
      *((__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_LOOP4_PARAM_OFFSET)) =
          se0_param;

      /* se0_param = (0); */
      se0_param.ICNT0 = seCnt4;
      se0_param.ICNT1 = elementSize;
      se0_param.DIM1  = seCnt4;
      /* Fetch 8 points separated by            */ /* (numPoints >>
                                                             3). This fetch
                                                             pattern   */
      /* can be used for bit reversal           */

      se0_param.ELETYPE   = __SE_ELETYPE_32BIT_CMPLX_SWAP;
      se0_param.TRANSPOSE = __SE_TRANSPOSE_64BIT;
      se0_param.VECLEN    = c7x::se_veclen<c7x::cfloat_vec>::value;
      se0_param.DIMFMT    = __SE_DIMFMT_2D;
      *((__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_LOOP5_PARAM_OFFSET)) =
          se0_param;
   }
   return (status);
}

static inline c7x::cfloat_vec
ifft_i32fc_o32fc_scaleAndConjugate (c7x::cfloat_vec in,
                                    c7x::float_vec  scaleVec,
                                    c7x::ulong_vec  xorVec)
{
   return (c7x::as_cfloat_vec (
       scaleVec * c7x::as_float_vec (c7x::as_ulong_vec (in) ^ xorVec)));
}

void ifft_i32fc_o32fc_conjugate_exec_ci (void *         pX,
                                         c7x::ulong_vec xorVec,
                                         uint32_t       size,
                                         void *         pBlock)
{

   __SE_TEMPLATE_v1 se0_param = __gen_SE_TEMPLATE_v1 ();
   __SA_TEMPLATE_v1 sa0_param = __gen_SA_TEMPLATE_v1 ();
   cfloat *restrict pXLocal   = (cfloat *) pX;

   se0_param =
       *((__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_CONJ_LOOP_PARAM_OFFSET));

   sa0_param =
       *((__SA_TEMPLATE_v1 *) ((uint8_t *) pBlock + SA_CONJ_LOOP_PARAM_OFFSET));

   __SE0_OPEN (pX, se0_param);
   __SA0_OPEN (sa0_param);

   uint32_t i         = 0;
   uint32_t loopCount = (size) / c7x::element_count_of<c7x::cfloat_vec>::value;
   c7x::cfloat_vec regIn, regStore;
   float           scale    = 1.0f;
   c7x::float_vec  scaleVec = __vload_dup (&scale);

   __vpred tmp;
   CV *    addr;

   for (i = 0; i < loopCount; i++) {
      regIn = c7x::strm_eng<0, c7x::cfloat_vec>::get_adv ();
      /* FFTLIB_debugPrintFloatVector (c7x::as_float_vec (regIn)); */

      regStore = ifft_i32fc_o32fc_scaleAndConjugate (regIn, scaleVec, xorVec);
      /* FFTLIB_debugPrintFloatVector (c7x::as_float_vec (regStore)); */

      tmp  = c7x::strm_agen<0, CV>::get_vpred ();
      addr = c7x::strm_agen<0, CV>::get_adv (&pXLocal[0]);
      __vstore_pred (tmp, addr, regStore);
   }

   __SE0_CLOSE ();
   __SA0_CLOSE ();
}

FFTLIB_STATUS
FFTLIB_ifft1d_i32fc_c32fc_o32fc_kernel (FFTLIB_F32 *          pX,
                                        FFTLIB_bufParams1D_t *bufParamsX,
                                        FFTLIB_F32 *          pW,
                                        FFTLIB_bufParams1D_t *bufParamsW,
                                        FFTLIB_F32 *          pY,
                                        FFTLIB_bufParams1D_t *bufParamsY,
                                        void *                pBlock)
{
   uint32_t         k;
   FFTLIB_STATUS    status = FFTLIB_SUCCESS;
   uint32_t         numPoints;
   uint32_t         numPointsPerDft;
   uint32_t         numLeadingZeros;
   uint32_t         offsetBitReverse;
   uint32_t         seCnt1, seCnt2, seCnt3;
   __SE_TEMPLATE_v1 se0_param = __gen_SE_TEMPLATE_v1 ();
   __SE_TEMPLATE_v1 se1_param = __gen_SE_TEMPLATE_v1 ();
   __SA_TEMPLATE_v1 sa0_param = __gen_SA_TEMPLATE_v1 ();

   cfloat *restrict pXLocal;
   /* cfloat *restrict pYLocal; */
   cfloat *restrict pWLocal;
   cfloat *restrict pY0;
   cfloat *restrict pY1;
   cfloat *restrict pY2;
   cfloat *restrict pY3;
   cfloat *restrict pY4;
   cfloat *restrict pY5;
   cfloat *restrict pY6;
   cfloat *restrict pY7;

   CV     vX_0, vX_N_4, vX_N_2, vX_3N_4;
   CV     vSum1, vSum2, vDiff1, vDiff2;
   CV     vTwX1, vTwX2, vTwX3;
   CV     vX0Temp, vX1Temp, vX2Temp, vX3Temp;
   CV     vX0, vX1, vX2, vX3;
   CV     vX_0_1, vX_N_4_1, vX_N_2_1, vX_3N_4_1;
   CV     vSum1_1, vSum2_1, vDiff1_1, vDiff2_1;
   CV     vX0_1, vX1_1, vX2_1, vX3_1;
   CV     vX0_2PtDft_1, vX0_2PtDft_2;
   CV     vX1_2PtDft_1, vX1_2PtDft_2;
   CV     vX2_2PtDft_1, vX2_2PtDft_2;
   CV     vX3_2PtDft_1, vX3_2PtDft_2;
   cfloat twTemp;

#ifdef FFTLIB_CHECK_PARAMS
   status = FFTLIB_ifft1d_i32fc_c32fc_o32fc_checkParams (
       pX, bufParamsX, pW, bufParamsW, pY, bufParamsY, pBlock);
   if (status == FFTLIB_SUCCESS)
#endif
   {
      numPoints       = bufParamsX->dim_x >> 1;
      numPointsPerDft = numPoints;

      float          scale    = 1.0 / (numPoints);
      c7x::float_vec scaleVec = __vload_dup (&scale);

#if defined(_HOST_BUILD)
      c7x::ulong_vec xorVec = (c7x::ulong_vec) (0x0000000080000000);

#else
      c7x::ulong_vec xorVec = (0x0000000080000000);
#endif

      ifft_i32fc_o32fc_conjugate_exec_ci ((void *) pX, xorVec, numPoints,
                                          pBlock);

      se0_param =
          *((__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_LOOP1_PARAM_OFFSET));
      se1_param =
          *((__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_TWID_PARAM_OFFSET));
      sa0_param =
          *((__SA_TEMPLATE_v1 *) ((uint8_t *) pBlock + SA_LOOP1_PARAM_OFFSET));
      seCnt1 = numPointsPerDft >> 2;
      seCnt2 = numPointsPerDft >> 4;
      seCnt3 = 1;

      pXLocal = (cfloat *) pX;
      pWLocal = (cfloat *) pW;
      /* pYLocal = (cfloat *) pY; */

      while (numPointsPerDft >= 16) {
         /* TODO OPT: Calculate params upfront in init function,
          *           rather than generating SE params on the fly here */
         se0_param.ICNT1 = 4;
         se0_param.DIM1  = seCnt1; /* 4 quarters(Offsets: 0, N/4, N/2, 3N/4) */
         se0_param.ICNT2 = seCnt2;
         se0_param.DIM2  = 4;
         /* Number of 8-point fetches within       */ /* each quarter */
         se0_param.ICNT3 = seCnt3;
         se0_param.DIM3  = numPointsPerDft; /* Number of DFT's               */
         __SE0_OPEN ((void *) pXLocal, se0_param);

         se1_param.ICNT1 = 3;
         se1_param.DIM1  = seCnt1; /* Twiddle factors for x1, x2 and x3      */
         se1_param.ICNT2 = seCnt2; /* Number of 8-point fetches within each  */
         se1_param.DIM2  = 4;      /* quarter                                */
         se1_param.ICNT3 = seCnt3; /* Number of DFT's                        */
         se1_param.DIM3  = 0;
         __SE1_OPEN ((void *) pWLocal, se1_param);

         sa0_param.ICNT1 = 4;
         sa0_param.DIM1  = /* Save to each of the 4 quarters         */ seCnt1;
         sa0_param.ICNT2 = seCnt2;
         sa0_param.DIM2  = 4;
         /* Number of 8-point stores within        */ /* each quarter */
         sa0_param.ICNT3 = seCnt3;
         sa0_param.DIM3  = numPointsPerDft;
         /* Number of DFT's                        */
         __SA0_OPEN (sa0_param);

         /* Loop is unrolled twice for better optimization */
         for (k = 0; k < numPoints; k += 32) {

            /* First iteration of loop unroll */
            vX_0    = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_4  = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_2  = c7x::strm_eng<0, CV>::get_adv ();
            vX_3N_4 = c7x::strm_eng<0, CV>::get_adv ();

            vSum1  = vX_0 + vX_N_2;
            vSum2  = vX_N_4 + vX_3N_4;
            vDiff1 = vX_0 - vX_N_2;
            vDiff2 = vX_N_4 - vX_3N_4;

            vTwX1 = c7x::strm_eng<1, CV>::get_adv ();
            vTwX2 = c7x::strm_eng<1, CV>::get_adv ();
            vTwX3 = c7x::strm_eng<1, CV>::get_adv ();

            vX0Temp = vSum1 + vSum2;
            vX1Temp = vDiff1 - __vcrot90sp_vv (vDiff2);
            vX2Temp = vSum1 - vSum2;
            vX3Temp = vDiff1 + __vcrot90sp_vv (vDiff2);

            vX0 = vX0Temp;
            vX1 = __complex_multiply (vX1Temp, vTwX1);
            vX2 = __complex_multiply (vX2Temp, vTwX2);
            vX3 = __complex_multiply (vX3Temp, vTwX3);

            /* __SA0ADV(CV, pXLocal) = vX0; */
            /* __SA0ADV(CV, pXLocal) = vX2; */
            /* __SA0ADV(CV, pXLocal) = vX1; */
            /* __SA0ADV(CV, pXLocal) = vX3; */

            __vpred tmp;
            CVP     addr;
            tmp  = c7x::strm_agen<0, CV>::get_vpred ();
            addr = c7x::strm_agen<0, CV>::get_adv (pXLocal);
            __vstore_pred (tmp, addr, vX0);

            tmp  = c7x::strm_agen<0, CV>::get_vpred ();
            addr = c7x::strm_agen<0, CV>::get_adv (pXLocal);
            __vstore_pred (tmp, addr, vX2);

            tmp  = c7x::strm_agen<0, CV>::get_vpred ();
            addr = c7x::strm_agen<0, CV>::get_adv (pXLocal);
            __vstore_pred (tmp, addr, vX1);

            tmp  = c7x::strm_agen<0, CV>::get_vpred ();
            addr = c7x::strm_agen<0, CV>::get_adv (pXLocal);
            __vstore_pred (tmp, addr, vX3);

            /* Second iteration of loop unroll */

            vX_0    = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_4  = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_2  = c7x::strm_eng<0, CV>::get_adv ();
            vX_3N_4 = c7x::strm_eng<0, CV>::get_adv ();

            vSum1  = vX_0 + vX_N_2;
            vSum2  = vX_N_4 + vX_3N_4;
            vDiff1 = vX_0 - vX_N_2;
            vDiff2 = vX_N_4 - vX_3N_4;

            vTwX1 = c7x::strm_eng<1, CV>::get_adv ();
            vTwX2 = c7x::strm_eng<1, CV>::get_adv ();
            vTwX3 = c7x::strm_eng<1, CV>::get_adv ();

            vX0Temp = vSum1 + vSum2;
            vX1Temp = vDiff1 - __vcrot90sp_vv (vDiff2);
            vX2Temp = vSum1 - vSum2;
            vX3Temp = vDiff1 + __vcrot90sp_vv (vDiff2);

            vX0 = vX0Temp;
            vX1 = __complex_multiply (vX1Temp, vTwX1);
            vX2 = __complex_multiply (vX2Temp, vTwX2);
            vX3 = __complex_multiply (vX3Temp, vTwX3);

            /* __SA0ADV(CV, pXLocal) = vX0; */
            /* __SA0ADV(CV, pXLocal) = vX2; */
            /* __SA0ADV(CV, pXLocal) = vX1; */
            /* __SA0ADV(CV, pXLocal) = vX3; */

            tmp  = c7x::strm_agen<0, CV>::get_vpred ();
            addr = c7x::strm_agen<0, CV>::get_adv (pXLocal);
            __vstore_pred (tmp, addr, vX0);

            tmp  = c7x::strm_agen<0, CV>::get_vpred ();
            addr = c7x::strm_agen<0, CV>::get_adv (pXLocal);
            __vstore_pred (tmp, addr, vX2);

            tmp  = c7x::strm_agen<0, CV>::get_vpred ();
            addr = c7x::strm_agen<0, CV>::get_adv (pXLocal);
            __vstore_pred (tmp, addr, vX1);

            tmp  = c7x::strm_agen<0, CV>::get_vpred ();
            addr = c7x::strm_agen<0, CV>::get_adv (pXLocal);
            __vstore_pred (tmp, addr, vX3);
         }
         __SA0_CLOSE ();
         __SE0_CLOSE ();
         __SE1_CLOSE ();

         numPointsPerDft >>= 2;
         pWLocal += numPointsPerDft * 3;
         seCnt1 >>= 2;
         seCnt2 >>= 2;
         seCnt3 <<= 2;
         /* printf ("\n\n"); */
         /* for (int32_t u = 0; u < 64 * 2; u++) { */
         /*    printf ("%f, ", ((float *) pX)[u]); */
         /* } */
      }
      if (numPointsPerDft == 4) {
         /* 4-point stage with bit-reversal */
         se0_param = *(
             (__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_LOOP4_PARAM_OFFSET));
         __SE0_OPEN ((void *) pXLocal, se0_param);

         numLeadingZeros = __norm ((int32_t) (numPoints - 1)) + 1;

         /* pY0 = &pYLocal[0x00000000u]; */
         /* pY1 = &pYLocal[0x40000000u >> numLeadingZeros]; */
         /* pY2 = &pYLocal[0x80000000u >> numLeadingZeros]; */
         /* pY3 = &pYLocal[0xC0000000u >> numLeadingZeros]; */

         pY0 = (cfloat *) (pY + 0);
         pY1 = (cfloat *) (pY + ((0x40000000u >> numLeadingZeros) << 1));
         pY2 = (cfloat *) (pY + ((0x80000000u >> numLeadingZeros) << 1));
         pY3 = (cfloat *) (pY + ((0xC0000000u >> numLeadingZeros) << 1));

#ifdef LAST_LOOP_UNROLL
         /* pY4 = &pYLocal[0x20000000u >> numLeadingZeros]; */
         /* pY5 = &pYLocal[0x60000000u >> numLeadingZeros]; */
         /* pY6 = &pYLocal[0xA0000000u >> numLeadingZeros]; */
         /* pY7 = &pYLocal[0xE0000000u >> numLeadingZeros]; */

         pY4 = (cfloat *) (pY + ((0x20000000u >> numLeadingZeros) << 1));
         pY5 = (cfloat *) (pY + ((0x60000000u >> numLeadingZeros) << 1));
         pY6 = (cfloat *) (pY + ((0xA0000000u >> numLeadingZeros) << 1));
         pY7 = (cfloat *) (pY + ((0xE0000000u >> numLeadingZeros) << 1));
#endif

#if 1
         for (k = 0; k<numPoints>> 2; k += 8)
#else
         for (k = 0; k<numPoints>> 2; k += 4)
#endif
         {
            offsetBitReverse = __bit_reverse (k) >> numLeadingZeros;

            vX_0    = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_4  = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_2  = c7x::strm_eng<0, CV>::get_adv ();
            vX_3N_4 = c7x::strm_eng<0, CV>::get_adv ();

            vSum1  = vX_0 + vX_N_2;
            vSum2  = vX_N_4 + vX_3N_4;
            vDiff1 = vX_0 - vX_N_2;
            vDiff2 = vX_N_4 - vX_3N_4;

            vX0 = vSum1 + vSum2;
            vX1 = vDiff1 - __vcrot90sp_vv (vDiff2);
            vX2 = vSum1 - vSum2;
            vX3 = vDiff1 + __vcrot90sp_vv (vDiff2);

            vX0 = ifft_i32fc_o32fc_scaleAndConjugate (vX0, scaleVec, xorVec);
            vX1 = ifft_i32fc_o32fc_scaleAndConjugate (vX1, scaleVec, xorVec);
            vX2 = ifft_i32fc_o32fc_scaleAndConjugate (vX2, scaleVec, xorVec);
            vX3 = ifft_i32fc_o32fc_scaleAndConjugate (vX3, scaleVec, xorVec);

            __vstore_reverse_bit ((CVP) (pY0 + offsetBitReverse), vX0);
            __vstore_reverse_bit ((CVP) (pY1 + offsetBitReverse), vX1);
            __vstore_reverse_bit ((CVP) (pY2 + offsetBitReverse), vX2);
            __vstore_reverse_bit ((CVP) (pY3 + offsetBitReverse), vX3);

#if 1
            vX_0_1    = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_4_1  = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_2_1  = c7x::strm_eng<0, CV>::get_adv ();
            vX_3N_4_1 = c7x::strm_eng<0, CV>::get_adv ();

            vSum1_1  = vX_0_1 + vX_N_2_1;
            vSum2_1  = vX_N_4_1 + vX_3N_4_1;
            vDiff1_1 = vX_0_1 - vX_N_2_1;
            vDiff2_1 = vX_N_4_1 - vX_3N_4_1;

            vX0_1 = vSum1_1 + vSum2_1;
            vX1_1 = vDiff1_1 - __vcrot90sp_vv (vDiff2_1);
            vX2_1 = vSum1_1 - vSum2_1;
            vX3_1 = vDiff1_1 + __vcrot90sp_vv (vDiff2_1);

            vX0_1 =
                ifft_i32fc_o32fc_scaleAndConjugate (vX0_1, scaleVec, xorVec);
            vX1_1 =
                ifft_i32fc_o32fc_scaleAndConjugate (vX1_1, scaleVec, xorVec);
            vX2_1 =
                ifft_i32fc_o32fc_scaleAndConjugate (vX2_1, scaleVec, xorVec);
            vX3_1 =
                ifft_i32fc_o32fc_scaleAndConjugate (vX3_1, scaleVec, xorVec);

            __vstore_reverse_bit ((CVP) (pY4 + offsetBitReverse), vX0_1);
            __vstore_reverse_bit ((CVP) (pY5 + offsetBitReverse), vX1_1);
            __vstore_reverse_bit ((CVP) (pY6 + offsetBitReverse), vX2_1);
            __vstore_reverse_bit ((CVP) (pY7 + offsetBitReverse), vX3_1);
#endif
         }
         __SE0_CLOSE ();

         /* printf ("\nStage == 4\n"); */
         /* for (int32_t u = 0; u < 64 * 2; u++) { */
         /*    printf ("%f, ", ((float *) pY)[u]); */
         /* } */
      }
      else {
#if 1
         /* 4-point stage followed by 2-point stage with bit-reversal */
         se0_param = *(
             (__SE_TEMPLATE_v1 *) ((uint8_t *) pBlock + SE_LOOP5_PARAM_OFFSET));
         __SE0_OPEN ((void *) pXLocal, se0_param);

         numLeadingZeros = __norm ((int32_t) (numPoints - 1)) + 1;
#if __C7X_HOSTEM__
         pWLocal += 1;
         twTemp = *pWLocal;
         vTwX1  = CV (twTemp, twTemp, twTemp, twTemp);
#else
         pWLocal += 1;
         twTemp = *pWLocal;
         vTwX1  = (CV) (twTemp, twTemp, twTemp, twTemp);
#endif

#if __C7X_HOSTEM__
         pWLocal += 2;
         twTemp = *pWLocal;
         vTwX2  = CV (twTemp, twTemp, twTemp, twTemp);
#else
         pWLocal += 2;
         twTemp = *pWLocal;
         vTwX2  = (CV) (twTemp, twTemp, twTemp, twTemp);
#endif
#if __C7X_HOSTEM__
         pWLocal += 2;
         twTemp = *pWLocal;
         vTwX3  = CV (twTemp, twTemp, twTemp, twTemp);
#else
         pWLocal += 2;
         twTemp = *pWLocal;
         vTwX3  = (CV) (twTemp, twTemp, twTemp, twTemp);
#endif

         /* pY0 = &pYLocal[0x00000000u]; */
         /* pY1 = &pYLocal[0x80000000u >> numLeadingZeros]; */
         /* pY2 = &pYLocal[0x20000000u >> numLeadingZeros]; */
         /* pY3 = &pYLocal[0xA0000000u >> numLeadingZeros]; */
         /* pY4 = &pYLocal[0x40000000u >> numLeadingZeros]; */
         /* pY5 = &pYLocal[0xC0000000u >> numLeadingZeros]; */
         /* pY6 = &pYLocal[0x60000000u >> numLeadingZeros]; */
         /* pY7 = &pYLocal[0xE0000000u >> numLeadingZeros]; */

         pY0 = (cfloat *) (pY + (0x00000000u));
         pY1 = (cfloat *) (pY + ((0x80000000u >> numLeadingZeros) << 1));
         pY2 = (cfloat *) (pY + ((0x20000000u >> numLeadingZeros) << 1));
         pY3 = (cfloat *) (pY + ((0xA0000000u >> numLeadingZeros) << 1));
         pY4 = (cfloat *) (pY + ((0x40000000u >> numLeadingZeros) << 1));
         pY5 = (cfloat *) (pY + ((0xC0000000u >> numLeadingZeros) << 1));
         pY6 = (cfloat *) (pY + ((0x60000000u >> numLeadingZeros) << 1));
         pY7 = (cfloat *) (pY + ((0xE0000000u >> numLeadingZeros) << 1));

         for (k = 0; k<numPoints>> 2; k += 8) {
            offsetBitReverse = __bit_reverse (k) >> numLeadingZeros;

            vX_0      = c7x::strm_eng<0, CV>::get_adv ();
            vX_0_1    = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_4    = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_4_1  = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_2    = c7x::strm_eng<0, CV>::get_adv ();
            vX_N_2_1  = c7x::strm_eng<0, CV>::get_adv ();
            vX_3N_4   = c7x::strm_eng<0, CV>::get_adv ();
            vX_3N_4_1 = c7x::strm_eng<0, CV>::get_adv ();

            vSum1  = vX_0 + vX_N_2;
            vSum2  = vX_N_4 + vX_3N_4;
            vDiff1 = vX_0 - vX_N_2;
            vDiff2 = vX_N_4 - vX_3N_4;

            vX0 = vSum1 + vSum2;
            vX1 = vDiff1 - __vcrot90sp_vv (vDiff2);
            vX2 = vSum1 - vSum2;
            vX3 = vDiff1 + __vcrot90sp_vv (vDiff2);

            vSum1_1  = vX_0_1 + vX_N_2_1;
            vSum2_1  = vX_N_4_1 + vX_3N_4_1;
            vDiff1_1 = vX_0_1 - vX_N_2_1;
            vDiff2_1 = vX_N_4_1 - vX_3N_4_1;

            vX0Temp = vSum1_1 + vSum2_1;
            vX1Temp = vDiff1_1 - __vcrot90sp_vv (vDiff2_1);
            vX2Temp = vSum1_1 - vSum2_1;
            vX3Temp = vDiff1_1 + __vcrot90sp_vv (vDiff2_1);

            vX0_1 = vX0Temp;
            vX1_1 = __complex_multiply (vX1Temp, vTwX1);
            vX2_1 = __complex_multiply (vX2Temp, vTwX2);
            vX3_1 = __complex_multiply (vX3Temp, vTwX3);

            vX0_2PtDft_1 = vX0 + vX0_1;
            vX0_2PtDft_2 = vX0 - vX0_1;
            vX1_2PtDft_1 = vX1 + vX1_1;
            vX1_2PtDft_2 = vX1 - vX1_1;
            vX2_2PtDft_1 = vX2 + vX2_1;
            vX2_2PtDft_2 = vX2 - vX2_1;
            vX3_2PtDft_1 = vX3 + vX3_1;
            vX3_2PtDft_2 = vX3 - vX3_1;

            vX0_2PtDft_1 = ifft_i32fc_o32fc_scaleAndConjugate (
                vX0_2PtDft_1, scaleVec, xorVec);
            vX0_2PtDft_2 = ifft_i32fc_o32fc_scaleAndConjugate (
                vX0_2PtDft_2, scaleVec, xorVec);
            vX1_2PtDft_1 = ifft_i32fc_o32fc_scaleAndConjugate (
                vX1_2PtDft_1, scaleVec, xorVec);
            vX1_2PtDft_2 = ifft_i32fc_o32fc_scaleAndConjugate (
                vX1_2PtDft_2, scaleVec, xorVec);
            vX2_2PtDft_1 = ifft_i32fc_o32fc_scaleAndConjugate (
                vX2_2PtDft_1, scaleVec, xorVec);
            vX2_2PtDft_2 = ifft_i32fc_o32fc_scaleAndConjugate (
                vX2_2PtDft_2, scaleVec, xorVec);
            vX3_2PtDft_1 = ifft_i32fc_o32fc_scaleAndConjugate (
                vX3_2PtDft_1, scaleVec, xorVec);
            vX3_2PtDft_2 = ifft_i32fc_o32fc_scaleAndConjugate (
                vX3_2PtDft_2, scaleVec, xorVec);

            __vstore_reverse_bit ((CVP) (pY0 + offsetBitReverse), vX0_2PtDft_1);
            __vstore_reverse_bit ((CVP) (pY1 + offsetBitReverse), vX0_2PtDft_2);
            __vstore_reverse_bit ((CVP) (pY2 + offsetBitReverse), vX1_2PtDft_1);
            __vstore_reverse_bit ((CVP) (pY3 + offsetBitReverse), vX1_2PtDft_2);
            __vstore_reverse_bit ((CVP) (pY4 + offsetBitReverse), vX2_2PtDft_1);
            __vstore_reverse_bit ((CVP) (pY5 + offsetBitReverse), vX2_2PtDft_2);
            __vstore_reverse_bit ((CVP) (pY6 + offsetBitReverse), vX3_2PtDft_1);
            __vstore_reverse_bit ((CVP) (pY7 + offsetBitReverse), vX3_2PtDft_2);
         }
         __SE0_CLOSE ();
#endif
      }
   }
   return (status);
}

#if (!defined(FFTLIB_REMOVE_CHECK_PARAMS) &&                                   \
     !defined(FFTLIB_IFFT1D_I32FC_C32FC_O32FC_REMOVE_CHECK_PARAMS)) ||         \
    (defined(FFTLIB_CHECK_PARAMS)) ||                                          \
    (defined(FFTLIB_IFFT1D_I32FC_C32FC_O32FC_CHECK_PARAMS))

FFTLIB_STATUS
FFTLIB_ifft1d_i32fc_c32fc_o32fc_checkParams (FFTLIB_F32 *          pX,
                                             FFTLIB_bufParams1D_t *bufParamsX,
                                             FFTLIB_F32 *          pW,
                                             FFTLIB_bufParams1D_t *bufParamsW,
                                             FFTLIB_F32 *          pY,
                                             FFTLIB_bufParams1D_t *bufParamsY,
                                             void *                pBlock)
{
   FFTLIB_STATUS status = FFTLIB_SUCCESS;

   if ((pX == NULL) || (pW == NULL) || (pY == NULL)) {
      status = FFTLIB_ERR_NULL_POINTER;
   }
   else if (bufParamsX->dim_x != bufParamsW->dim_x ||
            bufParamsX->dim_x != bufParamsY->dim_x) {
      status = FFTLIB_ERR_INVALID_DIMENSION;
   }
   else if (bufParamsX->dim_x < 64 * 2) { /* Minimum number of points is 64 */
      status = FFTLIB_ERR_INVALID_DIMENSION;
   }
   else if ((bufParamsX->data_type != FFTLIB_FLOAT32) ||
            (bufParamsW->data_type != FFTLIB_FLOAT32) ||
            (bufParamsY->data_type != FFTLIB_FLOAT32)) {
      status = FFTLIB_ERR_INVALID_TYPE;
   }
   else if (((uint64_t) pX) & 0xFu) { /* pX must be 16-byte aligned for a */
      status = FFTLIB_ERR_NOT_ALIGNED_PTRS_STRIDES; /* streaming engine
                                                       configuration         */
   }
   else {
      /* Check if number of pts is a power of 2 */
      uint32_t k = 0;
      while (k < 32) {
         if (bufParamsX->dim_x & (1u << k)) {
            break;
         }
         k++;
      }
      if ((1u << k) != bufParamsX->dim_x) {
         status = FFTLIB_ERR_INVALID_DIMENSION;
      }
   }
   return (status);
}

#endif
