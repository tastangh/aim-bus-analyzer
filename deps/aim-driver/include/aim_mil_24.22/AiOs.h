/* SPDX-FileCopyrightText: 2010-2021 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT OR GPL-2.0-or-later */

/*
    Description: Include file for operating system specific functions and headers
*/


#ifndef AIOS_H
#define AIOS_H

#if defined _AIM_WINDOWS || defined _AIM_VMEWINDOWS 
  /* Windows operating systems */
  #include "aioswindows.h"
#elif defined _AIM_RTX64
  /* LabViewRT operating systems */
  #include "AiOsRtx64.h" 
#elif defined _AIM_VXI
  /* LabViewRT operating systems */
  #include "AiOsLabViewRT.h" 
#elif defined _AIM_LINUX
  /* Linux operating systems */
  #include "AiOsLinux.h"
#elif defined __VXWORKS__
  /* VxWorks operating systems */
  #include "AiOsVxWorks.h"
#endif 


#endif /* AIOS_H */
