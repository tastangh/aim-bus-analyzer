/* SPDX-FileCopyrightText: 2001-2021 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT OR GPL-2.0-or-later */

/*! \file Ai_cdef.h

    Historic header for sharing any stuff between protocols.
    Do not add anything new here!!!
*/
#ifndef AI_CDEF
#define AI_CDEF

#ifdef _AIM_VXI
 #include <vpptype.h>
 #include <visa.h>

 #ifndef _NI_i386_
   #include <windows.h>
 #endif
#endif


#if !defined(_AIM_WINDOWS) && !defined(_AIM_RTX64) && !defined(_AIM_VMEWINDOWS) && !defined(_AIM_VME) && !defined(_AIM_VXI) && !defined(_AIM_LINUX) && !defined(_AIM_QNX) && !defined(_AIM_TARGET_SW) && !defined(_AIM_SYSDRV) && !defined(_VXBUS)
    #error target macro not defined !! _AIM_WINDOWS or _AIM_RTX64 or _AIM_VMEWINDOWS or _AIM_VME or _AIM_VXI or _AIM_LINUX or _AIM_QNX or _AIM_TARGET_SW or _AIM_SYSDRV or _VXBUS
/*
define _AIM_WINDOWS for using the 32-Bit Windows DLL
or
define _AIM_RTX64 for using the 64-Bit Windows RTX64 DLL
or
define _AIM_VME for using the 32-Bit VME library
or
define _AIM_VXI for using the 32-Bit Windows DLL for VXI platforms
or
define _AIM_LINUX for using the 32-Bit Linux library in application 
or
define _VXBUS for using the 64-bit Vx Bus library 
*/
#endif

#include "Ai_compiler.h"
#include "Ai_types.h"
#include "Ai_exports.h"



#if !defined( _AIM_SYSDRV) || !defined(_AIM_WINDOWS)
  /***************************************/
  /* Modifiers                           */
  /***************************************/
  #ifdef _NI_i386_        /* LabWindows CVI (National Instruments) */
    #define _API_DLL_FUNC __stdcall
    #define _API_ISR_FUNC __stdcall
    #define _API_EXTERN
    #define INLINE(Type) Type
  #endif /* _NI_i386 */

  #ifdef __BORLANDC__     /* BORLAND C/C++ */
    #ifdef __WIN32__    /* 32-Bit Windows */
      #define _API_DLL_FUNC _stdcall __declspec(dllexport)
      #define _API_ISR_FUNC _stdcall
      #ifdef __cplusplus
        #define _API_EXTERN extern "C"
      #else
        #define _API_EXTERN
      #endif
    #endif
  #endif /* BORLANDC */

  #ifdef _MSC_VER         /* MICROSOFT C */
    #if defined(_AIM_WINDOWS) || defined(_AIM_RTX64) || defined(_AIM_VXI) || defined(_AIM_VMEWINDOWS) /* 32-Bit Windows */
      #define _API_DLL_FUNC __stdcall
      #define _API_ISR_FUNC __stdcall
      #ifdef __cplusplus
        #define _API_EXTERN extern "C" __declspec( dllexport )
      #else
        #define _API_EXTERN __declspec( dllexport )
      #endif
      #define INLINE(Type) __inline Type
    #endif
    #define AI_DEPRECATED(func) __declspec(deprecated) func
  #endif                  /* Microsoft C */

  #ifdef __GNUC__
    #ifdef _AIM_WINDOWS
      #define _API_DLL_FUNC __declspec( dllexport ) __stdcall
      #define _API_ISR_FUNC __stdcall
    #else
      #define _API_DLL_FUNC
      #define _API_ISR_FUNC
    #endif
    #ifdef __cplusplus
      #define _API_EXTERN extern "C"
    #else
      #define _API_EXTERN
    #endif

    #if( __GNUC__ > 2 )
        #define AI_DEPRECATED(func) func __attribute__ ((deprecated))
    #else
        #define AI_DEPRECATED(func) func 
    #endif
    #define INLINE(Type) __inline__ Type
  #endif                  /* GNU C++ tested with PPC C and VxWorks */


  #ifdef _DIAB_TOOL
    #define _API_DLL_FUNC
    #define _API_ISR_FUNC
    #ifdef __cplusplus
      #define _API_EXTERN extern "C"
    #else
      #define _API_EXTERN
    #endif

    #define AI_DEPRECATED(func) func /* ignore depricated functions here */

    #define INLINE(Type) __inline__ Type
  #endif


  #ifdef __arm
    #define _API_DLL_FUNC
    #define _API_ISR_FUNC
    #ifdef __cplusplus
      #define INLINE(Type) inline Type
      #define _API_EXTERN extern "C"
    #else
      #define INLINE(Type) __inline Type
      #define _API_EXTERN
    #endif

    #define AI_DEPRECATED(func) func /* ignore depricated functions here */
  #endif

#endif /* _AIM_SYSDRV */

#if defined(_AIM_SYSDRV) && defined(_AIM_WINDOWS)
  #define _API_DLL_FUNC __stdcall
  #define _API_ISR_FUNC __stdcall

  #ifdef __cplusplus
    #define _API_EXTERN extern "C"
  #else
    #define _API_EXTERN
  #endif
  #define INLINE(Type) _inline Type

  #define AI_DEPRECATED(func) func /* ignore depricated functions here */
#endif


#ifndef INLINE
  #define INLINE(Type)
#endif


#ifndef AI_DEPRECATED
  #pragma message("WARNING: AI_DEPRECATED is not implemented for this compiler")
  #define AI_DEPRECATED(func) func
#endif


#if defined(_AIM_WINDOWS) || defined(_AIM_RTX64) || defined(_AIM_VMEWINDOWS) || defined(_AIM_QNX) || defined(_AIM_SYSDRV)
  #if !defined(_AIM_VXI) && !defined(_AIM_VME) && !defined(_AIM_LINUX)
    #define HOST_ENDIAN_LITTLE
  #endif
#endif

#if defined(_AIM_VMEWINDOWS)
  #define HOST_ENDIAN_LITTLE
#endif

#ifdef _AIM_LINUX
# if defined(_BIG_ENDIAN) || defined(__BIG_ENDIAN__)
#  define HOST_ENDIAN_BIG
# else
#  define HOST_ENDIAN_LITTLE
# endif
#endif /* _AIM_LINUX */

/* If AXI host is Windows the it has to use LITTLE_ENDIAN (default).    */
/* The AXI adapter has big endian so DLL might declare HOST_ENDIAN_BIG  */
#if defined(_AIM_VXI)
  #ifndef HOST_ENDIAN_LITTLE
    #ifndef HOST_ENDIAN_BIG
      #define HOST_ENDIAN_LITTLE
    #endif
  #endif
#endif

#if defined(_AIM_VME) || defined(_VXBUS)
  #ifndef HOST_ENDIAN_LITTLE
    #ifndef HOST_ENDIAN_BIG
      #define HOST_ENDIAN_BIG
    #endif
  #endif
#endif

#ifndef  FALSE
  #define  FALSE 0
#endif

#ifndef TRUE
  #define TRUE   1
#endif

#ifndef NULL
  #ifdef __cplusplus
    #define  NULL     (0)
  #else
    #define  NULL     ((void *)0)
  #endif

#endif

/******************************************/
/* DATA TYPES for non Windows Environment */
/******************************************/
#if !defined(_AIM_VMEWINDOWS) && (defined(_AIM_VME)) || (defined(_AIM_LINUX) && (!defined(OS_TARGET))) || defined(_AIM_QNX) || defined(_VXBUS)
#ifndef BYTE_DEFINED
  typedef AiUInt8 BYTE;
#define BYTE_DEFINED
#endif

#ifndef WORD_DEFINED
  typedef AiUInt16 WORD;
#define WORD_DEFINED
#endif

  typedef AiUInt32 DWORD;        /* for PPC environment */

#ifndef L_WORD_DEFINED
  typedef AiUInt32 L_WORD;
#define L_WORD_DEFINED
#endif



  typedef void *LPVOID;
  typedef AiUInt32 *LPDWORD;
  typedef AiUInt8 *LPOVERLAPPED; /* rh this is not the right structure type */
  typedef void *HINSTANCE;

  #define INVALID_HANDLE_VALUE  ((AiHandle)(-1))

  /* copied from windef.h */
  #define LOWORD(l)           ((WORD)(l))
  #define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))

  /*removed as in CDR2219*/
  /*#define OutputDebugString(str) (printf("%s", str))*/
#endif                  /* _AIM_VME || _AIM_LINUX || _AIM_VXI */

/************************************/
/* defines Byte swapping            */
/************************************/

/* while a macro reads the source value 4 times, these _BSWAP inline functions
   read the source value only once. This is important on accessing the PCI bus,
   e.g. when reading the interrupt status register or a timeing register.
*/

#if ! defined __MIDL__ && !defined _AIM_TARGET_SW
  static INLINE(AiUInt32) _BSWAP32( AiUInt32 input )
  {
      AiUInt32 result;
      char * bpResult = (char *)&result;
      char * bpInput = (char *)&input;
      
      bpResult[0] = bpInput[3];
      bpResult[1] = bpInput[2];
      bpResult[2] = bpInput[1];
      bpResult[3] = bpInput[0];
      
      return result;
  }
  static INLINE(AiUInt16) _BSWAP16( AiUInt16 input )
  {
      AiUInt16 result;
      char * bpResult = (char *)&result;
      char * bpInput = (char *)&input;
      
      bpResult[0] = bpInput[1];
      bpResult[1] = bpInput[0];
      
      return result;
  }
#elif defined __arm__ && defined _AIM_LINUX
        static inline int _BSWAP32(int val)
        {
            asm volatile (
                    "eor     r3, %1, %1, ror #16\n\t"
                    "bic     r3, r3, #0x00FF0000\n\t"
                    "mov     %0, %1, ror #8\n\t"
                    "eor     %0, %0, r3, lsr #8"
                    : "=r" (val)
                    : "0"(val)
                    : "r3"
            );
            return val;
        }

        static inline int _BSWAP16(int val)
        {
            asm volatile (
                    "and     r3, %1, #0xff \n\t"
                    "mov     %1, %1, lsr #8 \n\t"
                    "orr     %1, %1, r3, lsl #8"
                    : "=r" (val)
                    : "0"(val)
                    : "r3"
            );
            return val;
        }

#elif defined __arm__ && !defined _AIM_LINUX
        __inline int _BSWAP32(int x)
        {
            register int ret = x;
            __asm
            {
                eor x, ret, ret, ror#16
                bic x, x,#0x00FF0000
                mov ret, ret, ror#8
                eor ret, ret, x, lsr#8
            }
            return ret;
        }
        __inline int _BSWAP16(int x)
        {
            register int ret = x;
            __asm
            {
                and x, ret,#0xff
                mov ret, ret, lsr#8
                orr ret, ret, x, lsl#8
            }
            return ret;
        }
#else
        #define _BSWAP32(X)  ((((X)&0x000000FF)<<24)+ (((X)&0x0000FF00)<< 8)+ (((X)&0x00FF0000)>> 8)+ (((X)&0xFF000000)>>24))
        #define _BSWAP16(X)  ((((X)&0x00FF)<<8)+ (((X)&0xFF00)>>8))
#endif

#if defined (HOST_ENDIAN_LITTLE)
  #define BSWAP32_MAC    _BSWAP32
  #define BSWAP16_MAC    _BSWAP16

  #define HOST_TO_BE32(x) _BSWAP32(x)
  #define BE32_TO_HOST(x) _BSWAP32(x)

  #define BSWAP32(X)     (X)
  #define BSWAP16(X)     (X)

#elif defined (HOST_ENDIAN_BIG)
  #define BSWAP32_MAC(X) (X)
  #define BSWAP16_MAC(X) (X)

  #define HOST_TO_BE32(x) (x)
  #define BE32_TO_HOST(x) (x)

  #define BSWAP32        _BSWAP32
  #define BSWAP16        _BSWAP16
  
#else
    #error Endian order not defined HOST_ENDIAN_BIG or HOST_ENDIAN_LITTLE.
#endif

#if defined (HOST_ENDIAN_LITTLE) && defined (HOST_ENDIAN_BIG)
    #error Multiple definition of endian order (HOST_ENDIAN_BIG and HOST_ENDIAN_LITTLE)
#endif

#if defined(_AIM_QNX)
 #define AIM_HAVE_LIBRARY_TO_DRIVER_SHARED_MEM
 #define AIM_PLACE_CMD_ACK_IN_SHARED_MEM
#endif /* _AIM_QNX */

/* causes a compiler error if COND is 0 */
#define AI_STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(COND)?1:-1];

#endif /* AI_CDEF */
/* end: ai_cdef.h */
