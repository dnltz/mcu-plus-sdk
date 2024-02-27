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
 **|         Copyright (c) 2007-2017 Texas Instruments Incorporated           |**
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

#include "TI_memory.h"
#include "TI_test.h"
#include "TI_profile.h"

#if defined(_HOST_BUILD)
#include <malloc.h>
#endif

/* Heap global variables */
#if !defined(_HOST_BUILD)

/* Handle COFF/ELF for linker cmd file symbols */
#if defined(__TI_EABI__)
#define sram_start _sram_start
#endif

/* These extern variables are created by the linker command files */
extern uint32_t    sram_start;

static void   *fast_heap_start;
static void   *fast_heap_end;

#if defined(HEAP_L2SRAM)
static uint32_t    fast_heap_size;
static void       *fast_heap_front;
#endif

/* The linker will place this section in L2SRAM after linked section, if any
 * Therefore, the address of this variable indicates the first available
 * address in the L2SRAM for the fast heap to use */
#pragma DATA_SECTION(sram_start_placeholder, ".sram_start")
static uint8_t    sram_start_placeholder;

#endif /* !defined(_HOST_BUILD) */

uint32_t TI_meminit(uint32_t size)
{
   uint32_t    status = 0;

#if !defined(_HOST_BUILD)
   fast_heap_start =  (void *)(size_t)&sram_start_placeholder;
   fast_heap_end   = (void *)((size_t)&sram_start + size * 1024);
   if( fast_heap_start > fast_heap_end ) {
      status = 1; /* ERROR: There is no room in the L2SRAM. */
   }
#if defined(HEAP_L2SRAM)
   else {
      fast_heap_size = (size_t)fast_heap_end - (size_t)fast_heap_start;
      fast_heap_front = fast_heap_start;
   }
#endif
#endif
   return (status);
}

void *TI_memalign(size_t alignment, size_t size)
{
#if !defined(_HOST_BUILD) && defined(HEAP_L2SRAM)
   size_t    temp_start = ((size_t)fast_heap_front + (alignment - 1)) & ~(size_t)(alignment - 1);

   if((temp_start + size) > (size_t)fast_heap_end ) {
      temp_start = 0;
   } else {
      fast_heap_front = (void *)(temp_start + size);
   }
   return ((void *)temp_start);
#else
   return (memalign(alignment, size));
#endif
}

void *TI_malloc(size_t size)
{
#if !defined(_HOST_BUILD) && defined(HEAP_L2SRAM)
   size_t    temp_start = ((size_t)fast_heap_front + (sizeof(long double) - 1)) & ~(size_t)(sizeof(long double) - 1);

   if((temp_start + size) > (size_t)fast_heap_end ) {
      temp_start = 0;
   } else {
      fast_heap_front = (void *)(temp_start + size);
   }
   return ((void *)temp_start);
#else
   return (malloc(size));
#endif
}

void TI_align_free(void *ptr)
{
#if !defined(_HOST_BUILD) && defined(HEAP_L2SRAM)
   fast_heap_front = fast_heap_start;
#else
   align_free(ptr);
#endif
}

void TI_free(void *ptr)
{
#if !defined(_HOST_BUILD) && defined(HEAP_L2SRAM)
   fast_heap_front = fast_heap_start;
#else
   free(ptr);
#endif
}

int32_t TI_get_heap_size (void)
{
#if !defined(_HOST_BUILD) && defined(HEAP_L2SRAM)
   return (fast_heap_size);
#else
   return (0);
#endif
}

void  TI_memError(char *fn)
{
#if !defined(NO_PRINTF)
   sprintf(TI_test_print_buffer, "%s:\n    ERROR: This test can not run given the build-time configuration of L2SRAM.\n", fn);
   TI_test_print_string(TI_test_print_buffer);
#endif
}

/* Common Memory Utility:
 *
 *    TI_compare_mem
 *
 */
int32_t TI_compare_mem (void *a, void *b, int32_t size)
{
   int32_t    test_result = (int32_t) TI_TEST_KERNEL_PASS;

   /* Compare memory (byte-based) */
   if( memcmp(a, b, size)) {
      TI_test_assert((act_kernel == TI_PROFILE_KERNEL_OPT ||
                      act_kernel == TI_PROFILE_KERNEL_CN), "Invalid kernel type");

      /* Provide failure message */
      if( act_kernel == TI_PROFILE_KERNEL_OPT ) {
         sprintf(TI_test_print_buffer, "\nFailure: Output - Optimized:  Case #%d\n", test_index);
         TI_test_print_string(TI_test_print_buffer);
         test_result = (int32_t) TI_TEST_KERNEL_FAIL;
      } else {
         sprintf(TI_test_print_buffer, "\nFailure: Output - Natural C:  Case #%d\n", test_index);
         TI_test_print_string(TI_test_print_buffer);
         test_result = (int32_t) TI_TEST_KERNEL_FAIL;
      }
   }

   /* Return PASS/FAIL */
   return (test_result);
}

/* Common Memory Utility:
 *
 *    TI_compare_mem_2D
 *
 */

int32_t  TI_compare_mem_2D(void *a, void *b, int64_t tolerance, int32_t wrap_exception, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   int32_t   test_result = (int32_t) TI_TEST_KERNEL_PASS;
   uint32_t  row, col;
   uint8_t   warn  = 0;
   uint64_t  signWrapDiff;

   signWrapDiff = 0xFFFFFFFFFFFFFFFFu;

   for( row= 0; row < height; row++ ) {
      for( col=0; col < width; col++ ) {
         int64_t valA, valB;
         int32_t offset;

         offset = (row * (stride / elementSize)) + col;
         valA = 0;
         valB = 0;

         if( elementSize == 1 ) {
            uint8_t   *ptrA = (uint8_t *)a;
            uint8_t   *ptrB = (uint8_t *)b;

            valA = ptrA[offset];
            valB = ptrB[offset];
            signWrapDiff = 0xFFu;
         }
         else if( elementSize == 2 ) {
            uint16_t   *ptrA = (uint16_t *)a;
            uint16_t   *ptrB = (uint16_t *)b;

            valA = ptrA[offset];
            valB = ptrB[offset];
            signWrapDiff = 0xFFFFu;
         }
         else if( elementSize == 4 ) {
            uint32_t   *ptrA = (uint32_t *)a;
            uint32_t   *ptrB = (uint32_t *)b;

            valA = ptrA[offset];
            valB = ptrB[offset];
            signWrapDiff = 0xFFFFFFFFu;
         }
         else if( elementSize == 8 ) {
            uint64_t   *ptrA = (uint64_t *)a;
            uint64_t   *ptrB = (uint64_t *)b;

            valA = ptrA[offset];
            valB = ptrB[offset];
            signWrapDiff = 0xFFFFFFFFFFFFFFFFu;
         }
         else {
            sprintf(TI_test_print_buffer, "\nUnsupported data type in TI_compare_mem_2D.\n");
            TI_test_print_string(TI_test_print_buffer);
         }

         if(valA != valB)
         {
            if(tolerance) {
               int64_t diff = abs(valA - valB);
               if((diff > tolerance) &&
                  ((wrap_exception == 0) || ((uint64_t)diff < signWrapDiff - tolerance + 1))) {
                  sprintf(TI_test_print_buffer, "\nFailure at: row=%d, col=%d, val1=0x%x%x, val2=0x%x%x, tolerance: 0x%x%x\n", row, col, (uint32_t)((valA >> 32) & 0xFFFFFFFFu), (uint32_t)(valA & 0xFFFFFFFFu), (uint32_t)((valB >> 32) & 0xFFFFFFFFu), (uint32_t)(valB & 0xFFFFFFFFu), (uint32_t)((tolerance >> 32) & 0xFFFFFFFFu), (uint32_t)(tolerance & 0xFFFFFFFFu));
                  TI_test_print_string(TI_test_print_buffer);
                  return(TI_TEST_KERNEL_FAIL);
               } else if(warn == 0) {
                  warn = 1;
                  sprintf(TI_test_print_buffer, "\nWarning at: row=%d, col=%d, val1=0x%x%x, val2=0x%x%x\n", row, col, (uint32_t)((valA >> 32) & 0xFFFFFFFFu), (uint32_t)(valA & 0xFFFFFFFFu), (uint32_t)((valB >> 32) & 0xFFFFFFFFu), (uint32_t)(valB & 0xFFFFFFFFu));
                  TI_test_print_string(TI_test_print_buffer);
               }
            } else {
               sprintf(TI_test_print_buffer, "\nFailure at: row=%d, col=%d, val1=0x%x%x, val2=0x%x%x\n", row, col, (uint32_t)((valA >> 32) & 0xFFFFFFFFu), (uint32_t)(valA & 0xFFFFFFFFu), (uint32_t)((valB >> 32) & 0xFFFFFFFFu), (uint32_t)(valB & 0xFFFFFFFFu));
               TI_test_print_string(TI_test_print_buffer);
               return(TI_TEST_KERNEL_FAIL);
            }
         }
      }
   }

   return test_result;
}

int32_t  TI_compare_mem_roi2DDualStride(void *a, void *b, int32_t tolerance, int32_t wrap_exception, uint32_t width, uint32_t height, uint32_t strideA, uint32_t strideB, uint8_t elementSize)
{
   int32_t   test_result = (int32_t) TI_TEST_KERNEL_PASS;
   uint32_t  row, col;
   uint8_t   warn  = 0;

   for( row= 0; row < height; row++ ) {
      for( col=0; col < width; col++ ) {
         int32_t valA, valB;
         int32_t offsetA, offsetB;

         offsetA = (row * (strideA / elementSize)) + col;
         offsetB = (row * (strideB / elementSize)) + col;

         if( elementSize == 1 ) {
            uint8_t   *ptrA = (uint8_t *)a;
            uint8_t   *ptrB = (uint8_t *)b;

            valA = ptrA[offsetA];
            //                valB = ptrB[offsetA];
            valB = ptrB[offsetB];
         }
         else if( elementSize == 2 ) {
            uint16_t   *ptrA = (uint16_t *)a;
            uint16_t   *ptrB = (uint16_t *)b;

            valA = ptrA[offsetA];
            valB = ptrB[offsetB];
         }
         else if( elementSize == 4 ) {
            uint32_t   *ptrA = (uint32_t *)a;
            uint32_t   *ptrB = (uint32_t *)b;

            valA = ptrA[offsetA];
            valB = ptrB[offsetB];
         }
         else if( elementSize == 8 ) {
            uint64_t   *ptrA = (uint64_t *)a;
            uint64_t   *ptrB = (uint64_t *)b;

            valA = ptrA[offsetA];
            valB = ptrB[offsetB];
         }
         else {
            sprintf(TI_test_print_buffer, "\nUnsupported data type in TI_compare_mem_2D.\n");
            TI_test_print_string(TI_test_print_buffer);
         }

         if(valA != valB)
         {
            if(tolerance) {
               int32_t diff = abs(valA - valB);
               if((diff > tolerance) && (diff != wrap_exception)) {
                  sprintf(TI_test_print_buffer, "\nFailure at: row=%d, col=%d, val1=%d, val2=%d\n", row, col, valA, valB);
                  TI_test_print_string(TI_test_print_buffer);
                  return(TI_TEST_KERNEL_FAIL);
               } else if(warn == 0) {
                  warn = 1;
                  sprintf(TI_test_print_buffer, "\nWarning at: row=%d, col=%d, val1=%d, val2=%d\n", row, col, valA, valB);
                  TI_test_print_string(TI_test_print_buffer);
               }
            } else {
               sprintf(TI_test_print_buffer, "\nFailure at: row=%d, col=%d, val1=%d, val2=%d\n", row, col, valA, valB);
               TI_test_print_string(TI_test_print_buffer);
               return(TI_TEST_KERNEL_FAIL);
            }
         }
      }
   }

   return test_result;
}


// assume the stride is the same as the number of cols, as when data is loaded from a static array
int32_t  TI_compare_mem_roi2DStatic(void *a, void *b, int32_t tolerance, int32_t wrap_exception, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   return TI_compare_mem_roi2DDualStride(a, b,tolerance, wrap_exception, width, height, stride, width*elementSize, elementSize);
}

// assume that both roi's have the same stride
// bit depth on tolerance is the only difference between this and TI_compare_mem_2D.  Why have both?
int32_t  TI_compare_mem_roi2D(void *a, void *b, int32_t tolerance, int32_t wrap_exception, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   return TI_compare_mem_roi2DDualStride(a, b,tolerance, wrap_exception, width, height, stride, stride, elementSize);
}


/* Common Memory Utility:
 *
 *    TI_compare_mem_3D
 *
 */

int32_t  TI_compare_mem_3DDualStride(void *a, void *b, int32_t tolerance, int32_t wrap_exception, uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t strideA_y, uint32_t strideA_z, uint32_t strideB_y, uint32_t strideB_z, uint8_t elementSize)
{
   int32_t   test_result = (int32_t) TI_TEST_KERNEL_PASS;
   uint32_t  ctrX, ctrY, ctrZ;
   uint8_t   warn  = 0;

   for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
      for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
         for( ctrX=0; ctrX < dim_x; ctrX++ ) {
            int32_t valA, valB;
            int32_t offsetA, offsetB;

            offsetA = (ctrZ * (strideA_z / elementSize)) + (ctrY * (strideA_y / elementSize)) + ctrX;
            offsetB = (ctrZ * (strideB_z / elementSize)) + (ctrY * (strideB_y / elementSize)) + ctrX;

            if( elementSize == 1 ) {
               uint8_t   *ptrA = (uint8_t *)a;
               uint8_t   *ptrB = (uint8_t *)b;

               valA = ptrA[offsetA];
               valB = ptrB[offsetB];
            }
            else if( elementSize == 2 ) {
               uint16_t   *ptrA = (uint16_t *)a;
               uint16_t   *ptrB = (uint16_t *)b;

               valA = ptrA[offsetA];
               valB = ptrB[offsetB];
            }
            else if( elementSize == 4 ) {
               uint32_t   *ptrA = (uint32_t *)a;
               uint32_t   *ptrB = (uint32_t *)b;

               valA = ptrA[offsetA];
               valB = ptrB[offsetB];
            }
            else if( elementSize == 8 ) {
               uint64_t   *ptrA = (uint64_t *)a;
               uint64_t   *ptrB = (uint64_t *)b;

               valA = ptrA[offsetA];
               valB = ptrB[offsetB];
            }
            else {
               sprintf(TI_test_print_buffer, "\nUnsupported data type in TI_compare_mem_3D.\n");
               TI_test_print_string(TI_test_print_buffer);
            }

            if(valA != valB)
            {
               if(tolerance) {
                  int32_t diff = abs(valA - valB);
                  if((diff > tolerance) && (diff != wrap_exception)) {
                     sprintf(TI_test_print_buffer, "\nFailure at: z=%d, y=%d, x=%d, val1=%d, val2=%d\n", ctrZ, ctrY, ctrX, valA, valB);
                     TI_test_print_string(TI_test_print_buffer);
                     return(TI_TEST_KERNEL_FAIL);
                  } else if(warn == 0) {
                     warn = 1;
                     sprintf(TI_test_print_buffer, "\nWarning at: z=%d, y=%d, x=%d, val1=%d, val2=%d\n", ctrZ, ctrY, ctrX, valA, valB);
                     TI_test_print_string(TI_test_print_buffer);
                  }
               } else {
                  sprintf(TI_test_print_buffer, "\nFailure at: z=%d, y=%d, x=%d, val1=%d, val2=%d\n", ctrZ, ctrY, ctrX, valA, valB);
                  TI_test_print_string(TI_test_print_buffer);
                  return(TI_TEST_KERNEL_FAIL);
               }
            }
         }
      }
   }

   return test_result;
}

// assume the strides are the same for both inputs
int32_t  TI_compare_mem_3D(void *a, void *b, int32_t tolerance, int32_t wrap_exception, uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t stride_y, uint32_t stride_z, uint8_t elementSize)
{
   return TI_compare_mem_3DDualStride(a, b, tolerance, wrap_exception, dim_x, dim_y, dim_z, stride_y, stride_z, stride_y, stride_z, elementSize);
}


// assume the stride is the same as the number of cols, as when data is loaded from a static array
int32_t  TI_compare_mem_roi3DStatic(void *a, void *b, int32_t tolerance, int32_t wrap_exception, uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t stride_y, uint32_t stride_z, uint8_t elementSize)
{
   uint32_t strideB_y = dim_x * elementSize;
   uint32_t strideB_z = strideB_y * dim_y;

   return TI_compare_mem_3DDualStride(a, b, tolerance, wrap_exception, dim_x, dim_y, dim_z, stride_y, stride_z, strideB_y, strideB_z, elementSize);
}

// assume the strides are the same for both inputs
int32_t  TI_compare_mem_roi3D(void *a, void *b, int32_t tolerance, int32_t wrap_exception, uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t stride_y, uint32_t stride_z, uint8_t elementSize)
{
   return TI_compare_mem_3DDualStride(a, b, tolerance, wrap_exception, dim_x, dim_y, dim_z, stride_y, stride_z, stride_y, stride_z, elementSize);
}

/* Common Memory Utility:
 *
 *    TI_compare_mem_2D
 *
 */
int32_t  TI_compare_mem_2D_float(void *a, void *b, double log10tolerance, double abstolerance, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   int32_t   test_result = (int32_t) TI_TEST_KERNEL_PASS;
   uint32_t  row, col;
   uint8_t   warn  = 0;

   for( row= 0; row < height; row++ ) {
      for( col=0; col < width; col++ ) {
         double valA, valB;
         int32_t offset;

         offset = (row * (stride / elementSize)) + col;

         if( elementSize == 4 ) {
            float   *ptrA = (float *)a;
            float   *ptrB = (float *)b;

            valA = ptrA[offset];
            valB = ptrB[offset];
         }
         else if( elementSize == 8 ) {
            double   *ptrA = (double *)a;
            double   *ptrB = (double *)b;

            valA = ptrA[offset];
            valB = ptrB[offset];
         }
         else{
            sprintf(TI_test_print_buffer, "\nUnsupported data type in TI_compare_mem_2D_float.\n");
            TI_test_print_string(TI_test_print_buffer);
         }

         if(valA != valB)
         {
            if(log10tolerance) {
               double log10diff = fabs(log10(valB/valA));
               double absdiff = fabs(valB-valA);
               if((log10diff > log10tolerance) && (absdiff > abstolerance)) {
                  sprintf(TI_test_print_buffer, "\nFailure at: row=%d, col=%d, val1=%f, val2=%f\n", row, col, valA, valB);
                  TI_test_print_string(TI_test_print_buffer);
                  return(TI_TEST_KERNEL_FAIL);
               } else if(warn == 0) {
                  warn = 1;
                  sprintf(TI_test_print_buffer, "\nWarning at: row=%d, col=%d, val1=%f, val2=%f\n", row, col, valA, valB);
                  TI_test_print_string(TI_test_print_buffer);
               }
            } else if(abstolerance) {
               double absdiff = fabs(valB-valA);
               if(absdiff > abstolerance) {
                  sprintf(TI_test_print_buffer, "\nFailure at: row=%d, col=%d, val1=%f, val2=%f\n", row, col, valA, valB);
                  TI_test_print_string(TI_test_print_buffer);
                  return(TI_TEST_KERNEL_FAIL);
               } else if(warn == 0) {
                  warn = 1;
                  sprintf(TI_test_print_buffer, "\nWarning at: row=%d, col=%d, val1=%f, val2=%f\n", row, col, valA, valB);
                  TI_test_print_string(TI_test_print_buffer);
               }
            } else {
               sprintf(TI_test_print_buffer, "\nFailure at: row=%d, col=%d, val1=%f, val2=%f\n", row, col, valA, valB);
               TI_test_print_string(TI_test_print_buffer);
               return(TI_TEST_KERNEL_FAIL);
            }
         }
      }
   }

   return test_result;
}

/* Common Memory Utility:
 *
 *    TI_compare_mem_3D
 *
 */
int32_t  TI_compare_mem_3D_float(void *a, void *b, double log10tolerance, double abstolerance, uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t stride_y, uint32_t stride_z, uint8_t elementSize)
{
   int32_t   test_result = (int32_t) TI_TEST_KERNEL_PASS;
   uint32_t  ctrX, ctrY, ctrZ;
   uint8_t   warn  = 0;

   for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
      for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
         for( ctrX=0; ctrX < dim_x; ctrX++ ) {
            double valA, valB;
            int32_t offset;

            offset = (ctrZ * (stride_z / elementSize)) + (ctrY * (stride_y / elementSize)) + ctrX;

            if( elementSize == 4 ) {
               float   *ptrA = (float *)a;
               float   *ptrB = (float *)b;

               valA = ptrA[offset];
               valB = ptrB[offset];
            }
            else if( elementSize == 8 ) {
               double   *ptrA = (double *)a;
               double   *ptrB = (double *)b;

               valA = ptrA[offset];
               valB = ptrB[offset];
            }
            else{
               sprintf(TI_test_print_buffer, "\nUnsupported data type in TI_compare_mem_3D_float.\n");
               TI_test_print_string(TI_test_print_buffer);
            }

            if(valA != valB)
            {
               if(log10tolerance) {
                  double log10diff = fabs(log10(valB/valA));
                  double absdiff = fabs(valB-valA);
                  if((log10diff > log10tolerance) && (absdiff > abstolerance)) {
                     sprintf(TI_test_print_buffer, "\nFailure at: z=%d, y=%d, x=%d, val1=%f, val2=%f\n", ctrZ, ctrY, ctrX, valA, valB);
                     TI_test_print_string(TI_test_print_buffer);
                     return(TI_TEST_KERNEL_FAIL);
                  } else if(warn == 0) {
                     warn = 1;
                     sprintf(TI_test_print_buffer, "\nWarning at: z=%d, y=%d, x=%d, val1=%f, val2=%f\n", ctrZ, ctrY, ctrX, valA, valB);
                     TI_test_print_string(TI_test_print_buffer);
                  }
               } else if(abstolerance) {
                  double absdiff = fabs(valB-valA);
                  if(absdiff > abstolerance) {
                     sprintf(TI_test_print_buffer, "\nFailure at: z=%d, y=%d, x=%d, val1=%f, val2=%f\n", ctrZ, ctrY, ctrX, valA, valB);
                     TI_test_print_string(TI_test_print_buffer);
                     return(TI_TEST_KERNEL_FAIL);
                  } else if(warn == 0) {
                     warn = 1;
                     sprintf(TI_test_print_buffer, "\nWarning at: z=%d, y=%d, x=%d, val1=%f, val2=%f\n", ctrZ, ctrY, ctrX, valA, valB);
                     TI_test_print_string(TI_test_print_buffer);
                  }
               } else {
                  sprintf(TI_test_print_buffer, "\nFailure at: z=%d, y=%d, x=%d, val1=%f, val2=%f\n", ctrZ, ctrY, ctrX, valA, valB);
                  TI_test_print_string(TI_test_print_buffer);
                  return(TI_TEST_KERNEL_FAIL);
               }
            }
         }
      }
   }

   return test_result;
}

/* 2D Memory Utility:
 *
 *    fillConst
 *
 */
void fillConst(uint32_t val, void *array, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   uint32_t    row, col;

   if( elementSize == 1 ) {
      uint8_t   *ptr = (uint8_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride + col]= val;
         }
      }
   } else if( elementSize == 2 ) {
      uint16_t   *ptr = (uint16_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 2 + col]= val;
         }
      }
   } else if( elementSize == 4 ) {
      uint32_t   *ptr = (uint32_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 4 + col]= val;
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *ptr = (uint64_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 8 + col]= val;
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillConst.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 3D Memory Utility:
 *
 *    fillConst3D
 *
 */
void fillConst3D(uint32_t val, void *array, uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t stride_y, uint32_t stride_z, uint8_t elementSize)
{
   uint32_t  ctrX, ctrY, ctrZ;

   if( elementSize == 1 ) {
      uint8_t   *ptr = (uint8_t *)array;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z) + (ctrY * stride_y) + ctrX]= val;
            }
         }
      }

   } else if( elementSize == 2 ) {
      uint16_t   *ptr = (uint16_t *)array;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/2) + (ctrY * stride_y/2) + ctrX]= val;
            }
         }
      }

   } else if( elementSize == 4 ) {
      uint32_t   *ptr = (uint32_t *)array;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/4) + (ctrY * stride_y/4) + ctrX]= val;
            }
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *ptr = (uint64_t *)array;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/8) + (ctrY * stride_y/8) + ctrX]= val;
            }
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillConst3D.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 2D Memory Utility:
 *
 *    fillSeq
 *
 */
void fillSeq(void *array, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   uint32_t    row, col;

   if( elementSize == 1 ) {
      uint8_t   *ptr = (uint8_t *)array;
      uint8_t    val= 0;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride + col]= val++;
         }
      }
   } else if( elementSize == 2 ) {
      uint16_t   *ptr = (uint16_t *)array;
      uint16_t    val= 0;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 2 + col]= val++;
         }
      }
   } else if( elementSize == 4 ) {
      uint32_t   *ptr = (uint32_t *)array;
      uint32_t    val= 0;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 4 + col]= val++;
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *ptr = (uint64_t *)array;
      uint64_t    val= 0;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 8 + col]= val++;
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillSeq.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 3D Memory Utility:
 *
 *    fillSeq3D
 *
 */
void fillSeq3D(void *array, uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t stride_y, uint32_t stride_z, uint8_t elementSize)
{
   uint32_t  ctrX, ctrY, ctrZ;

   if( elementSize == 1 ) {
      uint8_t   *ptr = (uint8_t *)array;
      uint8_t    val= 0;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z) + (ctrY * stride_y) + ctrX]= val++;
            }
         }
      }
   } else if( elementSize == 2 ) {
      uint16_t   *ptr = (uint16_t *)array;
      uint16_t    val= 0;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/2) + (ctrY * stride_y/2) + ctrX]= val++;
            }
         }
      }
   } else if( elementSize == 4 ) {
      uint32_t   *ptr = (uint32_t *)array;
      uint32_t    val= 0;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/4) + (ctrY * stride_y/4) + ctrX]= val++;
            }
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *ptr = (uint64_t *)array;
      uint64_t    val= 0;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/8) + (ctrY * stride_y/8) + ctrX]= val++;
            }
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillSeq3D.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 2D Memory Utility:
 *
 *    fillSeqNeg
 *
 */
void fillSeqNeg(void *array, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   uint32_t    row, col;

   if( elementSize == 1 ) {
      int8_t   *ptr = (int8_t *)array;
      int8_t    val= 0;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride + col]= val--;
         }
      }
   } else if( elementSize == 2 ) {
      int16_t   *ptr = (int16_t *)array;
      int16_t    val= 0;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 2 + col]= val--;
         }
      }
   } else if( elementSize == 4 ) {
      int32_t   *ptr = (int32_t *)array;
      int32_t    val= 0;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 4 + col]= val--;
         }
      }
   } else if( elementSize == 8 ) {
      int64_t   *ptr = (int64_t *)array;
      int64_t    val= 0;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 8 + col]= val--;
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillSeqNeg.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 3D Memory Utility:
 *
 *    fillSeqNeg3D
 *
 */
void fillSeqNeg3D(void *array, uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t stride_y, uint32_t stride_z, uint8_t elementSize)
{
   uint32_t  ctrX, ctrY, ctrZ;

   if( elementSize == 1 ) {
      uint8_t   *ptr = (uint8_t *)array;
      uint8_t    val= 0;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z) + (ctrY * stride_y) + ctrX]= val--;
            }
         }
      }
   } else if( elementSize == 2 ) {
      uint16_t   *ptr = (uint16_t *)array;
      uint16_t    val= 0;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/2) + (ctrY * stride_y/2) + ctrX]= val--;
            }
         }
      }
   } else if( elementSize == 4 ) {
      uint32_t   *ptr = (uint32_t *)array;
      uint32_t    val= 0;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/4) + (ctrY * stride_y/4) + ctrX]= val--;
            }
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *ptr = (uint64_t *)array;
      uint64_t    val= 0;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/8) + (ctrY * stride_y/8) + ctrX]= val--;
            }
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillSeqNeg3D.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 2D Memory Utility:
 *
 *    copyRoi
 *
 */
void copyRoi(void *arrayDst, void *arraySrc, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   uint32_t    row, col;

   if( elementSize == 1 ) {
      uint8_t   *dst = (uint8_t *)arrayDst;
      uint8_t   *src = (uint8_t *)arraySrc;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            dst[row * stride + col]= src[row * width + col];
         }
      }
   } else if( elementSize == 2 ) {
      uint16_t   *dst = (uint16_t *)arrayDst;
      uint16_t   *src = (uint16_t *)arraySrc;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            dst[row * stride / 2 + col]= src[row * width + col];
         }
      }
   }

   else if( elementSize == 4 ) {
      uint32_t   *dst = (uint32_t *)arrayDst;
      uint32_t   *src = (uint32_t *)arraySrc;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            dst[row * stride / 4 + col]= src[row * width + col];
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *dst = (uint64_t *)arrayDst;
      uint64_t   *src = (uint64_t *)arraySrc;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            dst[row * stride / 8 + col]= src[row * width + col];
         }
      }
   }
   else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in copyRoi.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 2D Memory Utility:
 *
 *    copyRoi
 *
 */
void copyRoiDualStride(void *arrayDst, void *arraySrc, uint32_t width, uint32_t height, uint32_t strideDst, uint32_t strideSrc, uint8_t elementSize)
{
   uint32_t    row, col;

   if( elementSize == 1 ) {
      uint8_t *dst = (uint8_t *)arrayDst;
      uint8_t *src = (uint8_t *)arraySrc;

      for( row = 0; row < height; row++ ) {
         for( col = 0; col < width; col++ ) {
            dst[row*strideDst + col]= src[row * strideSrc + col];
         }
      }
   } else if( elementSize == 2 ) {
      uint16_t *dst = (uint16_t *)arrayDst;
      uint16_t *src = (uint16_t *)arraySrc;

      for( row = 0; row < height; row++ ) {
         for( col = 0; col < width; col++ ) {
            dst[row * strideDst/2 + col] = src[row * strideSrc/2 + col];
         }
      }
   }
   else if( elementSize == 4 ) {
      uint32_t   *dst = (uint32_t *)arrayDst;
      uint32_t   *src = (uint32_t *)arraySrc;

      for( row = 0; row < height; row++ ) {
         for( col = 0; col < width; col++ ) {
            dst[row * strideDst / 4 + col]= src[row * strideSrc/4 + col];
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *dst = (uint64_t *)arrayDst;
      uint64_t   *src = (uint64_t *)arraySrc;

      for( row = 0; row < height; row++ ) {
         for( col = 0; col < width; col++ ) {
            dst[row * strideDst / 8 + col]= src[row * strideSrc/8 + col];
         }
      }
   }
   else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in copyRoiDualStride.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 3D Memory Utility:
 *
 *    copyRoi3D
 *
 */
void copyRoi3D(void *arrayDst, void *arraySrc, uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t stride_y, uint32_t stride_z, uint8_t elementSize)
{
   uint32_t  ctrX, ctrY, ctrZ;

   if( elementSize == 1 ) {
      uint8_t   *dst = (uint8_t *)arrayDst;
      uint8_t   *src = (uint8_t *)arraySrc;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               //    dst[(ctrZ * stride_z) + (ctrY * stride_y) + ctrX]= src[(ctrZ * stride_z) + (ctrY * stride_y) + ctrX];
               dst[(ctrZ * stride_z) + (ctrY * stride_y) + ctrX]= src[(ctrZ * dim_x*dim_y) + (ctrY * dim_x) + ctrX];
            }
         }
      }
   } else if( elementSize == 2 ) {
      uint16_t   *dst = (uint16_t *)arrayDst;
      uint16_t   *src = (uint16_t *)arraySrc;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               //                    dst[(ctrZ * stride_z) + (ctrY * stride_y) + ctrX]= src[(ctrZ * stride_z) + (ctrY * stride_y) + ctrX];
               dst[(ctrZ * stride_z / 2) + (ctrY * stride_y / 2) + ctrX]= src[(ctrZ * dim_x*dim_y) + (ctrY * dim_x) + ctrX];
            }
         }
      }
   }
   else if( elementSize == 4 ) {
      uint32_t   *dst = (uint32_t *)arrayDst;
      uint32_t   *src = (uint32_t *)arraySrc;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               dst[(ctrZ * stride_z / 4) + (ctrY * stride_y / 4) + ctrX]= src[(ctrZ * dim_x*dim_y) + (ctrY * dim_x) + ctrX];
            }
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *dst = (uint64_t *)arrayDst;
      uint64_t   *src = (uint64_t *)arraySrc;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               dst[(ctrZ * stride_z / 8) + (ctrY * stride_y / 8) + ctrX]= src[(ctrZ * dim_x*dim_y) + (ctrY * dim_x) + ctrX];
            }
         }
      }
   }
   else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in copyRoi3D.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 2D Memory Utility:
 *
 *    fillRand
 *
 */
void fillRand(void *array, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   uint32_t    row, col;

   if( elementSize == 1 ) {
      uint8_t   *ptr = (uint8_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride + col]= rand();
         }
      }
   } else if( elementSize == 2 ) {
      uint16_t   *ptr = (uint16_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 2 + col]= rand();
         }
      }
   } else if( elementSize == 4 ) {
      uint32_t   *ptr = (uint32_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 4 + col]= rand();
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *ptr = (uint64_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 8 + col]= rand();
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillRand.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

void fillRandSigned(void *array, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   uint32_t    row, col;

   // RAND_MAX is assumed to be equal to 32767 == SHRT_MAX
   // rand effectively produces 15 random bits, we want elementSize*8 random bits

   if( elementSize == 1 ) {
      int8_t   *ptr = (int8_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride + col]= rand();
         }
      }
   } else if( elementSize == 2 ) {
      int16_t   *ptr = (int16_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            // bits                         [15:1]          [0]
            ptr[row * stride / 2 + col]= (rand() << 1) | (rand() & 0x00000001);
         }
      }
   } else if( elementSize == 4 ) {
      int32_t   *ptr = (int32_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            // bits                        [31:17]            [16:2]            [1:0]
            ptr[row * stride / 4 + col]= (rand() << 17) | (rand() << 2) | (rand() & 0x00000003);
         }
      }
   } else if( elementSize == 8 ) {
      int64_t   *ptr = (int64_t *)array;
      int64_t    temp = 0;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            // bits           [63:49]            [48:34]         [33:19]       [18:4]        [3:0]
            temp = (((int64_t)rand()) <<  49) | (((int64_t)rand()) << 34) | (((int64_t)rand()) << 19) | (((int64_t)rand()) << 4) | (((int64_t)rand()) & 0x000000000000000F);
            ptr[row * stride / 8 + col]= temp;
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillRand.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

void fillRandSmall(void *array, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize, uint8_t maxValue)
{
   uint32_t    row, col;

   if( elementSize == 1 ) {
      uint8_t   *ptr = (uint8_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride + col]= (rand() % (maxValue-1)) + 1;
         }
      }
   } else if( elementSize == 2 ) {
      uint16_t   *ptr = (uint16_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 2 + col]= (rand() % (maxValue-1)) + 1;
         }
      }
   } else if( elementSize == 4 ) {
      uint32_t   *ptr = (uint32_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 4 + col]= (rand() % (maxValue-1)) + 1;
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *ptr = (uint64_t *)array;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * stride / 8 + col]= (rand() % (maxValue-1)) + 1;
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillRandSmall.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}


/* 3D Memory Utility:
 *
 *    fillRand3D
 *
 */
void fillRand3D(void *array, uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t stride_y, uint32_t stride_z, uint8_t elementSize)
{
   uint32_t  ctrX, ctrY, ctrZ;

   if( elementSize == 1 ) {
      uint8_t   *ptr = (uint8_t *)array;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z) + (ctrY * stride_y) + ctrX]= rand();
            }
         }
      }
   } else if( elementSize == 2 ) {
      uint16_t   *ptr = (uint16_t *)array;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/2) + (ctrY * stride_y/2) + ctrX]= rand();
            }
         }
      }

   } else if( elementSize == 4 ) {
      uint32_t   *ptr = (uint32_t *)array;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/4) + (ctrY * stride_y/4) + ctrX]= rand();
            }
         }
      }
   } else if( elementSize == 8 ) {
      uint64_t   *ptr = (uint64_t *)array;

      for( ctrZ= 0; ctrZ < dim_z; ctrZ++ ) {
         for( ctrY= 0; ctrY < dim_y; ctrY++ ) {
            for( ctrX=0; ctrX < dim_x; ctrX++ ) {
               ptr[(ctrZ * stride_z/8) + (ctrY * stride_y/8) + ctrX]= rand();
            }
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillRand3D.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}


/* 2D Memory Utility:
 *
 *    fillBuffer
 *
 */
void TI_fillBuffer(uint8_t testPattern, uint8_t constFill, void *dest, void *src,
                   uint32_t width, uint32_t height, uint32_t stride,
                   uint8_t elementSize, char *testPatternType)
{

   if( testPattern == CONSTANT ) {
      if( dest ) {
         fillConst(constFill, dest, width, height, stride, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "CONSTANT");
#endif
   } else if( testPattern == SEQUENTIAL ) {
      if( dest ) {
         fillSeq(dest, width, height, stride, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "SEQUENTIAL");
#endif
   } else if( testPattern == SEQUENTIAL_NEGATIVE ) {
      if( dest ) {
         fillSeqNeg(dest, width, height, stride, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "SEQUENTIAL_NEGATIVE");
#endif
   } else if( testPattern == STATIC ) {
      if( dest && src ) {
         copyRoi(dest, src, width, height, stride, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "STATIC");
#endif
   } else if( testPattern == RANDOM ) {
      if( dest ) {
         fillRand(dest, width, height, stride, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "RANDOM");
#endif
   } else if( testPattern == RANDOM_SIGNED ) {
      if( dest ) {
         fillRandSigned(dest, width, height, stride, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "RANDOM_SIGNED");
#endif
   } else if( testPattern == RANDOM_SMALL ) {
      if( dest ) {
         fillRandSmall(dest, width, height, stride, elementSize, constFill);  // use constFill as maximum value
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "RANDOM_SMALL");
#endif
   } else {
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "CUSTOM");
#endif
   }
}

/* 3D Memory Utility:
 *
 *    fillBuffer3D
 *
 */
void TI_fillBuffer3D(uint8_t testPattern, uint8_t constFill, void *dest, void *src,
                     uint32_t dim_x, uint32_t dim_y, uint32_t dim_z, uint32_t stride_y, uint32_t stride_z,
                     uint8_t elementSize, char *testPatternType)
{
   if( testPattern == CONSTANT ) {
      if( dest ) {
         fillConst3D(constFill, dest, dim_x, dim_y, dim_z, stride_y, stride_z, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "CONSTANT");
#endif
   } else if( testPattern == SEQUENTIAL ) {
      if( dest ) {
         fillSeq3D(dest, dim_x, dim_y, dim_z, stride_y, stride_z, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "SEQUENTIAL");
#endif
   } else if( testPattern == SEQUENTIAL_NEGATIVE ) {
      if( dest ) {
         fillSeqNeg3D(dest, dim_x, dim_y, dim_z, stride_y, stride_z, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "SEQUENTIAL_NEGATIVE");
#endif
   } else if( testPattern == STATIC ) {
      if( dest && src ) {
         copyRoi3D(dest, src, dim_x, dim_y, dim_z, stride_y, stride_z, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "STATIC");
#endif
   } else if( testPattern == RANDOM ) {
      if( dest ) {
         fillRand3D(dest, dim_x, dim_y, dim_z, stride_y, stride_z, elementSize);
      }
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "RANDOM");
#endif
   } else {
#if !defined(NO_PRINTF)
      sprintf(testPatternType, "CUSTOM");
#endif
   }
}

/* 2D Memory Utility:
 *
 *    compare
 *
 */
uint32_t compare(uint8_t *dsp_output, uint8_t *natc_output, uint32_t width, uint32_t height, uint32_t stride)
{
   uint32_t    row, col;

   for( row= 0; row < height; row++ ) {
      for( col=0; col < width; col++ ) {
         if( dsp_output[row * stride + col] != natc_output[row * stride + col] ) {
            return (1);
         }
      }
   }

   return (0);
}

/* 2D Memory Utility:
 *
 *    copyRoi_float
 *
 */
void copyRoi_float(void *arrayDst, void *arraySrc, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   uint32_t    row, col;

   if( elementSize == 4 ) {
      float   *dst = (float *)arrayDst;
      float   *src = (float *)arraySrc;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            dst[row * stride + col]= src[row * width + col];
         }
      }
   } else if( elementSize == 8 ) {
      double   *dst = (double *)arrayDst;
      double   *src = (double *)arraySrc;

      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            dst[row * stride / 2 + col]= src[row * width + col];
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in copyRoi_float.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 2D Memory Utility:
 *
 *    fillRand_float
 *
 */
void fillRand_float(void *array, uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize)
{
   uint32_t    row, col;
   if( elementSize == 4 ) {
      float    *ptr = (float  *)array;
      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * (stride/elementSize) + col]= ( float  ) ( ( double ) rand() / ( ( double ) RAND_MAX / 2.0 ) ) - 1.0F; \
         }
      }
   } else if (elementSize == 8){
      double    *ptr = (double  *)array;
      for( row= 0; row < height; row++ ) {
         for( col=0; col < width; col++ ) {
            ptr[row * (stride/elementSize) + col]= ( ( double ) rand() / ( ( double ) RAND_MAX / 2.0 ) ) - 1.0F; \
         }
      }
   } else {
      sprintf(TI_test_print_buffer, "\nUnsupported data type in fillRand_float.\n");
      TI_test_print_string(TI_test_print_buffer);
   }
}

/* 2D Memory Utility:
 *
 *    fillBuffer_float
 *
 */
void TI_fillBuffer_float(uint8_t testPattern, void *constFill, void *dest, void *src,
                         uint32_t width, uint32_t height, uint32_t stride, uint8_t elementSize, char *testPatternType)
{
   if( testPattern == CONSTANT ) {
      sprintf(testPatternType, "CONSTANT");
   } else if( testPattern == SEQUENTIAL ) {

      sprintf(testPatternType, "SEQUENTIAL");
   } else if( testPattern == SEQUENTIAL_NEGATIVE ) {

      sprintf(testPatternType, "SEQUENTIAL");
   } else if( testPattern == STATIC ) {
      if( dest && src ) {
         copyRoi_float(dest, src, width, height, stride, elementSize);
      }
      sprintf(testPatternType, "STATIC");
   } else if( testPattern == RANDOM ) {
      if( dest ) {
         fillRand_float(dest, width, height, stride, elementSize);
      }
      sprintf(testPatternType, "RANDOM");
   } else {
      sprintf(testPatternType, "CUSTOM");
   }
}

uint32_t TI_randomDimensionLessThan(uint32_t maxValue){
   return (rand() % (maxValue-1)) + 1;
}


/* ======================================================================== */
/*  End of file:  TI_memory.c                                             */
/* ======================================================================== */
