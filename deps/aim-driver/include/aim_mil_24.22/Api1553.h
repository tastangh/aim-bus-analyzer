/* SPDX-FileCopyrightText: 2001-2022 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

/*! \file Api1553.h
 *
 *  This is the main include file for 1553 applications. It includes all the neccessary sub include files for application development.
 *
 */

#ifndef API_1553_H
#define API_1553_H

#include "Ai_cdef.h"
#include "Ai1553Com.h"
#include "Ai1553i_def.h"
#include "Ai1553i_fnc.h"

#ifdef _AIM_VME
#include "AiVmeGeneric.h"
#endif

#ifdef _NI_i386_        /* LabWindows CVI */
  #if !defined(_VXI_PP_WIN32)
    #include "Api1553Cvi.h"
  #endif
#endif

#endif
/* end: 1553_def.h */
