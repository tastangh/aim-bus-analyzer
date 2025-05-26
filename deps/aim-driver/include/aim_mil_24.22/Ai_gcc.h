/* SPDX-FileCopyrightText: 2010-2021 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT OR GPL-2.0-or-later */

/*! \file Ai_gcc.h
 * 
 * This header file contains GCC specific macros
 */

#ifndef _AI_GCC_H
#define _AI_GCC_H
 
 
/**************** Linux driver section *************************************
 * *************************************************************************/
#if defined __linux__ && defined __KERNEL__

  /* C99 integer types are defined in linux/types.h, not in stdint.h */
  #include <linux/types.h>


/************ This stdint.h check is not fully correct, as **********************
 ************ its availability may depend on C library used. ********************
 ************ We just assume on systems with GCC >= 4.0 the file is present. ****
 ************ This check might have to be adapted when it leads to problems. ****
 ********************************************************************************/
#elif __GNUC__ >= 4

  #include <stdint.h>         /* Header for integer types with fixed width (since C99) */
  #include <stddef.h>         /* ANSI C header for ptrdiff_t, size_t, NULL */
    
 
/**************** Legacy C89 section. Define types of stdint.h by ourself *******
 * ******************************************************************************/
#else


 typedef signed char         int8_t;
 typedef unsigned char       uint8_t;
 typedef signed short        int16_t;
 typedef unsigned short      uint16_t;
 typedef signed int          int32_t;
 typedef unsigned int        uint32_t;
 typedef signed long long    int64_t;
 typedef unsigned long long  uint64_t;

 typedef unsigned long int   uintptr_t;


#endif



 /*! \def AI_INLINE
   Macro for inline functions that should work with all GCC versions
 */
#define AI_INLINE __inline__



/*! \def AI_UNUSED
  Macro to avoid compiler warnings of unreferenced formal parameter
*/
#define AI_UNUSED(x) (void)(x)

#endif
