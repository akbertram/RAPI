/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 1998-2010    The R Core Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, a copy is available at
 *  https://www.R-project.org/Licenses/
 */

/* Included by R.h: API */

#ifndef R_EXT_PRINT_H_
#define R_EXT_PRINT_H_

#ifdef  __cplusplus
/* If the vprintf interface is defined at all in C++ it may only be
   defined in namespace std. */
# ifdef R_USE_C99_IN_CXX
#  include <cstdarg>
#  ifdef __SUNPRO_CC
using _STLP_VENDOR_CSTD::va_list;
#  endif
# endif
extern "C" {
#else
# include <stdarg.h>
#endif

void Rprintf(const char * format, ...);
void REprintf(const char * format, ...);
#if !defined(__cplusplus) || defined R_USE_C99_IN_CXX
void Rvprintf(const char * format, va_list arg);
void REvprintf(const char * format, va_list arg);
#endif

#ifdef  __cplusplus
}
#endif

#endif /* R_EXT_PRINT_H_ */
