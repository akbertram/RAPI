/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 1998-2014    The R Core Team
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

/*
 *  These functions are not part of the API.
 */
#ifndef PRTUTIL_H_
#define PRTUTIL_H_

#include <Rinternals.h> // for R_xlen_t
#include <R_ext/Complex.h>

#define formatLogical      Rf_formatLogical
#define formatInteger      Rf_formatInteger
#define formatReal         Rf_formatReal
#define formatComplex      Rf_formatComplex
#define EncodeLogical      Rf_EncodeLogical
#define EncodeInteger      Rf_EncodeInteger
#define EncodeReal         Rf_EncodeReal
#define EncodeReal0        Rf_EncodeReal0
#define EncodeComplex      Rf_EncodeComplex
#define VectorIndex        Rf_VectorIndex
#define printIntegerVector Rf_printIntegerVector
#define printRealVector    Rf_printRealVector
#define printComplexVector Rf_printComplexVector

#ifdef  __cplusplus
extern "C" {
#endif

/* Computation of printing formats */
void formatLogical(int * x, R_xlen_t n, int * fieldwidth);
void formatInteger(int * x, R_xlen_t n, int * fieldwidth);
void formatReal(double * x, R_xlen_t n, int * w, int * d, int * e, int nsmall);
void formatComplex(Rcomplex * x, R_xlen_t n, int * wr, int * dr, int * er, int * wi, int * di, int * ei, int nsmall);

/* Formating of values */
const char *EncodeLogical(int x, int w);
const char *EncodeInteger(int x, int w);
const char *EncodeReal0(double x, int w, int d, int e, const char * dec);
const char *EncodeComplex(Rcomplex x, int wr, int dr, int er, int wi, int di, int ei, const char * dec);

/* Legacy, misused by packages RGtk2 and qtbase */
const char *EncodeReal(double x, int w, int d, int e, char cdec);


/* Printing */
int	IndexWidth(R_xlen_t n);
void VectorIndex(R_xlen_t i, int w);

//void printLogicalVector(int *, R_xlen_t, int);
void printIntegerVector(int *, R_xlen_t, int);
void printRealVector   (double *, R_xlen_t, int);
void printComplexVector(Rcomplex *, R_xlen_t, int);

#ifdef  __cplusplus
}
#endif

#endif /* PRTUTIL_H_ */
