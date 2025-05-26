/* SPDX-FileCopyrightText: 2010-2021 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT OR GPL-2.0-or-later */

/*! \file Ai_types.h
 * 
 * This header file contains AIM specific primitive data types
 */

#ifndef _AI_TYPES_H
#define _AI_TYPES_H




#include "Ai_compiler.h"




/********************* Compiler specific section here for specifying primitive types with fixed width **********************
 ***************************************************************************************************************************/




/***************** These are alias types for built-in ANSI C primitive data types as specified since C99. ******
                   They do specify a fixed width and are usually provided by stdint.h
 *****************************************************************************************/
    typedef int8_t              AiInt8;
    typedef uint8_t             AiUInt8;
    typedef int16_t             AiInt16;
    typedef uint16_t            AiUInt16;
    typedef int32_t             AiInt32;
    typedef uint32_t            AiUInt32;
    typedef int64_t             AiInt64;
    typedef uint64_t            AiUInt64;

    


 /***************** These are alias types for built-in ANSI C primitive data types. ******
                    They do NOT specify a fixed width 
 *****************************************************************************************/
    typedef char                AiChar;
    typedef unsigned char       AiUChar;
    typedef short               AiShort;
    typedef unsigned short      AiUShort;
    typedef int                 AiInt;
    typedef unsigned int        AiUInt;
    typedef long                AiLong;
    typedef unsigned long       AiULong;
    typedef double              AiDouble;
    typedef float               AiFloat;
    typedef size_t              AiSize;
    typedef unsigned char       AiBoolean;
    
    /* Constants to use with Boolean */
    #define AiFalse   (AiBoolean)0
    #define AiTrue    (AiBoolean)1




/****************** These are types for handling pointers ************************************

**********************************************************************************************/
    typedef void*               AiAddr;
    typedef uintptr_t           AiUIntPtr;
    typedef ptrdiff_t           AiPtrDiff;
    typedef AiUInt32            AiTrgVoidPtr;
    typedef AiUInt32            AiTrgUI8Ptr;



/***************** Convenience macros that do not have ***************************************
*                  an equivalent in ANSI C but are used by AIM
*                  APIs or for communication between different
*                  platforms, compilers etc. 
************************************************************************************************/

    typedef AiUInt32            AiBool32;
    typedef AiInt32             AiReturn;
    typedef void*               AiHandle;

	typedef union ai_int64_union
    {
        AiInt64 ll_QuadPart;

		struct ai_int64_union_ll
        {
            AiUInt32 ul_LoPart;
            AiInt32 l_HiPart;
        } ll;
    } Ai_Int64_Union;
    
    typedef union ai_uint64_union
    {
        AiUInt64 ull_QuadPart;

		struct ai_uint64_union_ull
        {
            AiUInt32 ul_LoPart;
            AiUInt32 ul_HiPart;
        } ull;      
    } Ai_UInt64_Union;




#endif
