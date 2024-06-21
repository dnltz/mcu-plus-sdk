
/******************************************************************************/
/*!
 * \file 
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

#ifndef COMMON_MATHLIB_UTIL_DEBUGPRINTVECTOR_H_
#define COMMON_MATHLIB_UTIL_DEBUGPRINTFECTOR_H_ 1

/*******************************************************************************
 *
 * INCLUDES
 *
 ******************************************************************************/

#include "../MATHLIB_types.h"


//----------------------------------------------------------------------------------
#ifdef __cplusplus
#include <c7x_scalable.h> // for device scalability

#pragma FUNC_ALWAYS_INLINE
static inline void MATHLIB_debugPrintVector(c7x::uchar_vec vector){
#if MATHLIB_DEBUGPRINT
   for (uint i = 0; i < c7x::element_count_of<c7x::uchar_vec>::value; i++){
      MATHLIB_PRINTF("%04u", *((uchar*)(&vector) + i));
   }

   MATHLIB_PRINTF("%s", "\n");
#endif
}

#pragma FUNC_ALWAYS_INLINE
static inline void MATHLIB_debugPrintVector(c7x::char_vec vector){
#if MATHLIB_DEBUGPRINT
   for (uint i = 0; i < c7x::element_count_of<c7x::char_vec>::value; i++){
      MATHLIB_PRINTF("%04d", *((int8_t*)(&vector) + i));
   }

   MATHLIB_PRINTF("%s", "\n");
#endif
}

#pragma FUNC_ALWAYS_INLINE
static inline void MATHLIB_debugPrintVector(c7x::ushort_vec vector){
#if MATHLIB_DEBUGPRINT
   for (uint i = 0; i < c7x::element_count_of<c7x::ushort_vec>::value; i++){
      MATHLIB_PRINTF("%05u", *((ushort*)(&vector) + i));
   }

   MATHLIB_PRINTF("%s", "\n");
#endif
}

#pragma FUNC_ALWAYS_INLINE
static inline void MATHLIB_debugPrintVector(c7x::short_vec vector){
#if MATHLIB_DEBUGPRINT
   for (uint i = 0; i < c7x::element_count_of<c7x::short_vec>::value; i++){
      MATHLIB_PRINTF("%05d", *((short*)(&vector) + i));
   }

   MATHLIB_PRINTF("%s", "\n");
#endif
}

#pragma FUNC_ALWAYS_INLINE
static inline void MATHLIB_debugPrintVector(c7x::uint_vec vector){
#if MATHLIB_DEBUGPRINT
   for (uint i = 0; i < c7x::element_count_of<c7x::uint_vec>::value; i++){
      MATHLIB_PRINTF("%010u", *((uint*)(&vector) + i));
   }

   MATHLIB_PRINTF("%s", "\n");
#endif
}

#pragma FUNC_ALWAYS_INLINE
static inline void MATHLIB_debugPrintVector(c7x::int_vec vector){
#if MATHLIB_DEBUGPRINT
   for (uint i = 0; i < c7x::element_count_of<c7x::int_vec>::value; i++){
      MATHLIB_PRINTF("%010d", *((int*)(&vector) + i));
   }

   MATHLIB_PRINTF("%s", "\n");
#endif
}

#pragma FUNC_ALWAYS_INLINE
static inline void MATHLIB_debugPrintVector(c7x::float_vec vector){
#if MATHLIB_DEBUGPRINT
   for (uint i = 0; i < c7x::element_count_of<c7x::float_vec>::value; i++){
      MATHLIB_PRINTF("%f", *((float*)(&vector) + i));
   }

   MATHLIB_PRINTF("%s", "\n");
#endif
}

#pragma FUNC_ALWAYS_INLINE
static inline void MATHLIB_debugPrintVector(c7x::double_vec vector){
#if MATHLIB_DEBUGPRINT
   for (uint i = 0; i < c7x::element_count_of<c7x::double_vec>::value; i++){
      MATHLIB_PRINTF("%lf", *((double*)(&vector) + i));
   }

   MATHLIB_PRINTF("%s", "\n");
#endif
}
#pragma FUNC_ALWAYS_INLINE
static inline void MATHLIB_debugPrintVectorInHex(c7x::long_vec vector){
#if MATHLIB_DEBUGPRINT
   for (uint i = 0; i < c7x::element_count_of<c7x::long_vec>::value; i++){
      MATHLIB_PRINTF("%016lx", *((long*)(&vector) + i));
   }

   MATHLIB_PRINTF("%s", "\n");
#endif
}

#pragma FUNC_ALWAYS_INLINE
static inline void MATHLIB_debugPrintNonScalableVectorInHex(long8 vector)
{
#if MATHLIB_DEBUGPRINT
   MATHLIB_PRINTF("%010lx ", vector.s[7]);
   MATHLIB_PRINTF("%010lx ", vector.s[6]);
   MATHLIB_PRINTF("%010lx ", vector.s[5]);
   MATHLIB_PRINTF("%010lx ", vector.s[4]);
   MATHLIB_PRINTF("%010lx ", vector.s[3]);
   MATHLIB_PRINTF("%010lx ", vector.s[2]);
   MATHLIB_PRINTF("%010lx ", vector.s[1]);
   MATHLIB_PRINTF("%010lx ", vector.s[0]);

   MATHLIB_PRINTF("%s", "\n");
#endif
   return;

}

#endif //#ifdef __cplusplus
#endif /* COMMON_MATHLIB_UTIL_DEBUGPRINTVECTOR_H_ */
