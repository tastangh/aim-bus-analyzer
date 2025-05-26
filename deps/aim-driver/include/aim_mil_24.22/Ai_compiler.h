/* SPDX-FileCopyrightText: 2010-2021 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT OR GPL-2.0-or-later */

/*! \file Ai_compiler.h
 * 
 * This header file encapsulates compiler specific
 * functionalities
 */

#ifndef _AI_COMPILER_H
#define _AI_COMPILER_H


#ifdef __GNUC__

#include "Ai_gcc.h"

#elif defined __DCC__

#include "Ai_diab.h"

#elif defined _MSC_VER

#include "Ai_msvc.h"

#elif defined __ARMCC_VERSION

#include "Ai_armcc.h"

#elif defined __BORLANDC__

#include "Ai_borland.h"

#elif defined _NI_i386_

#include "Ai_nicc.h"

#else

#error "Unsupported compiler"

#endif

#endif


