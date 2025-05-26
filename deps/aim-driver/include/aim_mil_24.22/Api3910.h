/* SPDX-FileCopyrightText: 2001-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file Api3910.h
 *
 *  This is the main include file for 3910 applications. It includes all the neccessary sub include files for application development.
 *
 */

/******************************************************************************
    Description:
******************************************************************************/
#ifndef API_3910_H
#define API_3910_H

#include "Ai_cdef.h"
#include "Ai3910i_def.h"
#include "Ai3910i_fnc.h"
#ifdef _NI_i386_        /* LabWindows CVI */ 
  #if !defined(_VXI_PP_WIN32)
    #include "Api3910Cvi.h"
    #include "AiEfCvi.h"
  #endif
#endif

#endif
/* end: api3910.h */
