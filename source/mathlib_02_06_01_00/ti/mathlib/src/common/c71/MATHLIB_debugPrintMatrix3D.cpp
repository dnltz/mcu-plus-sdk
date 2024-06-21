/******************************************************************************/
/*!
 * \file MATHLIB_debugPrintMatrix.c
 */
/* Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/


/*******************************************************************************
 *
 * INCLUDES
 *
 ******************************************************************************/

#include "../MATHLIB_types.h"
#include "../MATHLIB_bufParams.h"
#include "../MATHLIB_utility.h"
   
template <typename dataType>
void MATHLIB_debugPrintMatrix3D_helper(dataType *matrix, const MATHLIB_bufParams3D_t *params)
{
   uint32_t x,y,z;
   dataType* xPtr;
   dataType* yPtr;
   dataType* zPtr = matrix;
   
   
   // stride_y is stored in bytes, but easier to use in elements
   uint32_t stride_y_elements = params->stride_y / MATHLIB_sizeof(params->data_type);
   uint32_t stride_z_elements = params->stride_z / MATHLIB_sizeof(params->data_type);
   
   for(z = 0; z < params->dim_z; z++){
      MATHLIB_PRINTF("%s", "\n");
      yPtr = zPtr;
      for(y = 0; y < params->dim_y; y++){
         xPtr = yPtr;
         MATHLIB_PRINTF("%p |", xPtr);
         for(x = 0; x < params->dim_x; x++){
            MATHLIB_PRINTF("%3d ", *(xPtr));
            xPtr++;
         }
         MATHLIB_PRINTF("%s", "|\n");
         yPtr += stride_y_elements;
      }
      zPtr += stride_z_elements;
   }
   
   return;
}

template void MATHLIB_debugPrintMatrix3D_helper<int8_t>  (int8_t   *matrix, const MATHLIB_bufParams3D_t *params);
template void MATHLIB_debugPrintMatrix3D_helper<int16_t> (int16_t  *matrix, const MATHLIB_bufParams3D_t *params);
template void MATHLIB_debugPrintMatrix3D_helper<int32_t> (int32_t  *matrix, const MATHLIB_bufParams3D_t *params);


template <typename dataType>
void MATHLIB_debugPrintMatrix3D_helperU(dataType *matrix, const MATHLIB_bufParams3D_t *params)
{
   uint32_t x,y,z;
   dataType* xPtr;
   dataType* yPtr;
   dataType* zPtr = matrix;
   
   
   // stride_y is stored in bytes, but easier to use in elements
   uint32_t stride_y_elements = params->stride_y / MATHLIB_sizeof(params->data_type);
   uint32_t stride_z_elements = params->stride_z / MATHLIB_sizeof(params->data_type);
   
   for(z = 0; z < params->dim_z; z++){
      MATHLIB_PRINTF("%s", "\n");
      yPtr = zPtr;
      for(y = 0; y < params->dim_y; y++){
         xPtr = yPtr;
         MATHLIB_PRINTF("%p |", xPtr);
         for(x = 0; x < params->dim_x; x++){
            MATHLIB_PRINTF("%3u ", *(xPtr));
            xPtr++;
         }
         MATHLIB_PRINTF("%s", "|\n");
         yPtr += stride_y_elements;
      }
      zPtr += stride_z_elements;
   }
   
   return;
}

template void MATHLIB_debugPrintMatrix3D_helper<uint8_t> (uint8_t  *matrix, const MATHLIB_bufParams3D_t *params);
template void MATHLIB_debugPrintMatrix3D_helper<uint16_t>(uint16_t *matrix, const MATHLIB_bufParams3D_t *params);


/******************************************************************************/

/******************************************************************************/
/*!
 * \ingroup
 * \brief
 * \details
 * \return  void
 */
/******************************************************************************/


// want this function to have C-linkage in library...
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
   
   void MATHLIB_debugPrintMatrix3D(void *matrix, const MATHLIB_bufParams3D_t *params)
   {
      switch(params->data_type){
         case MATHLIB_INT32 :
            MATHLIB_debugPrintMatrix3D_helper<int32_t>((int32_t *)matrix, params);
            break;
         case MATHLIB_INT16 :
            MATHLIB_debugPrintMatrix3D_helper<int16_t>((int16_t *)matrix, params);
            break;
         case MATHLIB_UINT16 :
            MATHLIB_debugPrintMatrix3D_helperU<uint16_t>((uint16_t *)matrix, params);
            break;
         case MATHLIB_INT8 :
            MATHLIB_debugPrintMatrix3D_helper<int8_t>((int8_t *)matrix, params);
            break;
         case MATHLIB_UINT8 :
            MATHLIB_debugPrintMatrix3D_helperU<uint8_t>((uint8_t *)matrix, params);
            break;
         default :
            MATHLIB_PRINTF("\nERROR: Unrecognized data type in %s.\n", __FUNCTION__);
      }
      
      return;
   }
#ifdef __cplusplus
}
#endif /* __cplusplus */

