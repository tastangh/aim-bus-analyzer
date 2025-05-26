/* SPDX-FileCopyrightText: 2010-2021 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT OR GPL-2.0-or-later */

/*! \file Ai_exports.h
 * 
 * This header file contains macros
 * for exporting library functions on different 
 */

#ifndef _AI_EXPORTS_H
#define _AI_EXPORTS_H


/*! \def AI_CFUNC 
    Allows binding of C++ applications.
    Only internal. Should not be used in API function signature */
#ifdef __cplusplus
  #define AI_CFUNC extern "C"
#else
  #define AI_CFUNC
#endif


/*! \def AI_LIB_FUNC
    Will export a function from a library.
    Must be stated before return value in function signature. E.g. AI_LIB_FUNC AiReturn my_exported_func(...)
    With Visual Studio, it is necessary to define pre-processor macro 'AI_LIB_EXPORT' when building a library
*/
#ifdef _MSC_VER
  #ifdef AI_LIB_EXPORT
    #define AI_LIB_FUNC AI_CFUNC __declspec( dllexport )
  #else
    #define AI_LIB_FUNC AI_CFUNC __declspec( dllimport )
  #endif
#else
  #define AI_LIB_FUNC AI_CFUNC
#endif


/*! \def AI_CALL_CONV 
    Specifies unique calling convention for a function.
    Must be stated after return value in function signature. E.g. AiReturn AI_CALL_CONV my_exported_func(...)
*/
#ifdef _WIN32
  #define AI_CALL_CONV  __cdecl
#else
  #define AI_CALL_CONV
#endif



 


#endif


