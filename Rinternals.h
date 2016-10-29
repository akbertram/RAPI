/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 1995, 1996  Robert Gentleman and Ross Ihaka
 *  Copyright (C) 1999-2016   The R Core Team.
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

/* This file is installed and available to packages, but only a small
   part of the contents is within the API.  See chapter 6 of 'Writing
   R Extensions'.
 */

#ifndef R_INTERNALS_H_
#define R_INTERNALS_H_

// Support for NO_C_HEADERS added in R 3.3.0
#ifdef __cplusplus
# ifndef NO_C_HEADERS
#  include <cstdio>
#  ifdef __SUNPRO_CC
using std::FILE;
#  endif
#  include <climits>
#  include <cstddef>
# endif
extern "C" {
#else
# ifndef NO_C_HEADERS
#  include <stdio.h>
#  include <limits.h> /* for INT_MAX */
#  include <stddef.h> /* for ptrdiff_t */
# endif
#endif

#include <R_ext/Arith.h>
#include <R_ext/Boolean.h>
#include <R_ext/Complex.h>
#include <R_ext/Error.h>  // includes NORET macro
#include <R_ext/Memory.h>
#include <R_ext/Utils.h>
#include <R_ext/Print.h>

#include <R_ext/libextern.h>

typedef unsigned char Rbyte;

/* type for length of (standard, not long) vectors etc */
typedef int R_len_t;
#define R_LEN_T_MAX INT_MAX

/* both config.h and Rconfig.h set SIZEOF_SIZE_T, but Rconfig.h is
   skipped if config.h has already been included. */
#ifndef R_CONFIG_H
# include <Rconfig.h>
#endif

#if ( SIZEOF_SIZE_T > 4 )
# define LONG_VECTOR_SUPPORT
#endif

#ifdef LONG_VECTOR_SUPPORT
    typedef ptrdiff_t R_xlen_t;
    typedef struct { R_xlen_t lv_length, lv_truelength; } R_long_vec_hdr_t;
# define R_XLEN_T_MAX 4503599627370496
# define R_SHORT_LEN_MAX 2147483647
# define R_LONG_VEC_TOKEN -1
#else
    typedef int R_xlen_t;
# define R_XLEN_T_MAX R_LEN_T_MAX
#endif

#ifndef TESTING_WRITE_BARRIER
# define INLINE_PROTECT
#endif

/* Fundamental Data Types:  These are largely Lisp
 * influenced structures, with the exception of LGLSXP,
 * INTSXP, REALSXP, CPLXSXP and STRSXP which are the
 * element types for S-like data objects.
 *
 *			--> TypeTable[] in ../main/util.c for  typeof()
 */

/*  These exact numeric values are seldom used, but they are, e.g., in
 *  ../main/subassign.c, and they are serialized.
*/
#ifndef enum_SEXPTYPE
/* NOT YET using enum:
 *  1)	The SEXPREC struct below has 'SEXPTYPE type : 5'
 *	(making FUNSXP and CLOSXP equivalent in there),
 *	giving (-Wall only ?) warnings all over the place
 * 2)	Many switch(type) { case ... } statements need a final `default:'
 *	added in order to avoid warnings like [e.g. l.170 of ../main/util.c]
 *	  "enumeration value `FUNSXP' not handled in switch"
 */
typedef unsigned int SEXPTYPE;

#define NILSXP	     0	  /* nil = NULL */
#define SYMSXP	     1	  /* symbols */
#define LISTSXP	     2	  /* lists of dotted pairs */
#define CLOSXP	     3	  /* closures */
#define ENVSXP	     4	  /* environments */
#define PROMSXP	     5	  /* promises: [un]evaluated closure arguments */
#define LANGSXP	     6	  /* language constructs (special lists) */
#define SPECIALSXP   7	  /* special forms */
#define BUILTINSXP   8	  /* builtin non-special forms */
#define CHARSXP	     9	  /* "scalar" string type (internal only)*/
#define LGLSXP	    10	  /* logical vectors */
/* 11 and 12 were factors and ordered factors in the 1990s */
#define INTSXP	    13	  /* integer vectors */
#define REALSXP	    14	  /* real variables */
#define CPLXSXP	    15	  /* complex variables */
#define STRSXP	    16	  /* string vectors */
#define DOTSXP	    17	  /* dot-dot-dot object */
#define ANYSXP	    18	  /* make "any" args work.
			     Used in specifying types for symbol
			     registration to mean anything is okay  */
#define VECSXP	    19	  /* generic vectors */
#define EXPRSXP	    20	  /* expressions vectors */
#define BCODESXP    21    /* byte code */
#define EXTPTRSXP   22    /* external pointer */
#define WEAKREFSXP  23    /* weak reference */
#define RAWSXP      24    /* raw bytes */
#define S4SXP       25    /* S4, non-vector */

/* used for detecting PROTECT issues in memory.c */
#define NEWSXP      30    /* fresh node created in new page */
#define FREESXP     31    /* node released by GC */

#define FUNSXP      99    /* Closure or Builtin or Special */


#else /* NOT YET */
/*------ enum_SEXPTYPE ----- */
typedef enum {
    NILSXP	= 0,	/* nil = NULL */
    SYMSXP	= 1,	/* symbols */
    LISTSXP	= 2,	/* lists of dotted pairs */
    CLOSXP	= 3,	/* closures */
    ENVSXP	= 4,	/* environments */
    PROMSXP	= 5,	/* promises: [un]evaluated closure arguments */
    LANGSXP	= 6,	/* language constructs (special lists) */
    SPECIALSXP	= 7,	/* special forms */
    BUILTINSXP	= 8,	/* builtin non-special forms */
    CHARSXP	= 9,	/* "scalar" string type (internal only)*/
    LGLSXP	= 10,	/* logical vectors */
    INTSXP	= 13,	/* integer vectors */
    REALSXP	= 14,	/* real variables */
    CPLXSXP	= 15,	/* complex variables */
    STRSXP	= 16,	/* string vectors */
    DOTSXP	= 17,	/* dot-dot-dot object */
    ANYSXP	= 18,	/* make "any" args work */
    VECSXP	= 19,	/* generic vectors */
    EXPRSXP	= 20,	/* expressions vectors */
    BCODESXP	= 21,	/* byte code */
    EXTPTRSXP	= 22,	/* external pointer */
    WEAKREFSXP	= 23,	/* weak reference */
    RAWSXP	= 24,	/* raw bytes */
    S4SXP	= 25,	/* S4 non-vector */

    NEWSXP      = 30,   /* fresh node creaed in new page */
    FREESXP     = 31,   /* node released by GC */

    FUNSXP	= 99	/* Closure or Builtin */
} SEXPTYPE;
#endif

/* These are also used with the write barrier on, in attrib.c and util.c */
#define TYPE_BITS 5
#define MAX_NUM_SEXPTYPE (1<<TYPE_BITS)

// ======================= USE_RINTERNALS section
#ifdef USE_RINTERNALS
/* This is intended for use only within R itself.
 * It defines internal structures that are otherwise only accessible
 * via SEXP, and macros to replace many (but not all) of accessor functions
 * (which are always defined).
 */

/* Flags */


struct sxpinfo_struct {
    SEXPTYPE type      :  TYPE_BITS;/* ==> (FUNSXP == 99) %% 2^5 == 3 == CLOSXP
			     * -> warning: `type' is narrower than values
			     *              of its type
			     * when SEXPTYPE was an enum */
    unsigned int obj   :  1;
    unsigned int named :  2;
    unsigned int gp    : 16;
    unsigned int mark  :  1;
    unsigned int debug :  1;
    unsigned int trace :  1;  /* functions and memory tracing */
    unsigned int spare :  1;  /* currently unused */
    unsigned int gcgen :  1;  /* old generation number */
    unsigned int gccls :  3;  /* node class */
}; /*		    Tot: 32 */

struct vecsxp_struct {
    R_len_t	length;
    R_len_t	truelength;
};

struct primsxp_struct {
    int offset;
};

struct symsxp_struct {
    struct SEXPREC *pname;
    struct SEXPREC *value;
    struct SEXPREC *internal;
};

struct listsxp_struct {
    struct SEXPREC *carval;
    struct SEXPREC *cdrval;
    struct SEXPREC *tagval;
};

struct envsxp_struct {
    struct SEXPREC *frame;
    struct SEXPREC *enclos;
    struct SEXPREC *hashtab;
};

struct closxp_struct {
    struct SEXPREC *formals;
    struct SEXPREC *body;
    struct SEXPREC *env;
};

struct promsxp_struct {
    struct SEXPREC *value;
    struct SEXPREC *expr;
    struct SEXPREC *env;
};

/* Every node must start with a set of sxpinfo flags and an attribute
   field. Under the generational collector these are followed by the
   fields used to maintain the collector's linked list structures. */

/* Define SWITH_TO_REFCNT to use reference counting instead of the
   'NAMED' mechanism. This uses the R-devel binary layout. The two
   'named' field bits are used for the REFCNT, so REFCNTMAX is 3. */
//#define SWITCH_TO_REFCNT

#if defined(SWITCH_TO_REFCNT) && ! defined(COMPUTE_REFCNT_VALUES)
# define COMPUTE_REFCNT_VALUES
#endif
#define REFCNTMAX (4 - 1)

#define SEXPREC_HEADER \
    struct sxpinfo_struct sxpinfo; \
    struct SEXPREC *attrib; \
    struct SEXPREC *gengc_next_node, *gengc_prev_node

/* The standard node structure consists of a header followed by the
   node data. */
typedef struct SEXPREC {
    SEXPREC_HEADER;
    union {
	struct primsxp_struct primsxp;
	struct symsxp_struct symsxp;
	struct listsxp_struct listsxp;
	struct envsxp_struct envsxp;
	struct closxp_struct closxp;
	struct promsxp_struct promsxp;
    } u;
} SEXPREC, *SEXP;

/* The generational collector uses a reduced version of SEXPREC as a
   header in vector nodes.  The layout MUST be kept consistent with
   the SEXPREC definition.  The standard SEXPREC takes up 7 words on
   most hardware; this reduced version should take up only 6 words.
   In addition to slightly reducing memory use, this can lead to more
   favorable data alignment on 32-bit architectures like the Intel
   Pentium III where odd word alignment of doubles is allowed but much
   less efficient than even word alignment. */
typedef struct VECTOR_SEXPREC {
    SEXPREC_HEADER;
    struct vecsxp_struct vecsxp;
} VECTOR_SEXPREC, *VECSEXP;

typedef union { VECTOR_SEXPREC s; double align; } SEXPREC_ALIGN;

/* General Cons Cell Attributes */
#define ATTRIB(x)	((x)->attrib)
#define OBJECT(x)	((x)->sxpinfo.obj)
#define MARK(x)		((x)->sxpinfo.mark)
#define TYPEOF(x)	((x)->sxpinfo.type)
#define NAMED(x)	((x)->sxpinfo.named)
#define RTRACE(x)	((x)->sxpinfo.trace)
#define LEVELS(x)	((x)->sxpinfo.gp)
#define SET_OBJECT(x,v)	(((x)->sxpinfo.obj)=(v))
#define SET_TYPEOF(x,v)	(((x)->sxpinfo.type)=(v))
#define SET_NAMED(x,v)	(((x)->sxpinfo.named)=(v))
#define SET_RTRACE(x,v)	(((x)->sxpinfo.trace)=(v))
#define SETLEVELS(x,v)	(((x)->sxpinfo.gp)=((unsigned short)v))

#if defined(COMPUTE_REFCNT_VALUES)
# define REFCNT(x) ((x)->sxpinfo.named)
# define TRACKREFS(x) (TYPEOF(x) == CLOSXP ? TRUE : ! (x)->sxpinfo.spare)
#else
# define REFCNT(x) 0
# define TRACKREFS(x) FALSE
#endif

#ifdef SWITCH_TO_REFCNT
# undef NAMED
# undef SET_NAMED
# define NAMED(x) REFCNT(x)
# define SET_NAMED(x, v) do {} while (0)
#endif

/* S4 object bit, set by R_do_new_object for all new() calls */
#define S4_OBJECT_MASK ((unsigned short)(1<<4))
#define IS_S4_OBJECT(x) ((x)->sxpinfo.gp & S4_OBJECT_MASK)
#define SET_S4_OBJECT(x) (((x)->sxpinfo.gp) |= S4_OBJECT_MASK)
#define UNSET_S4_OBJECT(x) (((x)->sxpinfo.gp) &= ~S4_OBJECT_MASK)

/* Vector Access Macros */
#ifdef LONG_VECTOR_SUPPORT
# define IS_LONG_VEC(x) (SHORT_VEC_LENGTH(x) == R_LONG_VEC_TOKEN)
# define SHORT_VEC_LENGTH(x) (((VECSEXP) (x))->vecsxp.length)
# define SHORT_VEC_TRUELENGTH(x) (((VECSEXP) (x))->vecsxp.truelength)
# define LONG_VEC_LENGTH(x) ((R_long_vec_hdr_t *) (x))[-1].lv_length
# define LONG_VEC_TRUELENGTH(x) ((R_long_vec_hdr_t *) (x))[-1].lv_truelength
# define XLENGTH(x) (IS_LONG_VEC(x) ? LONG_VEC_LENGTH(x) : SHORT_VEC_LENGTH(x))
# define XTRUELENGTH(x)	(IS_LONG_VEC(x) ? LONG_VEC_TRUELENGTH(x) : SHORT_VEC_TRUELENGTH(x))
# define LENGTH(x) (IS_LONG_VEC(x) ? R_BadLongVector(x, __FILE__, __LINE__) : SHORT_VEC_LENGTH(x))
# define TRUELENGTH(x) (IS_LONG_VEC(x) ? R_BadLongVector(x, __FILE__, __LINE__) : SHORT_VEC_TRUELENGTH(x))
# define SET_SHORT_VEC_LENGTH(x,v) (SHORT_VEC_LENGTH(x) = (v))
# define SET_SHORT_VEC_TRUELENGTH(x,v) (SHORT_VEC_TRUELENGTH(x) = (v))
# define SET_LONG_VEC_LENGTH(x,v) (LONG_VEC_LENGTH(x) = (v))
# define SET_LONG_VEC_TRUELENGTH(x,v) (LONG_VEC_TRUELENGTH(x) = (v))
# define SETLENGTH(x,v) do { \
      SEXP sl__x__ = (x); \
      R_xlen_t sl__v__ = (v); \
      if (IS_LONG_VEC(sl__x__)) \
	  SET_LONG_VEC_LENGTH(sl__x__,  sl__v__); \
      else SET_SHORT_VEC_LENGTH(sl__x__, (R_len_t) sl__v__); \
  } while (0)
# define SET_TRUELENGTH(x,v) do { \
      SEXP sl__x__ = (x); \
      R_xlen_t sl__v__ = (v); \
      if (IS_LONG_VEC(sl__x__)) \
	  SET_LONG_VEC_TRUELENGTH(sl__x__, sl__v__); \
      else SET_SHORT_VEC_TRUELENGTH(sl__x__, (R_len_t) sl__v__); \
  } while (0)
# define IS_SCALAR(x, type) (TYPEOF(x) == (type) && SHORT_VEC_LENGTH(x) == 1)
#else
# define SHORT_VEC_LENGTH(x) (((VECSEXP) (x))->vecsxp.length)
# define LENGTH(x)	(((VECSEXP) (x))->vecsxp.length)
# define TRUELENGTH(x)	(((VECSEXP) (x))->vecsxp.truelength)
# define XLENGTH(x) LENGTH(x)
# define XTRUELENGTH(x) TRUELENGTH(x)
# define SETLENGTH(x,v)		((((VECSEXP) (x))->vecsxp.length)=(v))
# define SET_TRUELENGTH(x,v)	((((VECSEXP) (x))->vecsxp.truelength)=(v))
# define SET_SHORT_VEC_LENGTH SETLENGTH
# define SET_SHORT_VEC_TRUELENGTH SET_TRUELENGTH
# define IS_LONG_VEC(x) 0
# define IS_SCALAR(x, type) (TYPEOF(x) == (type) && LENGTH(x) == 1)
#endif

/* Under the generational allocator the data for vector nodes comes
   immediately after the node structure, so the data address is a
   known offset from the node SEXP. */
#define DATAPTR(x)	(((SEXPREC_ALIGN *) (x)) + 1)
#define CHAR(x)		((const char *) DATAPTR(x))
#define LOGICAL(x)	((int *) DATAPTR(x))
#define INTEGER(x)	((int *) DATAPTR(x))
#define RAW(x)		((Rbyte *) DATAPTR(x))
#define COMPLEX(x)	((Rcomplex *) DATAPTR(x))
#define REAL(x)		((double *) DATAPTR(x))
#define STRING_ELT(x,i)	((SEXP *) DATAPTR(x))[i]
#define VECTOR_ELT(x,i)	((SEXP *) DATAPTR(x))[i]
#define STRING_PTR(x)	((SEXP *) DATAPTR(x))
#define VECTOR_PTR(x)	((SEXP *) DATAPTR(x))

/* List Access Macros */
/* These also work for ... objects */
#define LISTVAL(x)	((x)->u.listsxp)
#define TAG(e)		((e)->u.listsxp.tagval)
#define CAR(e)		((e)->u.listsxp.carval)
#define CDR(e)		((e)->u.listsxp.cdrval)
#define CAAR(e)		CAR(CAR(e))
#define CDAR(e)		CDR(CAR(e))
#define CADR(e)		CAR(CDR(e))
#define CDDR(e)		CDR(CDR(e))
#define CDDDR(e)	CDR(CDR(CDR(e)))
#define CADDR(e)	CAR(CDR(CDR(e)))
#define CADDDR(e)	CAR(CDR(CDR(CDR(e))))
#define CAD4R(e)	CAR(CDR(CDR(CDR(CDR(e)))))
#define MISSING_MASK	15 /* reserve 4 bits--only 2 uses now */
#define MISSING(x)	((x)->sxpinfo.gp & MISSING_MASK)/* for closure calls */
#define SET_MISSING(x,v) do { \
  SEXP __x__ = (x); \
  int __v__ = (v); \
  int __other_flags__ = __x__->sxpinfo.gp & ~MISSING_MASK; \
  __x__->sxpinfo.gp = __other_flags__ | __v__; \
} while (0)

/* Closure Access Macros */
#define FORMALS(x)	((x)->u.closxp.formals)
#define BODY(x)		((x)->u.closxp.body)
#define CLOENV(x)	((x)->u.closxp.env)
#define RDEBUG(x)	((x)->sxpinfo.debug)
#define SET_RDEBUG(x,v)	(((x)->sxpinfo.debug)=(v))
#define RSTEP(x)	((x)->sxpinfo.spare)
#define SET_RSTEP(x,v)	(((x)->sxpinfo.spare)=(v))

/* Symbol Access Macros */
#define PRINTNAME(x)	((x)->u.symsxp.pname)
#define SYMVALUE(x)	((x)->u.symsxp.value)
#define INTERNAL(x)	((x)->u.symsxp.internal)
#define DDVAL_MASK	1
#define DDVAL(x)	((x)->sxpinfo.gp & DDVAL_MASK) /* for ..1, ..2 etc */
#define SET_DDVAL_BIT(x) (((x)->sxpinfo.gp) |= DDVAL_MASK)
#define UNSET_DDVAL_BIT(x) (((x)->sxpinfo.gp) &= ~DDVAL_MASK)
#define SET_DDVAL(x,v) ((v) ? SET_DDVAL_BIT(x) : UNSET_DDVAL_BIT(x)) /* for ..1, ..2 etc */

/* Environment Access Macros */
#define FRAME(x)	((x)->u.envsxp.frame)
#define ENCLOS(x)	((x)->u.envsxp.enclos)
#define HASHTAB(x)	((x)->u.envsxp.hashtab)
#define ENVFLAGS(x)	((x)->sxpinfo.gp)	/* for environments */
#define SET_ENVFLAGS(x,v)	(((x)->sxpinfo.gp)=(v))

#else /* not USE_RINTERNALS */
// ======================= not USE_RINTERNALS section

typedef struct SEXPREC *SEXP;

#define CHAR(x)		R_CHAR(x)

/** @brief Access the content of CHARSXP as a C-style string.
 *
 * @param x \c pointer to a CHARSXP .
 *
 * @return \c const pointer to character 0 of \a x .
 */
const char *(R_CHAR)(SEXP x);

/* Various tests with macro versions in the second USE_RINTERNALS section */

/** @brief Is this the null value?
 *
 * @param s Pointer to an R value.
 *
 * @return TRUE iff the R value pointed to by \a s is either
 * <tt>R_NilValue</tt> or has SEXPTYPE <tt>NILSXP</tt>.
 */
Rboolean (Rf_isNull)(SEXP s);

/** @brief Test if an R value is a symbol.
 *
 * @param s Pointer to an R value.
 *
 * @return TRUE iff \a s points to an R value with SEXPTYPE
 *         SYMSXP.
 */
Rboolean (Rf_isSymbol)(SEXP s);

/**
 * @param s Pointer to an R value.
 * @return TRUE iff the R value pointed to by \a s is a
 *         logical vector.
 */
Rboolean (Rf_isLogical)(SEXP s);

/**
 * @param s Pointer to an R value.
 * @return TRUE iff the R value pointed to by \a s is a real vector.
 */
Rboolean (Rf_isReal)(SEXP s);

/**
 * @param s Pointer to an R value.
 * @return TRUE iff the R value pointed to by \a s is a complex vector.
 */
Rboolean (Rf_isComplex)(SEXP s);

/**
 * @param s Pointer to an R value.
 * @return TRUE iff the R value pointed to by \a s is an expression.
 */
Rboolean (Rf_isExpression)(SEXP s);

/** @brief Is this an environment?
 *
 * @param s Pointer to a R value.
 *
 * @return TRUE iff the value pointed to by s is an environment.
 */
Rboolean (Rf_isEnvironment)(SEXP s);

/**
 * @param s Pointer to an R value.
 * @return TRUE iff the R value pointed to by \a s is a vector of
 *         strings.
 */
Rboolean (Rf_isString)(SEXP s);

/** @brief Does an object have a class attribute?
 *
 * @param s Pointer to an R value.
 *
 * @return TRUE iff the object pointed to by \a s has a
 * class attribute.
 */
Rboolean (Rf_isObject)(SEXP s);

# define IS_SCALAR(x, type) (TYPEOF(x) == (type) && XLENGTH(x) == 1)
#endif /* USE_RINTERNALS */

#define IS_SIMPLE_SCALAR(x, type) \
    (IS_SCALAR(x, type) && ATTRIB(x) == R_NilValue)

#define NAMEDMAX 2
#define INCREMENT_NAMED(x) do {				\
	SEXP __x__ = (x);				\
	if (NAMED(__x__) != NAMEDMAX)			\
	    SET_NAMED(__x__, NAMED(__x__) + 1);		\
    } while (0)

#if defined(COMPUTE_REFCNT_VALUES)
# define SET_REFCNT(x,v) (REFCNT(x) = (v))
# if defined(EXTRA_REFCNT_FIELDS)
#  define SET_TRACKREFS(x,v) (TRACKREFS(x) = (v))
# else
#  define SET_TRACKREFS(x,v) ((x)->sxpinfo.spare = ! (v))
# endif
# define DECREMENT_REFCNT(x) do {					\
	SEXP drc__x__ = (x);						\
	if (REFCNT(drc__x__) > 0 && REFCNT(drc__x__) < REFCNTMAX)	\
	    SET_REFCNT(drc__x__, REFCNT(drc__x__) - 1);			\
    } while (0)
# define INCREMENT_REFCNT(x) do {			      \
	SEXP irc__x__ = (x);				      \
	if (REFCNT(irc__x__) < REFCNTMAX)		      \
	    SET_REFCNT(irc__x__, REFCNT(irc__x__) + 1);	      \
    } while (0)
#else
# define SET_REFCNT(x,v) do {} while(0)
# define SET_TRACKREFS(x,v) do {} while(0)
# define DECREMENT_REFCNT(x) do {} while(0)
# define INCREMENT_REFCNT(x) do {} while(0)
#endif

#define ENABLE_REFCNT(x) SET_TRACKREFS(x, TRUE)
#define DISABLE_REFCNT(x) SET_TRACKREFS(x, FALSE)

/* Macros for some common idioms. */
#ifdef SWITCH_TO_REFCNT
# define MAYBE_SHARED(x) (REFCNT(x) > 1)
# define NO_REFERENCES(x) (REFCNT(x) == 0)
# define MARK_NOT_MUTABLE(x) SET_REFCNT(x, REFCNTMAX)
#else
# define MAYBE_SHARED(x) (NAMED(x) > 1)
# define NO_REFERENCES(x) (NAMED(x) == 0)
# define MARK_NOT_MUTABLE(x) SET_NAMED(x, NAMEDMAX)
#endif
#define MAYBE_REFERENCED(x) (! NO_REFERENCES(x))
#define NOT_SHARED(x) (! MAYBE_SHARED(x))

/* Complex assignment support */
/* temporary definition that will need to be refined to distinguish
   getter from setter calls */
#define IS_GETTER_CALL(call) (CADR(call) == R_TmpvalSymbol)

/* Accessor functions.  Many are declared using () to avoid the macro
   definitions in the USE_RINTERNALS section.
   The function STRING_ELT is used as an argument to arrayAssign even
   if the macro version is in use.
*/

/* General Cons Cell Attributes */

/** @brief Get the attributes of an R value.
 *
 * @param x Pointer to the value whose attributes are required.
 *
 * @return Pointer to the attributes object of \a x , which will either be
 *    a pairlist or R_NilValue.
 */
SEXP (ATTRIB)(SEXP x);

/** @brief Does an value have a class attribute?
 *
 * @param x Pointer to an R object.
 *
 * @return true iff \a x has a class attribute.
 */
int  (OBJECT)(SEXP x);
int  (MARK)(SEXP x);

/** @brief Get object's SEXPTYPE.
 *
 * @param x Pointer to R value.
 *
 * @return SEXPTYPE of \a x .
 */
int  (TYPEOF)(SEXP x);

/** @brief Get object copying status.
 *
 * @param x Pointer to a R value.
 *
 * @return Refer to 'R Internals' document.
 */
int  (NAMED)(SEXP x);
int  (REFCNT)(SEXP x);
void (SET_OBJECT)(SEXP x, int v);
void (SET_TYPEOF)(SEXP x, int v);

/** @brief Set object copying status.
 *
 * @param x Pointer to R value.  The function does nothing
 *          if \a x is R_NilValue.
 *
 * @param v Refer to 'R Internals' document.
 */
void (SET_NAMED)(SEXP x, int v);

/** @brief Replace an object's attributes.
 *
 * @param x Pointer to an R value.
 *
 * @param v Pointer to a pairlist giving the new attributes of \a
 *          x or R_NilValue.  \a x should be considered to assume ownership of
 *          the 'car' values in \a v ; they should therefore not
 *          be subsequently altered externally.
 */
void SET_ATTRIB(SEXP x, SEXP v);

/** @brief Replace the attributes of \a to by those of \a from.
 *
 * The status of \a to as an S4 Object is also copied from \a from .
 *
 * @param to Pointer to a R value.
 *
 * @param from Pointer to another R Value.
 */
void DUPLICATE_ATTRIB(SEXP to, SEXP from);
void SHALLOW_DUPLICATE_ATTRIB(SEXP to, SEXP from);

/* S4 object testing */

/** @brief Is this an S4 object?
 *
 * @param x Pointer to a R value.
 *
 * @return true iff \a x is an S4 object.
 */
int (IS_S4_OBJECT)(SEXP x);
void (SET_S4_OBJECT)(SEXP x);
void (UNSET_S4_OBJECT)(SEXP x);

/* Vector Access Functions */
int  (LENGTH)(SEXP x);
int  (TRUELENGTH)(SEXP x);

/**
 * Set length of vector.
 *
 * @param x Pointer to a vector object.
 *
 * @param v The required new length, which must not be greater than
 *          the original length of the vector.
 */
void (SETLENGTH)(SEXP x, int v);

/**
 * Set 'true length' of vector.
 *
 * @param x Pointer to a vector object.
 *
 * @param v The required new 'true length'.
 */

void (SET_TRUELENGTH)(SEXP x, int v);

/**
 * @param x Pointer to a vector object.
 *
 * @return The length of \a x, or 0 if \a x is R_NilValue.  (In
 *         the case of certain hash tables, this means the 'capacity'
 *         of \a x , not all of which may be used.)
 */
R_xlen_t  (XLENGTH)(SEXP x);

/**
 * @param x Pointer to a vector object.
 *
 * @return The 'true length' of \a x.  According to the R Internals
 *         document for R 2.4.1, this is only used for certain hash
 *         tables, and signifies the number of used slots in the
 *         table.
 */
R_xlen_t  (XTRUELENGTH)(SEXP x);
int  (IS_LONG_VEC)(SEXP x);
int  (LEVELS)(SEXP x);
int  (SETLEVELS)(SEXP x, int v);

/**
 * @param x Pointer to a logical vector.
 *
 * @return Pointer to the first element of \a x .
 */
int  *(LOGICAL)(SEXP x);

/**
 * @param x Pointer to an \c IntVector or a \c LogicalVector (i.e. an
 *          R integer or logical vector).
 *
 * @return Pointer to the first element of \a x .
 */
int  *(INTEGER)(SEXP x);

/**
 * @param x Pointer to a raw vector.
 *
 * @return Pointer to the first element of \a x .
 */
Rbyte *(RAW)(SEXP x);

/**
 * @param x Pointer to a numeric vector.
 *
 * @return Pointer to the first element of \a x .
 */
double *(REAL)(SEXP x);

/**
 * @param x Pointer to a complex vector.
 *
 * @return Pointer to the first element of \a x .
 */
Rcomplex *(COMPLEX)(SEXP x);

/**
 * @brief Examine element of a character vector.
 *
 * @param x Pointer to a character vector.
 *
 * @param i Index of the required element.  There is no bounds
 *          checking.
 *
 * @return Pointer to extracted \a i 'th element.
 */
SEXP (STRING_ELT)(SEXP x, R_xlen_t i);

/** @brief Examine element of a list.
 *
 * @param x Pointer to a list.
 *
 * @param i Index of the required element.  There is no bounds checking.
 *
 * @return The value of the \a i 'th element.
 */
SEXP (VECTOR_ELT)(SEXP x, R_xlen_t i);

/** @brief Set element of character vector.
 * 
 * @param x Pointer to a character vector .
 *
 * @param i Index of the required element.  There is no bounds checking.
 *
 * @param v Pointer to CHARSXP representing the new value.
 */
void SET_STRING_ELT(SEXP x, R_xlen_t i, SEXP v);

/** @brief Set element of list.
 *
 * @param x Pointer to a list.
 *
 * @param i Index of the required element.
 *
 * @param v Pointer to R value representing the new value.
 *
 * @return The new value \a v.
 */
SEXP SET_VECTOR_ELT(SEXP x, R_xlen_t i, SEXP v);
SEXP *(STRING_PTR)(SEXP x);
SEXP * NORET (VECTOR_PTR)(SEXP x);

#ifdef LONG_VECTOR_SUPPORT
    R_len_t NORET R_BadLongVector(SEXP, const char *, int);
#endif

/* List Access Functions */
/* These also work for ... objects */
#define CONS(a, b)	cons((a), (b))		/* data lists */
#define LCONS(a, b)	lcons((a), (b))		/* language lists */

/** @brief Get tag of pairlist element.
 *
 * @param e Pointer to a pairlist element, or R_NilValue.
 * @return Pointer to the tag of the list element, or 0 if \a e is
 * R_NilValue.
 */
SEXP (TAG)(SEXP e);

/** @brief Get car of pairlist element.
 *
 * @param e Pointer to a pairlist element, or R_NilValue.
 * @return Pointer to the value of the list car, or 0 if \a e is
 * R_NilValue.
 */

SEXP (CAR)(SEXP e);
/** @brief Get tail of pairlist element.
 *
 * @param e Pointer to a pairlist element, or R_NilValue.
 * @return Pointer to the tail of the list, or 0 if \a e is
 * R_NilValue.
 */
SEXP (CDR)(SEXP e);

/**
 * @brief Equivalent to CAR(CAR(e)).
 */
SEXP (CAAR)(SEXP e);

/**
 * @brief Equivalent to CDR(CAR(e)).
 */
SEXP (CDAR)(SEXP e);

/**
 * @brief Equivalent to CAR(CDR(e)).
 */
SEXP (CADR)(SEXP e);

/**
 * @brief Equivalent to CDR(CDR(e)).
 */
SEXP (CDDR)(SEXP e);

/**
 * @brief Equivalent to CDR(CDR(CDR(e))).
 */
SEXP (CDDDR)(SEXP e);

/**
 * @brief Equivalent to CAR(CDR(CDR(e))).
 */
SEXP (CADDR)(SEXP e);

/**
 * @brief Equivalent to CAR(CDR(CDR(CDR(e)))).
 */
SEXP (CADDDR)(SEXP e);

/**
 * @brief Equivalent to CAR(CDR(CDR(CDR(CDR(e))))).
 */
SEXP (CAD4R)(SEXP e);
int  (MISSING)(SEXP x);
void (SET_MISSING)(SEXP x, int v);

/**
 * @brief Set the tag of a pairlist element.
 *
 * @param x Pointer to a pairlist element.
 * @param y Pointer to an R value representing the new tag of
 *          the pairlist element.
 */
void SET_TAG(SEXP x, SEXP y);

/**
 * @brief Set the 'car' value of a pairlist element.
 * @param x Pointer to a pairlist element.
 * @param y Pointer to an R value representing the new value of the
 *          list car.
 *
 * @returns \a y.
 */
SEXP SETCAR(SEXP x, SEXP y);

/**
 * @brief Replace the tail of a pairlist element.
 * @param x Pointer to a pairlist element.
 * @param y Pointer to an R value representing the new tail of the list.
 *
 * @returns \a y.
 */
SEXP SETCDR(SEXP x, SEXP y);

/**
 * @brief Set the 'car' value of the second element of list.
 * @param x Pointer to a pairlist element with at least one successor
 *         .
 * @param y Pointer to an R value representing the new value of the
 *          second element of the list.
 *
 * @returns \a y.
 */
SEXP SETCADR(SEXP x, SEXP y);

/**
 * @brief Set the 'car' value of the third element of list.
 * @param x Pointer to a pairlist element with at least two
 *          successors.
 * @param y Pointer to an R value representing the new value of the
 *          third element of the list.
 *
 * @returns \a y.
 */
SEXP SETCADDR(SEXP x, SEXP y);

/**
 * @brief Set the 'car' value of the fourth element of list.
 * @param x Pointer to a pairlist element with at least three
 *          successors.
 * @param y Pointer to an R value representing the new value of the
 *          fourth element of the list.
 *
 * @returns \a y.
 */
SEXP SETCADDDR(SEXP x, SEXP y);

/**
 * @brief Set the 'car' value of the fifth element of list.
 * @param x Pointer to a pairlist element with at least four
 *          successors.
 * @param y Pointer to an R value representing the new value of the
 *          fifth element of the list.
 *
 * @returns \a y.
 */
SEXP SETCAD4R(SEXP e, SEXP y);

SEXP CONS_NR(SEXP a, SEXP b);

/* Closure Access Functions */

/** @brief Access formal arguments of a closure.
 *
 * @param x Pointer to a closure object.
 *
 * @return Pointer to the formal argument list of \a x.
 */
SEXP (FORMALS)(SEXP x);

/** @brief Access the body of a closure.
 *
 * @param x Pointer to a closure object.
 *
 * @return Pointer to the body of \a x.
 */
SEXP (BODY)(SEXP x);

/** @brief Access the environment of a closure.
 *
 * @param x Pointer to a closure object.
 *
 * @return Pointer to the environment of x.
 */
SEXP (CLOENV)(SEXP x);

/** @brief Query debugging status.
 *
 * @param x Pointer to a closure object.
 *
 * @return \c true if debugging is set, i.e. evaluations of the
 *         function should run under the browser.
 */
int  (RDEBUG)(SEXP x);
int  (RSTEP)(SEXP x);

/** @brief Get function tracing status.
 *
 * @param x Pointer to a function, or R_NilValue.
 *
 * @return Refer to 'R Internals' document.  Returns 0 if \a x is R_NilValue.
 */
int  (RTRACE)(SEXP x);

/**
 * Set the debugging state of a closure object.
 *
 * @param x Pointer to a closure object.
 *
 * @param v The new debugging state.
 */
void (SET_RDEBUG)(SEXP x, int v);
void (SET_RSTEP)(SEXP x, int v);

/** @brief Set function tracing status.
 *
 * @param x Pointer to a function, or R_NilValue.
 *
 * @param v The desired tracing status: non-zero if tracing is
 * required.
 */
void (SET_RTRACE)(SEXP x, int v);
void SET_FORMALS(SEXP x, SEXP v);
void SET_BODY(SEXP x, SEXP v);

/** @brief Replace the environment of a closure.
 *
 * @param x Pointer to a closure object.
 *
 * @param v Pointer to the environment now to be
 *          considered as the environment of this closure.
 *          R_NilValue is not permissible.
 */
void SET_CLOENV(SEXP x, SEXP v);

/* Symbol Access Functions */

/** @brief Symbol name.
 *
 * @param x Pointer to a symbol.
 *
 * @return Pointer to a CHARSXP representing \a x's name.
 */
SEXP (PRINTNAME)(SEXP x);

/** @brief Symbol's value in the base environment.
 *
 * @param x Pointer to a symbol.
 *
 * @return Pointer to an R value representings \a x's value.
 *         Returns R_UnboundValue if no value is currently
 *         associated with the Symbol.
 */
SEXP (SYMVALUE)(SEXP x);

/** @brief Get function accessed via <tt>.Internal()</tt>.
 *
 * @param x Pointer to a symbol.
 *
 * @return If \a x is associated with a function invoked in R \e
 * via <tt>.Internal()</tt>, then a pointer to the appropriate
 * function, otherwise R_NilValue.
 */
SEXP (INTERNAL)(SEXP x);

/** @brief Does symbol relate to a <tt>...</tt> expression?
 *
 * @param x Pointer to a symbol.
 *
 * @return \c TRUE iff this symbol denotes an element of a
 *         <tt>...</tt> expression.
 */
int  (DDVAL)(SEXP x);
void (SET_DDVAL)(SEXP x, int v);
void SET_PRINTNAME(SEXP x, SEXP v);

/** @brief Set symbol's value in the base environment.
 *
 * @param x Pointer to a symbol.
 *
 * @param val Pointer to the R value now to be considered as
 *            the value of this symbol.  R_NilValue or
 *            R_UnboundValue are permissible values of \a val.
 *
 * @todo No binding to R_UnboundValue ought to be created.
 */
void SET_SYMVALUE(SEXP x, SEXP v);

/** @brief Associate a Symbol with a <tt>.Internal()</tt> function.
 *
 * @param x Pointer to a symbol.
 *
 * @param v Pointer to the builtin function to be
 * associated by this symbol.  R_NilValue is permissible, and
 * signifies that any previous association of \a sym with a
 * function is to be removed from the table.
 */
void SET_INTERNAL(SEXP x, SEXP v);

/* Environment Access Functions */

/** @brief Access an environment's Frame, represented as a pairlist.
 *
 * @param x Pointer to an environment.
 *
 * @return Pointer to a pairlist representing the contents of the
 * Frame of \a x (may be R_NilValue).
 */
SEXP (FRAME)(SEXP x);

/** @brief Access enclosing environment.
 *
 * @param x Pointer to an environment.
 *
 * @return Pointer to the enclosing environment of \a x .
 */
SEXP (ENCLOS)(SEXP x);
SEXP (HASHTAB)(SEXP x);
int  (ENVFLAGS)(SEXP x);
void (SET_ENVFLAGS)(SEXP x, int v);
void SET_FRAME(SEXP x, SEXP v);
void SET_ENCLOS(SEXP x, SEXP v);
void SET_HASHTAB(SEXP x, SEXP v);

/* Promise Access Functions */
/* First five have macro versions in Defn.h */

/** @brief Access the expression of a promise.
 *
 * @param x Pointer to a promise.
 *
 * @return Pointer to the expression to be evaluated by the
 *         promise. 
 */
SEXP (PRCODE)(SEXP x);

/** @brief Access the environment of a promise.
 *
 * @param x Pointer to a promise.
 *
 * @return Pointer to the environment in which the promise
 *         is to be  evaluated.  Set to R_NilValue when the
 *         promise has been evaluated.
 */
SEXP (PRENV)(SEXP x);

/** @brief Access the value of a promise.
 *
 * @param x Pointer to a promise.
 *
 * @return Pointer to the value of the promise, or to
 *         R_UnboundValue if it has not yet been evaluated..
 */
SEXP (PRVALUE)(SEXP x);
int  (PRSEEN)(SEXP x);
void (SET_PRSEEN)(SEXP x, int v);
void SET_PRENV(SEXP x, SEXP v);

/** @brief Set the value of a promise.
 *
 * Once the value is set to something other than R_UnboundValue,
 * the environment pointer is set to R_NilValue.
 *
 * @param x Pointer to a promise.
 *
 * @param v Pointer to the value to be assigned to the promise.
 *
 * @todo Replace this with a method call to evaluate the promise.
 */
void SET_PRVALUE(SEXP x, SEXP v);
void SET_PRCODE(SEXP x, SEXP v);
void SET_PRSEEN(SEXP x, int v);

/* Hashing Functions */
/* There are macro versions in Defn.h */
int  (HASHASH)(SEXP x);
int  (HASHVALUE)(SEXP x);
void (SET_HASHASH)(SEXP x, int v);
void (SET_HASHVALUE)(SEXP x, int v);


/* External pointer access macros */
#define EXTPTR_PTR(x)	CAR(x)
#define EXTPTR_PROT(x)	CDR(x)
#define EXTPTR_TAG(x)	TAG(x)

/* Bytecode access macros */
#define BCODE_CODE(x)	CAR(x)
#define BCODE_CONSTS(x) CDR(x)
#define BCODE_EXPR(x)	TAG(x)
#define isByteCode(x)	(TYPEOF(x)==BCODESXP)

/* Pointer Protection and Unprotection */
#define PROTECT(s)	Rf_protect(s)
#define UNPROTECT(n)	Rf_unprotect(n)
#define UNPROTECT_PTR(s)	Rf_unprotect_ptr(s)

/* We sometimes need to coerce a protected value and place the new
   coerced value under protection.  For these cases PROTECT_WITH_INDEX
   saves an index of the protection location that can be used to
   replace the protected value using REPROTECT. */
typedef int PROTECT_INDEX;
#define PROTECT_WITH_INDEX(x,i) R_ProtectWithIndex(x,i)
#define REPROTECT(x,i) R_Reprotect(x,i)

/* Evaluation Environment */
LibExtern SEXP	R_GlobalEnv;	    /* The "global" environment */

LibExtern SEXP  R_EmptyEnv;	    /* An empty environment at the root of the
				    	environment tree */
LibExtern SEXP  R_BaseEnv;	    /* The base environment; formerly R_NilValue */
LibExtern SEXP	R_BaseNamespace;    /* The (fake) namespace for base */
LibExtern SEXP	R_NamespaceRegistry;/* Registry for registered namespaces */

LibExtern SEXP	R_Srcref;           /* Current srcref, for debuggers */

/* Special Values */
LibExtern SEXP	R_NilValue;	    /* The nil object */
LibExtern SEXP	R_UnboundValue;	    /* Unbound marker */
LibExtern SEXP	R_MissingArg;	    /* Missing argument marker */
#ifdef __MAIN__
attribute_hidden
#else
extern
#endif
SEXP	R_RestartToken;     /* Marker for restarted function calls */

/* Symbol Table Shortcuts */
LibExtern SEXP	R_baseSymbol; // <-- backcompatible version of:
LibExtern SEXP	R_BaseSymbol;	// "base"
LibExtern SEXP	R_BraceSymbol;	    /* "{" */
LibExtern SEXP	R_Bracket2Symbol;   /* "[[" */
LibExtern SEXP	R_BracketSymbol;    /* "[" */
LibExtern SEXP	R_ClassSymbol;	    /* "class" */
LibExtern SEXP	R_DeviceSymbol;	    /* ".Device" */
LibExtern SEXP	R_DimNamesSymbol;   /* "dimnames" */
LibExtern SEXP	R_DimSymbol;	    /* "dim" */
LibExtern SEXP	R_DollarSymbol;	    /* "$" */
LibExtern SEXP	R_DotsSymbol;	    /* "..." */
LibExtern SEXP	R_DoubleColonSymbol;// "::"
LibExtern SEXP	R_DropSymbol;	    /* "drop" */
LibExtern SEXP	R_LastvalueSymbol;  /* ".Last.value" */
LibExtern SEXP	R_LevelsSymbol;	    /* "levels" */
LibExtern SEXP	R_ModeSymbol;	    /* "mode" */
LibExtern SEXP	R_NaRmSymbol;	    /* "na.rm" */
LibExtern SEXP	R_NameSymbol;	    /* "name" */
LibExtern SEXP	R_NamesSymbol;	    /* "names" */
LibExtern SEXP	R_NamespaceEnvSymbol;// ".__NAMESPACE__."
LibExtern SEXP	R_PackageSymbol;    /* "package" */
LibExtern SEXP	R_PreviousSymbol;   /* "previous" */
LibExtern SEXP	R_QuoteSymbol;	    /* "quote" */
LibExtern SEXP	R_RowNamesSymbol;   /* "row.names" */
LibExtern SEXP	R_SeedsSymbol;	    /* ".Random.seed" */
LibExtern SEXP	R_SortListSymbol;   /* "sort.list" */
LibExtern SEXP	R_SourceSymbol;	    /* "source" */
LibExtern SEXP	R_SpecSymbol;	// "spec"
LibExtern SEXP	R_TripleColonSymbol;// ":::"
LibExtern SEXP	R_TspSymbol;	    /* "tsp" */

LibExtern SEXP  R_dot_defined;      /* ".defined" */
LibExtern SEXP  R_dot_Method;       /* ".Method" */
LibExtern SEXP	R_dot_packageName;// ".packageName"
LibExtern SEXP  R_dot_target;       /* ".target" */
LibExtern SEXP  R_dot_Generic;      /* ".Generic" */

/* Missing Values - others from Arith.h */
#define NA_STRING	R_NaString
LibExtern SEXP	R_NaString;	    /* NA_STRING as a CHARSXP */
LibExtern SEXP	R_BlankString;	    /* "" as a CHARSXP */
LibExtern SEXP	R_BlankScalarString;	    /* "" as a STRSXP */

/* srcref related functions */
SEXP R_GetCurrentSrcref(int skip);
SEXP R_GetSrcFilename(SEXP srcref);

/*--- FUNCTIONS ------------------------------------------------------ */

/* Type Coercions of all kinds */

SEXP Rf_asChar(SEXP x);
SEXP Rf_coerceVector(SEXP v, SEXPTYPE type);
SEXP Rf_PairToVectorList(SEXP x);
SEXP Rf_VectorToPairList(SEXP x);
SEXP Rf_asCharacterFactor(SEXP x);
int Rf_asLogical(SEXP x);
int Rf_asInteger(SEXP x);
double Rf_asReal(SEXP x);
Rcomplex Rf_asComplex(SEXP x);


#ifndef R_ALLOCATOR_TYPE
#define R_ALLOCATOR_TYPE
typedef struct R_allocator R_allocator_t;
#endif

/* Other Internally Used Functions, excluding those which are inline-able*/

char * Rf_acopy_string(const char * in);
void Rf_addMissingVarsToNewEnv(SEXP, SEXP);
SEXP Rf_alloc3DArray(SEXPTYPE mode, int nrow, int ncol, int nface);
SEXP Rf_allocArray(SEXPTYPE mode, SEXP dims);
SEXP Rf_allocFormalsList2(SEXP sym1, SEXP sym2);
SEXP Rf_allocFormalsList3(SEXP sym1, SEXP sym2, SEXP sym3);
SEXP Rf_allocFormalsList4(SEXP sym1, SEXP sym2, SEXP sym3, SEXP sym4);
SEXP Rf_allocFormalsList5(SEXP sym1, SEXP sym2, SEXP sym3, SEXP sym4, SEXP sym5);
SEXP Rf_allocFormalsList6(SEXP sym1, SEXP sym2, SEXP sym3, SEXP sym4, SEXP sym5, SEXP sym6);
SEXP Rf_allocMatrix(SEXPTYPE mode, int nrow, int ncol);

/** @brief Create a pairlist of a specified length.
 *
 * This constructor creates a pairlist with a specified
 * number of elements.  On creation, each element has 'car' and 'tag'
 * set to R_NilValue.
 *
 * @param n Number of elements required in the list.
 *
 * @return The constructed list, or R_NilValue if \a n is zero.
 */
SEXP Rf_allocList(int n);

/** @brief Create an S4 object.
 *
 * @return Pointer to the created object.
 */
SEXP Rf_allocS4Object(void);

/** @brief Create an object of a type derived from pairlist element.
 *
 * The object is created with car, tag and tail pointers all set to R_NilValue.
 *
 * @param t The SEXPTYPE of the required object. Must be one of
 *          LISTSXP, LANGSXP, DOTSXP or BCODESXP.
 *
 * @return Pointer to the created object.
 */
SEXP Rf_allocSExp(SEXPTYPE t);
SEXP Rf_allocVector3(SEXPTYPE type, R_xlen_t length, R_allocator_t* allocator);
R_xlen_t Rf_any_duplicated(SEXP x, Rboolean from_last);
R_xlen_t Rf_any_duplicated3(SEXP x, SEXP incomp, Rboolean from_last);
SEXP Rf_applyClosure(SEXP call, SEXP op, SEXP arglist, SEXP rho, SEXP suppliedvars);
SEXP Rf_arraySubscript(int dim, SEXP s, SEXP dims, SEXP (*)(SEXP,SEXP dng),
                       SEXP (*)(SEXP, int strg), SEXP x);
SEXP Rf_classgets(SEXP vec, SEXP klass);

/** @brief Creates a pairlist with a specified car and tail.
 *
 * This function protects its arguments from the garbage collector.
 *
 * @param cr Pointer to the 'car' of the element to be created.
 *
 * @param tl Pointer to the 'tail' of the element to be created,
 *          which must be a pairlist or R_NilValue.
 *
 * @return Pointer to the constructed pairlist.
 */
SEXP Rf_cons(SEXP car, SEXP cdr);
void Rf_copyMatrix(SEXP s, SEXP t, Rboolean byrow);
void Rf_copyListMatrix(SEXP s, SEXP t, Rboolean byrow);

/** @brief Copy attributes, with some exceptions.
 *
 * This is called in the case of binary operations to copy most
 * attributes from one of the input arguments to the output.
 * Note that the Dim, Dimnames and Names attributes are not
 * copied: these should have been assigned elsewhere.  The
 * function also copies the S4 object status.
 *
 * @param inp Pointer to the R value from which attributes are to
 *          be copied.
 *
 * @param ans Pointer to the R value to which attributes are to be
 *          copied.
 *
 * @note The above documentation is probably incomplete: refer to the
 *       source code for further details.
 */
void Rf_copyMostAttrib(SEXP inp, SEXP ans);
void Rf_copyVector(SEXP s, SEXP t);
int Rf_countContexts(int ctxttype, int browser);
SEXP Rf_CreateTag(SEXP x);
void Rf_defineVar(SEXP symbol, SEXP value, SEXP rho);
SEXP Rf_dimgets(SEXP vec, SEXP val);
SEXP Rf_dimnamesgets(SEXP vec, SEXP val);
SEXP Rf_DropDims(SEXP x);
SEXP Rf_duplicate(SEXP s);
SEXP Rf_shallow_duplicate(SEXP s);
SEXP Rf_lazy_duplicate(SEXP s);
/* the next really should not be here and is also in Defn.h */
SEXP Rf_duplicated(SEXP x, Rboolean from_last);
Rboolean R_envHasNoSpecialSymbols(SEXP env);

/** @brief Evaluate an object in a specified Environment.
 *
 * @param e Pointer to the object to be evaluated.
 *
 * @param rho Pointer to an Environment
 *
 * @return Pointer to the result of evaluating \a e in \a rho.
 */
SEXP Rf_eval(SEXP e, SEXP rho);
SEXP Rf_findFun(SEXP symbol, SEXP rho);
void Rf_findFunctionForBody(SEXP body);

/** @brief Find the binding of a symbol in an environment and its enclosing environments.
 *
 * This function calls \ref Rf_findVarInFrame3 on the given frame and its enclosing 
 * environments, until a binding is found or the empty environment is reached.
 *
 * @param symbol the \ref SYMSXP that should be looked up
 * @param rho an \ref ENVSXP that is the starting point for the lookup
 *
 * @return Returns the binding value, or \ref R_UnboundValue if none was found.
 *
 * @gc if one of the environments is of class "UserDefinedDatabase" or active bindings 
 * (\ref R_MakeActiveBinding) are used.
 */
SEXP Rf_findVar(SEXP symbol, SEXP rho);
SEXP Rf_findVarInFrame(SEXP rho, SEXP symbol);

/** @brief Find the binding for a symbol in a single environment.
 *
 * The lookup will query user defined databases if the environment is of class
 * "UserDefinedDatabase" (using \ref R_ObjectTable), and query active binding 
 * (\ref R_MakeActiveBinding).
 *
 * @param rho an \ref ENVSXP in which the lookup will take place
 * @param symbol the \ref SYMSXP that should be looked up
 * @param doGet specifies if the lookup is done to get the value (TRUE), as opposed to 
 *     only determining whether there is a binding. The only effect is that, if this is 
 *     FALSE, \ref R_ObjectTable::exists is called before calling \ref R_ObjectTable::get.
 *
 * @return Returns the binding value, or \ref R_UnboundValue if none was found.
 *
 * @gc if the environments is of class "UserDefinedDatabase" or active bindings 
 * (\ref R_MakeActiveBinding) are used.
 */
SEXP Rf_findVarInFrame3(SEXP rho, SEXP symbol, Rboolean doGet);

/** @brief Access a named attribute.
 *
 * @param vec Pointer to the R value whose attributes are to be
 *          accessed.
 *
 * @param name Either a pointer to the symbol representing the
 *          required attribute, or a pointer to a character vector
 *          containing the required symbol name as the first element; in
 *          the latter case, as a side effect, the corresponding
 *          symbol is installed if necessary.
 *
 * @return Pointer to the requested attribute, or R_NilValue
 *         if there is no such attribute.
 *
 * @note The above documentation is incomplete: refer to the
 *       source code for further details.
 */
SEXP Rf_getAttrib(SEXP vec, SEXP name);
SEXP Rf_GetArrayDimnames(SEXP x);
SEXP Rf_GetColNames(SEXP dimnames);
void Rf_GetMatrixDimnames(SEXP x, SEXP* rl, SEXP* cl, const char** rn, const char** cn);
SEXP Rf_GetOption(SEXP tag, SEXP rho); /* pre-2.13.0 compatibility */
SEXP Rf_GetOption1(SEXP tag);
int Rf_GetOptionDigits(void);
int Rf_GetOptionWidth(void);
SEXP Rf_GetRowNames(SEXP dimnames);
void Rf_gsetVar(SEXP symbol, SEXP value, SEXP rho);

/** @brief Get a pointer to a regular Symbol object.
 *
 * If no Symbol with the specified name currently exists, one will
 * be created, and a pointer to it returned.  Otherwise a pointer
 * to the existing Symbol will be returned.
 *
 * @param name The name of the required Symbol (CE_NATIVE encoding
 *          is assumed).
 *
 * @return Pointer to a Symbol (preexisting or newly created) with
 * the required name.
 */
SEXP Rf_install(const char * name);
SEXP Rf_installChar(SEXP charSXP);
SEXP Rf_installDDVAL(int i);
SEXP Rf_installS3Signature(const char *, const char *);
Rboolean Rf_isFree(SEXP val);
Rboolean Rf_isOrdered(SEXP s);
Rboolean Rf_isUnmodifiedSpecSym(SEXP sym, SEXP env);
Rboolean Rf_isUnordered(SEXP s);
Rboolean Rf_isUnsorted(SEXP x, Rboolean strictly);
SEXP Rf_lengthgets(SEXP x, R_len_t len);
SEXP Rf_xlengthgets(SEXP x, R_xlen_t len);
SEXP R_lsInternal(SEXP env, Rboolean all);
SEXP R_lsInternal3(SEXP env, Rboolean all, Rboolean sorted);
SEXP Rf_match(SEXP itable, SEXP ix, int nmatch);
SEXP Rf_matchE(SEXP itable, SEXP ix, int nmatch, SEXP env);
SEXP Rf_namesgets(SEXP vec, SEXP val);

/** @brief Get a pointer to a CHARSXP object.
 *
 * CE_NATIVE encoding is assumed.  If no CHARSXP with the
 * specified text and encoding currently exists, one will be
 * created.  Otherwise a pointer to the existing CHARSXP will
 * be returned.
 *
 * @param str The null-terminated text of the required string.
 *
 * @return Pointer to a string object representing the specified
 *         text.
 */
SEXP Rf_mkChar(const char * name);

/** @brief Create a CHARSXP object for specified text.
 *
 * CE_NATIVE encoding is assumed.  If no CHARSXP with the
 * specified text and encoding currently exists, one will be
 * created.  Otherwise a pointer to the existing CHARSXP will
 * be returned.
 *
 * @param text The text of the string to be created, possibly
 *          including embedded null characters.  The encoding is
 *          assumed to be CE_NATIVE.
 *
 * @param length The length of the string pointed to by \a text.
 *          Must be nonnegative.  The created string will comprise
 *          the text plus an appended null byte.
 *
 * @return Pointer to the created string.
 */
SEXP Rf_mkCharLen(const char * name, int len);
Rboolean Rf_NonNullStringMatch(SEXP s, SEXP t);
int Rf_ncols(SEXP s);
int Rf_nrows(SEXP s);
SEXP Rf_nthcdr(SEXP s, int n);

// ../main/character.c :
typedef enum {Bytes, Chars, Width} nchar_type;
int R_nchar(SEXP string, nchar_type type_,
	    Rboolean allowNA, Rboolean keepNA, const char* msg_name);

Rboolean Rf_pmatch(SEXP formal, SEXP tag, Rboolean exact);
Rboolean Rf_psmatch(const char * f, const char * t, Rboolean exact);
void Rf_PrintValue(SEXP s);
#ifndef INLINE_PROTECT

/** @brief Push a node pointer onto the C pointer protection stack.
 *
 * @param node Pointer to the node to be protected from the
 *          garbage collector.
 * @return a copy of \a node .
 */
SEXP Rf_protect(SEXP);
#endif
void Rf_readS3VarsFromFrame(SEXP, SEXP*, SEXP*, SEXP*, SEXP*, SEXP*, SEXP*);

/** @brief Set or remove a named attribute.
 *
 * @param vec Pointer to the value whose attributes are to be
 *          modified.
 *
 * @param name Either a pointer to the symbol representing the
 *          required attribute, or a pointer to a character vector
 *          containing the required symbol name as the first element; in
 *          the latter case, as a side effect, the corresponding
 *          symbol is installed if necessary.
 *
 * @param val Either the value to which the attribute is to be
 *          set, or R_NilValue.  In the latter case the
 *          attribute (if present) is removed.
 *
 * @return Refer to source code.  (Sometimes \a vec, sometimes \a
 * val, sometime R_NilValue ...)
 *
 * @note The above documentation is probably incomplete: refer to the
 *       source code for further details.
 */
SEXP Rf_setAttrib(SEXP vec, SEXP name, SEXP val);
void Rf_setSVector(SEXP* vec, int len, SEXP val);
void Rf_setVar(SEXP symbol, SEXP value, SEXP rho);
SEXP Rf_stringSuffix(SEXP string, int fromIndex);
SEXPTYPE Rf_str2type(const char * s);
Rboolean Rf_StringBlank(SEXP x);
SEXP Rf_substitute(SEXP lang,SEXP rho);
const char * Rf_translateChar(SEXP x);
const char * Rf_translateChar0(SEXP x);

/** @brief Convert contents of a CHARSXP to UTF8.
 *
 * @param x Pointer to a CHARSXP.
 *
 * @return The text of \a x rendered in UTF8 encoding.
 *
 * @note The result is held in memory allocated using R_alloc().
 * The calling code must arrange for this memory to be released in
 * due course.
 */
const char * Rf_translateCharUTF8(SEXP x);

/** @brief Name of type within R.
 *
 * Translate a SEXPTYPE to the name by which it is known within R.
 *
 * @param st The SEXPTYPE whose name is required.
 *
 * @return The SEXPTYPE's name within R.
 */
const char * Rf_type2char(SEXPTYPE t);
SEXP Rf_type2rstr(SEXPTYPE t);
SEXP Rf_type2str(SEXPTYPE t);
SEXP Rf_type2str_nowarn(SEXPTYPE t);
#ifndef INLINE_PROTECT

/** @brief Pop cells from the C pointer protection stack.
 *
 * As a consistency check, it is required that the unprotect takes
 * place within the same ProtectStack::Scope as the corresponding
 * protects.
 *
 * @param count Number of cells to be popped.  Must not be
 *          larger than the current size of the C pointer
 *          protection stack.
 */
void Rf_unprotect(int);
#endif

/** @brief Remove entry from pointer protection stack.
 *
 * Removes from the C pointer protection stack the uppermost stack
 * cell containing a pointer to a specified node, and drops all
 * the stack cells above it by one place.
 *
 * @param node Pointer to the node whose cell is to be removed
 *          from the C pointer protection stack.
 */
void Rf_unprotect_ptr(SEXP s);

void NORET R_signal_protect_error(void);
void NORET R_signal_unprotect_error(void);
void NORET R_signal_reprotect_error(PROTECT_INDEX i);

#ifndef INLINE_PROTECT

/** @brief Push a node pointer onto the C pointer protection stack.
 *
 * Push a node pointer onto the C pointer protection stack, and
 * record the index of the resulting stack cell (for subsequent
 * use with R_Reprotect).
 *
 * @param node Pointer to the node to be protected from the
 *          garbage collector.
 *
 * @param iptr Pointer to a location in which the stack cell index
 *          is to be stored.
 */
void R_ProtectWithIndex(SEXP, PROTECT_INDEX *);

/** @brief Retarget a cell in the C pointer protection stack.
 *
 * Change the node that a particular cell in the C pointer
 * protection stack protects.  As a consistency check, it is
 * required that the reprotect takes place within the same
 * ProtectStack::Scope as the original protect.
 *
 * @param node Pointer to the node now to be protected from
 *          the garbage collector by the designated stack
 *          cell.  (Not necessarily a different node from the
 *          one currently protected.)
 *
 * @param index Index (as returned by R_ProtectWithIndex() ) of
 *          the stack cell to be retargeted to node.  Must be less
 *          than the current size of the C pointer protection
 *          stack.
 */
void R_Reprotect(SEXP, PROTECT_INDEX);
#endif
SEXP R_tryEval(SEXP e, SEXP env, int * ErrorOccurred);
SEXP R_tryEvalSilent(SEXP e, SEXP env, int * ErrorOccurred);
const char *R_curErrorBuf();

Rboolean Rf_isS4(SEXP s);
SEXP Rf_asS4(SEXP s, Rboolean flag, int complete);
SEXP Rf_S3Class(SEXP obj);
int Rf_isBasicClass(const char * ss);

Rboolean R_cycle_detected(SEXP s, SEXP child);

typedef enum {
    CE_NATIVE = 0,
    CE_UTF8   = 1,
    CE_LATIN1 = 2,
    CE_BYTES  = 3,
    CE_SYMBOL = 5,
    CE_ANY    =99
} cetype_t;

cetype_t Rf_getCharCE(SEXP x);

/** @brief Get a pointer to a CHARSXP object.
 *
 * If no CHARSXP with the specified text and encoding
 * currently exists, one will be created.  Otherwise a pointer to
 * the existing CHARSXP will be returned.
 *
 * @param str The null-terminated text of the required cached string.
 *
 * @param encoding The encoding of the required String.
 *          Only CE_NATIVE, CE_UTF8 or CE_LATIN1 are permitted in
 *          this context.
 *
 * @return Pointer to a string object representing the specified
 *         text in the specified encoding.
 */
SEXP Rf_mkCharCE(const char * name, cetype_t enc);

/** @brief Create a CHARSXP object for specified text and
 * encoding.
 *
 * If no CHARSXP with the specified text and encoding
 * currently exists, one will be created.  Otherwise a pointer to
 * the existing CHARSXP will be returned.
 *
 * @param text The text of the string to be created, possibly
 *          including embedded null characters.  The encoding is
 *          assumed to be CE_NATIVE.
 *
 * @param length The length of the string pointed to by \a text.
 *          Must be nonnegative.  The created string will comprise
 *          the text plus an appended null byte.
 *
 * @param encoding The encoding of the required String.
 *          Only CE_NATIVE, CE_UTF8 or CE_LATIN1 are permitted in
 *          this context.
 *
 * @return Pointer to the created string.
 */
SEXP Rf_mkCharLenCE(const char * name, int len, cetype_t enc);
const char *Rf_reEnc(const char *x, cetype_t ce_in, cetype_t ce_out, int subst);

				/* return(.) NOT reached : for -Wall */
#define error_return(msg)	{ Rf_error(msg);	   return R_NilValue; }
#define errorcall_return(cl,msg){ Rf_errorcall(cl, msg);   return R_NilValue; }

#ifdef __MAIN__
#undef extern
#undef LibExtern
#endif

/* Calling a function with arguments evaluated */
SEXP R_forceAndCall(SEXP e, int n, SEXP rho);

/* External pointer interface */

/** @brief Create an external pointer object.
 *
 * @param p The pointer that the external pointer object is
 *          to encapsulate.  May be NULL.
 * @param tag Pointer to the tag object.  May be R_NilValue (and
 *          often is).
 * @param prot Pointer to the protege object.  May be R_NilValue
 *          (and often is).
 *
 * @return Pointer to the created external pointer object.
 */
SEXP R_MakeExternalPtr(void *p, SEXP tag, SEXP prot);

/** @brief Get the encapsulated external pointer.
 *
 * @param s Pointer to an external pointer object.
 *
 * @return the external pointer encapsulated by \a s.
 */
void *R_ExternalPtrAddr(SEXP s);

/** @brief Get pointer to tag object.
 *
 * @param s Pointer to an external pointer object.
 *
 * @return a pointer to the tag object of \a s.
 */
SEXP R_ExternalPtrTag(SEXP s);

/** @brief Get pointer to protege object.
 *
 * @param s Pointer to an external pointer object.
 *
 * @return a pointer to the protege object of \a s.
 */
SEXP R_ExternalPtrProtected(SEXP s);

/** @brief Reset the encapsulated pointer to R_NilValue.
 *
 * @param s Pointer to an external pointer object.
 */
void R_ClearExternalPtr(SEXP s);

/** @brief Set the value of the encapsulated pointer
 *
 * @param s Pointer to an external pointer object.
 *
 * @param p New pointer value (may be NULL).
 */
void R_SetExternalPtrAddr(SEXP s, void *p);

/** @brief Designate the tag object.
 *
 * @param s Pointer to an external pointer object.
 *
 * @param tag Pointer to the new tag object (or R_NilValue).
 */
void R_SetExternalPtrTag(SEXP s, SEXP tag);

/** @brief Designate the protege object.
 *
 * @param s Pointer to an external pointer object.
 *
 * @param p Pointer to the new protege object (or R_NilValue).
 */
void R_SetExternalPtrProtected(SEXP s, SEXP p);

/* Finalization interface */
typedef void (*R_CFinalizer_t)(SEXP);
void R_RegisterFinalizer(SEXP s, SEXP fun);
void R_RegisterCFinalizer(SEXP s, R_CFinalizer_t fun);
void R_RegisterFinalizerEx(SEXP s, SEXP fun, Rboolean onexit);
void R_RegisterCFinalizerEx(SEXP s, R_CFinalizer_t fun, Rboolean onexit);
void R_RunPendingFinalizers(void);

/* Weak reference interface */
SEXP R_MakeWeakRef(SEXP key, SEXP val, SEXP fin, Rboolean onexit);
SEXP R_MakeWeakRefC(SEXP key, SEXP val, R_CFinalizer_t fin, Rboolean onexit);
SEXP R_WeakRefKey(SEXP w);
SEXP R_WeakRefValue(SEXP w);
void R_RunWeakRefFinalizer(SEXP w);

SEXP R_PromiseExpr(SEXP p);
SEXP R_ClosureExpr(SEXP p);
void R_initialize_bcode(void);
SEXP R_bcEncode(SEXP bytes);
SEXP R_bcDecode(SEXP code);
void R_registerBC(SEXP bcBytes, SEXP bcode);
Rboolean R_checkConstants(Rboolean abortOnError);
#define PREXPR(e) R_PromiseExpr(e)
#define BODY_EXPR(e) R_ClosureExpr(e)

/* Protected evaluation */
Rboolean R_ToplevelExec(void (*fun)(void *), void *data);
SEXP R_ExecWithCleanup(SEXP (*fun)(void *), void *data,
		       void (*cleanfun)(void *), void *cleandata);

/* Environment and Binding Features */
void R_RestoreHashCount(SEXP rho);
Rboolean R_IsPackageEnv(SEXP rho);
SEXP R_PackageEnvName(SEXP rho);
SEXP R_FindPackageEnv(SEXP info);
Rboolean R_IsNamespaceEnv(SEXP rho);
SEXP R_NamespaceEnvSpec(SEXP rho);
SEXP R_FindNamespace(SEXP info);
void R_LockEnvironment(SEXP env, Rboolean bindings);
Rboolean R_EnvironmentIsLocked(SEXP env);
void R_LockBinding(SEXP sym, SEXP env);
void R_unLockBinding(SEXP sym, SEXP env);
void R_MakeActiveBinding(SEXP sym, SEXP fun, SEXP env);
Rboolean R_BindingIsLocked(SEXP sym, SEXP env);
Rboolean R_BindingIsActive(SEXP sym, SEXP env);
Rboolean R_HasFancyBindings(SEXP rho);


/* ../main/errors.c : */
/* needed for R_load/savehistory handling in front ends */
#if defined(__GNUC__) && __GNUC__ >= 3
void Rf_errorcall(SEXP call, const char * format, ...) __attribute__((noreturn));
#else
void Rf_errorcall(SEXP, const char *, ...);
#endif
void Rf_warningcall(SEXP call, const char * format, ...);
void Rf_warningcall_immediate(SEXP call, const char * format, ...);

/* Save/Load Interface */
#define R_XDR_DOUBLE_SIZE 8
#define R_XDR_INTEGER_SIZE 4

void R_XDREncodeDouble(double d, void *buf);
double R_XDRDecodeDouble(void *buf);
void R_XDREncodeInteger(int i, void *buf);
int R_XDRDecodeInteger(void *buf);

typedef void *R_pstream_data_t;

typedef enum {
    R_pstream_any_format,
    R_pstream_ascii_format,
    R_pstream_binary_format,
    R_pstream_xdr_format,
    R_pstream_asciihex_format
} R_pstream_format_t;

typedef struct R_outpstream_st *R_outpstream_t;
struct R_outpstream_st {
    R_pstream_data_t data;
    R_pstream_format_t type;
    int version;
    void (*OutChar)(R_outpstream_t, int);
    void (*OutBytes)(R_outpstream_t, void *, int);
    SEXP (*OutPersistHookFunc)(SEXP, SEXP);
    SEXP OutPersistHookData;
};

typedef struct R_inpstream_st *R_inpstream_t;
struct R_inpstream_st {
    R_pstream_data_t data;
    R_pstream_format_t type;
    int (*InChar)(R_inpstream_t);
    void (*InBytes)(R_inpstream_t, void *, int);
    SEXP (*InPersistHookFunc)(SEXP, SEXP);
    SEXP InPersistHookData;
};

void R_InitInPStream(R_inpstream_t stream, R_pstream_data_t data,
		     R_pstream_format_t type,
		     int (*inchar)(R_inpstream_t),
		     void (*inbytes)(R_inpstream_t, void *, int),
		     SEXP (*phook)(SEXP, SEXP), SEXP pdata);
void R_InitOutPStream(R_outpstream_t stream, R_pstream_data_t data,
		      R_pstream_format_t type, int version,
		      void (*outchar)(R_outpstream_t, int),
		      void (*outbytes)(R_outpstream_t, void *, int),
		      SEXP (*phook)(SEXP, SEXP), SEXP pdata);

void R_InitFileInPStream(R_inpstream_t stream, FILE *fp,
			 R_pstream_format_t type,
			 SEXP (*phook)(SEXP, SEXP), SEXP pdata);
void R_InitFileOutPStream(R_outpstream_t stream, FILE *fp,
			  R_pstream_format_t type, int version,
			  SEXP (*phook)(SEXP, SEXP), SEXP pdata);

#ifdef NEED_CONNECTION_PSTREAMS
/* The connection interface is not available to packages.  To
   allow limited use of connection pointers this defines the opaque
   pointer type. */
#ifndef HAVE_RCONNECTION_TYPEDEF
typedef struct Rconn  *Rconnection;
#define HAVE_RCONNECTION_TYPEDEF
#endif
void R_InitConnOutPStream(R_outpstream_t stream, Rconnection con,
			  R_pstream_format_t type, int version,
			  SEXP (*phook)(SEXP, SEXP), SEXP pdata);
void R_InitConnInPStream(R_inpstream_t stream,  Rconnection con,
			 R_pstream_format_t type,
			 SEXP (*phook)(SEXP, SEXP), SEXP pdata);
#endif

void R_Serialize(SEXP s, R_outpstream_t ops);
SEXP R_Unserialize(R_inpstream_t ips);

/* slot management (in attrib.c) */
SEXP R_do_slot(SEXP obj, SEXP name);
SEXP R_do_slot_assign(SEXP obj, SEXP name, SEXP value);
int R_has_slot(SEXP obj, SEXP name);
/* S3-S4 class (inheritance), attrib.c */
SEXP R_S4_extends(SEXP klass, SEXP useTable);

/* class definition, new objects (objects.c) */
SEXP R_do_MAKE_CLASS(const char *what);
SEXP R_getClassDef  (const char *what);
SEXP R_getClassDef_R(SEXP what);
Rboolean R_has_methods_attached(void);
Rboolean R_isVirtualClass(SEXP class_def, SEXP env);
Rboolean R_extends  (SEXP class1, SEXP class2, SEXP env);
SEXP R_do_new_object(SEXP class_def);
/* supporting  a C-level version of  is(., .) : */
int R_check_class_and_super(SEXP x, const char **valid, SEXP rho);
int R_check_class_etc      (SEXP x, const char **valid);

/* preserve objects across GCs */

/** @brief Protect object against garbage collection.
 *
 * This is intended for long-term protection, for which PROTECT()
 * etc. would be inappropriate.
 *
 * @param object Pointer to the object to be preserved.  It is
 *          permissible for this to be R_NilValue.
 */
void R_PreserveObject(SEXP object);

/** @brief Remove object's protection against garbage collection.
 *
 * @param object Pointer to the object whose protection is to be
 *          removed.  It is permissible (but pointless) for this
 *          to be a pointer to an object that is not currently
 *          protected by R_PreserveObject(), but in that case
 *          R_ReleaseObject() has no effect.
 */
void R_ReleaseObject(SEXP object);

/* Shutdown actions */
void R_dot_Last(void);		/* in main.c */
void R_RunExitFinalizers(void);	/* in memory.c */

/* Replacements for popen and system */
#ifdef HAVE_POPEN
FILE *R_popen(const char * command, const char * type);
#endif
int R_system(const char * command);

/* R_compute_identical:  C version of identical() function
   The third arg to R_compute_identical() consists of bitmapped flags for non-default options:
   currently the first 4 default to TRUE, so the flag is set for FALSE values:
   1 = !NUM_EQ
   2 = !SINGLE_NA
   4 = !ATTR_AS_SET
   8 = !IGNORE_BYTECODE
  16 = !IGNORE_ENV
  Default from R's default: 16
*/
Rboolean R_compute_identical(SEXP x, SEXP y, int flags);

SEXP R_body_no_src(SEXP x); // body(x) without "srcref" etc, ../main/utils.c

/* C version of R's  indx <- order(..., na.last, decreasing) :
   e.g.  arglist = Rf_lang2(x,y)  or  Rf_lang3(x,y,z) */
void R_orderVector (int *indx, int n, SEXP arglist, Rboolean nalast, Rboolean decreasing);
// C version of R's  indx <- order(x, na.last, decreasing) :
void R_orderVector1(int *indx, int n, SEXP x,       Rboolean nalast, Rboolean decreasing);

#ifndef R_NO_REMAP
#define acopy_string		Rf_acopy_string
#define addMissingVarsToNewEnv	Rf_addMissingVarsToNewEnv
#define alloc3DArray            Rf_alloc3DArray
#define allocArray		Rf_allocArray
#define allocFormalsList2	Rf_allocFormalsList2
#define allocFormalsList3	Rf_allocFormalsList3
#define allocFormalsList4	Rf_allocFormalsList4
#define allocFormalsList5	Rf_allocFormalsList5
#define allocFormalsList6	Rf_allocFormalsList6
#define allocList		Rf_allocList
#define allocMatrix		Rf_allocMatrix
#define allocS4Object		Rf_allocS4Object
#define allocSExp		Rf_allocSExp
#define allocVector		Rf_allocVector
#define allocVector3		Rf_allocVector3
#define any_duplicated		Rf_any_duplicated
#define any_duplicated3		Rf_any_duplicated3
#define applyClosure		Rf_applyClosure
#define arraySubscript		Rf_arraySubscript
#define asChar			Rf_asChar
#define asCharacterFactor	Rf_asCharacterFactor
#define asComplex		Rf_asComplex
#define asInteger		Rf_asInteger
#define asLogical		Rf_asLogical
#define asReal			Rf_asReal
#define asS4			Rf_asS4
#define classgets		Rf_classgets
#define coerceVector		Rf_coerceVector
#define conformable		Rf_conformable
#define cons			Rf_cons
#define copyListMatrix		Rf_copyListMatrix
#define copyMatrix		Rf_copyMatrix
#define copyMostAttrib		Rf_copyMostAttrib
#define copyVector		Rf_copyVector
#define countContexts		Rf_countContexts
#define CreateTag		Rf_CreateTag
#define defineVar		Rf_defineVar
#define dimgets			Rf_dimgets
#define dimnamesgets		Rf_dimnamesgets
#define DropDims                Rf_DropDims
#define duplicate		Rf_duplicate
#define duplicated		Rf_duplicated
#define elt			Rf_elt
#define errorcall		Rf_errorcall
#define eval			Rf_eval
#define findFun			Rf_findFun
#define findFunctionForBody	Rf_findFunctionForBody
#define findVar			Rf_findVar
#define findVarInFrame		Rf_findVarInFrame
#define findVarInFrame3		Rf_findVarInFrame3
#define GetArrayDimnames	Rf_GetArrayDimnames
#define getAttrib		Rf_getAttrib
#define getCharCE		Rf_getCharCE
#define GetColNames		Rf_GetColNames
#define GetMatrixDimnames	Rf_GetMatrixDimnames
#define GetOption1		Rf_GetOption1
#define GetOptionDigits		Rf_GetOptionDigits
#define GetOptionWidth		Rf_GetOptionWidth
#define GetOption		Rf_GetOption
#define GetRowNames		Rf_GetRowNames
#define gsetVar			Rf_gsetVar
#define inherits		Rf_inherits
#define install			Rf_install
#define installChar		Rf_installChar
#define installDDVAL		Rf_installDDVAL
#define installS3Signature	Rf_installS3Signature
#define isArray			Rf_isArray
#define isBasicClass            Rf_isBasicClass
#define isComplex		Rf_isComplex
#define isEnvironment		Rf_isEnvironment
#define isExpression		Rf_isExpression
#define isFactor		Rf_isFactor
#define isFrame			Rf_isFrame
#define isFree			Rf_isFree
#define isFunction		Rf_isFunction
#define isInteger		Rf_isInteger
#define isLanguage		Rf_isLanguage
#define isList			Rf_isList
#define isLogical		Rf_isLogical
#define isSymbol		Rf_isSymbol
#define isMatrix		Rf_isMatrix
#define isNewList		Rf_isNewList
#define isNull			Rf_isNull
#define isNumeric		Rf_isNumeric
#define isNumber		Rf_isNumber
#define isObject		Rf_isObject
#define isOrdered		Rf_isOrdered
#define isPairList		Rf_isPairList
#define isPrimitive		Rf_isPrimitive
#define isReal			Rf_isReal
#define isS4			Rf_isS4
#define isString		Rf_isString
#define isTs			Rf_isTs
#define isUnmodifiedSpecSym	Rf_isUnmodifiedSpecSym
#define isUnordered		Rf_isUnordered
#define isUnsorted		Rf_isUnsorted
#define isUserBinop		Rf_isUserBinop
#define isValidString		Rf_isValidString
#define isValidStringF		Rf_isValidStringF
#define isVector		Rf_isVector
#define isVectorAtomic		Rf_isVectorAtomic
#define isVectorizable		Rf_isVectorizable
#define isVectorList		Rf_isVectorList
#define lang1			Rf_lang1
#define lang2			Rf_lang2
#define lang3			Rf_lang3
#define lang4			Rf_lang4
#define lang5			Rf_lang5
#define lang6			Rf_lang6
#define lastElt			Rf_lastElt
#define lazy_duplicate		Rf_lazy_duplicate
#define lcons			Rf_lcons
#define length(x)		Rf_length(x)
#define lengthgets		Rf_lengthgets
#define list1			Rf_list1
#define list2			Rf_list2
#define list3			Rf_list3
#define list4			Rf_list4
#define list5			Rf_list5
#define list6			Rf_list6
#define listAppend		Rf_listAppend
#define match			Rf_match
#define matchE			Rf_matchE
#define mkChar			Rf_mkChar
#define mkCharCE		Rf_mkCharCE
#define mkCharLen		Rf_mkCharLen
#define mkCharLenCE		Rf_mkCharLenCE
#define mkNamed			Rf_mkNamed
#define mkString		Rf_mkString
#define namesgets		Rf_namesgets
#define ncols			Rf_ncols
#define nlevels			Rf_nlevels
#define NonNullStringMatch	Rf_NonNullStringMatch
#define nrows			Rf_nrows
#define nthcdr			Rf_nthcdr
#define PairToVectorList	Rf_PairToVectorList
#define pmatch			Rf_pmatch
#define psmatch			Rf_psmatch
#define PrintValue		Rf_PrintValue
#define protect			Rf_protect
#define readS3VarsFromFrame	Rf_readS3VarsFromFrame
#define reEnc			Rf_reEnc
#define rownamesgets		Rf_rownamesgets
#define S3Class                 Rf_S3Class
#define ScalarComplex		Rf_ScalarComplex
#define ScalarInteger		Rf_ScalarInteger
#define ScalarLogical		Rf_ScalarLogical
#define ScalarReal		Rf_ScalarReal
#define ScalarString		Rf_ScalarString
#define ScalarRaw		Rf_ScalarRaw
#define setAttrib		Rf_setAttrib
#define setSVector		Rf_setSVector
#define setVar			Rf_setVar
#define shallow_duplicate	Rf_shallow_duplicate
#define str2type		Rf_str2type
#define stringSuffix		Rf_stringSuffix
#define stringPositionTr	Rf_stringPositionTr
#define StringBlank		Rf_StringBlank
#define substitute		Rf_substitute
#define topenv		        Rf_topenv
#define translateChar		Rf_translateChar
#define translateChar0		Rf_translateChar0
#define translateCharUTF8      	Rf_translateCharUTF8
#define type2char		Rf_type2char
#define type2rstr		Rf_type2rstr
#define type2str		Rf_type2str
#define type2str_nowarn		Rf_type2str_nowarn
#define unprotect		Rf_unprotect
#define unprotect_ptr		Rf_unprotect_ptr
#define VectorToPairList	Rf_VectorToPairList
#define warningcall		Rf_warningcall
#define warningcall_immediate	Rf_warningcall_immediate
#define xlength(x)		Rf_xlength(x)
#define xlengthgets		Rf_xlengthgets

#endif

#if defined(CALLED_FROM_DEFN_H) && !defined(__MAIN__) && (defined(COMPILING_R) || ( __GNUC__ && !defined(__INTEL_COMPILER) ))
#include "Rinlinedfuns.h"
#else
/* need remapped names here for use with R_NO_REMAP */

/*
   These are the inlinable functions that are provided in Rinlinedfuns.h
   It is *essential* that these do not appear in any other header file,
   with or without the Rf_ prefix.
*/

/**
 * @brief Create a vector object.
 *
 *  Allocate a vector object.  This ensures only validity of
 *  SEXPTYPE values representing lists (as the elements must be
 *  initialized).
 *
 * @param stype The type of vector required.
 *
 * @param length The length of the vector to be created.
 *
 * @return Pointer to the created vector.
 */
SEXP     Rf_allocVector(SEXPTYPE, R_xlen_t);
Rboolean Rf_conformable(SEXP, SEXP);
SEXP	 Rf_elt(SEXP, int);
Rboolean Rf_inherits(SEXP, const char *);
Rboolean Rf_isArray(SEXP);
Rboolean Rf_isFactor(SEXP);
Rboolean Rf_isFrame(SEXP);
Rboolean Rf_isFunction(SEXP);
Rboolean Rf_isInteger(SEXP);
Rboolean Rf_isLanguage(SEXP);
Rboolean Rf_isList(SEXP);
Rboolean Rf_isMatrix(SEXP);
Rboolean Rf_isNewList(SEXP);
Rboolean Rf_isNumber(SEXP);
Rboolean Rf_isNumeric(SEXP);
Rboolean Rf_isPairList(SEXP);
Rboolean Rf_isPrimitive(SEXP);
Rboolean Rf_isTs(SEXP);
Rboolean Rf_isUserBinop(SEXP);
Rboolean Rf_isValidString(SEXP);
Rboolean Rf_isValidStringF(SEXP);

/** @brief Is an R value a vector?
 *
 * Vector in this context embraces R matrices and arrays.
 *
 * @param s Pointer to the R value to be tested.  The value may be
 *          R_NilValue, in which case the function returns FALSE.
 *
 * @return TRUE iff \a s points to a vector object.
 */
Rboolean Rf_isVector(SEXP);
Rboolean Rf_isVectorAtomic(SEXP);
Rboolean Rf_isVectorList(SEXP);
Rboolean Rf_isVectorizable(SEXP);
SEXP	 Rf_lang1(SEXP);
SEXP	 Rf_lang2(SEXP, SEXP);
SEXP	 Rf_lang3(SEXP, SEXP, SEXP);
SEXP	 Rf_lang4(SEXP, SEXP, SEXP, SEXP);
SEXP	 Rf_lang5(SEXP, SEXP, SEXP, SEXP, SEXP);
SEXP	 Rf_lang6(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
SEXP	 Rf_lastElt(SEXP);

/** @brief Create an Expression with a specified car and tail.
 *
 * This function protects its arguments from the garbage collector.
 *
 * @param cr Pointer to the 'car' of the element to be created.
 *
 * @param tl Pointer to the 'tail' of the element to be created,
 *          which must be a pairlist or R_NilValue.
 *
 * @return Pointer to the constructed list.
 */
SEXP	 Rf_lcons(SEXP, SEXP);
R_len_t  Rf_length(SEXP);
SEXP	 Rf_list1(SEXP);
SEXP	 Rf_list2(SEXP, SEXP);
SEXP	 Rf_list3(SEXP, SEXP, SEXP);
SEXP	 Rf_list4(SEXP, SEXP, SEXP, SEXP);
SEXP	 Rf_list5(SEXP, SEXP, SEXP, SEXP, SEXP);
SEXP	 Rf_list6(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
SEXP	 Rf_listAppend(SEXP, SEXP);
SEXP	 Rf_mkNamed(SEXPTYPE, const char **);
SEXP	 Rf_mkString(const char *);
int	 Rf_nlevels(SEXP);
int	 Rf_stringPositionTr(SEXP, const char *);
SEXP	 Rf_ScalarComplex(Rcomplex);
SEXP	 Rf_ScalarInteger(int);
SEXP	 Rf_ScalarLogical(int);
SEXP	 Rf_ScalarRaw(Rbyte);
SEXP	 Rf_ScalarReal(double);
SEXP	 Rf_ScalarString(SEXP);
R_xlen_t  Rf_xlength(SEXP);
# ifdef INLINE_PROTECT
SEXP Rf_protect(SEXP);
void Rf_unprotect(int);
void R_ProtectWithIndex(SEXP, PROTECT_INDEX *);
void R_Reprotect(SEXP, PROTECT_INDEX);
# endif
SEXP R_FixupRHS(SEXP x, SEXP y);
#endif

#ifdef USE_RINTERNALS

/* Test macros with function versions above */
#undef isNull
#define isNull(s)	(TYPEOF(s) == NILSXP)
#undef isSymbol
#define isSymbol(s)	(TYPEOF(s) == SYMSXP)
#undef isLogical
#define isLogical(s)	(TYPEOF(s) == LGLSXP)
#undef isReal
#define isReal(s)	(TYPEOF(s) == REALSXP)
#undef isComplex
#define isComplex(s)	(TYPEOF(s) == CPLXSXP)
#undef isExpression
#define isExpression(s) (TYPEOF(s) == EXPRSXP)
#undef isEnvironment
#define isEnvironment(s) (TYPEOF(s) == ENVSXP)
#undef isString
#define isString(s)	(TYPEOF(s) == STRSXP)
#undef isObject
#define isObject(s)	(OBJECT(s) != 0)

/* macro version of R_CheckStack */
#define R_CheckStack() do {						\
	void NORET R_SignalCStackOverflow(intptr_t);				\
	int dummy;							\
	intptr_t usage = R_CStackDir * (R_CStackStart - (uintptr_t)&dummy); \
	if(R_CStackLimit != -1 && usage > ((intptr_t) R_CStackLimit))	\
	    R_SignalCStackOverflow(usage);				\
    } while (FALSE)
#endif


#ifdef __cplusplus
}
#endif

#endif /* R_INTERNALS_H_ */
