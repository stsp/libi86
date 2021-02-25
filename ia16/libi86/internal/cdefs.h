/*
 * Various macros and type definitions, for use by libi86 modules and other
 * libi86 header files.
 *
 * Copyright (c) 2018--2021 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING2.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef _LIBI86_INTERNAL_CDEFS_H_
#define _LIBI86_INTERNAL_CDEFS_H_

#ifdef __cplusplus
# define _LIBI86_BEGIN_EXTERN_C	extern "C" {
# define _LIBI86_END_EXTERN_C	}
#else
# define _LIBI86_BEGIN_EXTERN_C
# define _LIBI86_END_EXTERN_C
#endif

#ifdef __FAR
typedef void __far *__libi86_fpv_t;
typedef const void __far *__libi86_fpcv_t;
typedef volatile void __far *__libi86_fpvv_t;
typedef const volatile void __far *__libi86_fpcvv_t;
typedef const char __far *__libi86_fpcc_t;
/*
 * As of October 2020, GCC does not yet support named address spaces --- in
 * particular our __far keyword --- for the C++ & Objective-C languages.
 */
#elif defined __cplusplus
# include <libi86/internal/far-cxx.h>
#else
typedef struct
  {
    unsigned long __p_;
  }
__libi86_fpv_t, __libi86_fpcv_t, __libi86_fpvv_t, __libi86_fpcvv_t,
__libi86_fpcc_t;
#endif

/*
 * Declare function prototypes that redirect calls to NAME to call ALIAS_TO
 * instead.  These macros work similarly to __REDIRECT (...) and friends in
 * glibc <sys/cdefs.h>.
 */
#define _LIBI86_REDIRECT(name, proto, alias_to) \
	name proto __asm (_LIBI86_ASM_NAME (alias_to))
#define _LIBI86_ASM_NAME(c_name) \
	_LIBI86_ASM_NAME_2(__USER_LABEL_PREFIX__, #c_name)
#define _LIBI86_ASM_NAME_2(prefix, c_name) _LIBI86_ASM_NAME_3(prefix, c_name)
#define _LIBI86_ASM_NAME_3(prefix, c_name) #prefix c_name

/*
 * If _BORLANDC_SOURCE is in effect, declare function prototypes that
 * redirect calls to NAME to call __libi86_bc_NAME instead.  Otherwise,
 * declare normal function prototypes.
 */
#ifdef _BORLANDC_SOURCE
# define _LIBI86_BC_REDIRECT(name, proto) \
	 name proto __asm (_LIBI86_BC_ASM_NAME (name))
# define _LIBI86_BC_ASM_NAME(c_name) \
	 _LIBI86_BC_ASM_NAME_2(__USER_LABEL_PREFIX__, #c_name)
# define _LIBI86_BC_ASM_NAME_2(prefix, c_name) \
	 _LIBI86_BC_ASM_NAME_3(prefix, c_name)
# define _LIBI86_BC_ASM_NAME_3(prefix, c_name) #prefix "__libi86_bc_" c_name
#else
# define _LIBI86_BC_REDIRECT(name, proto) name proto
#endif

/*
 * Add an attribute to make a function declaration an alias for the function
 * ALIAS_TO.
 */
#define _LIBI86_ALIAS(alias_to) \
	__attribute__ ((__alias__ (_LIBI86_ASM_NAME (alias_to))))
/*
 * Add attributes to make a function declaration a weak alias for the
 * function ALIAS_TO.
 */
#define _LIBI86_WEAK_ALIAS(alias_to) \
	__attribute__ ((__weak__, __alias__ (_LIBI86_ASM_NAME (alias_to))))
/*
 * If compiling user code, add an attribute to warn that a function is
 * deprecated.
 */
#ifdef _LIBI86_COMPILING_
# define _LIBI86_DEPRECATED(msg)
# define _LIBI86_WARNING(msg)
# define _LIBI86_ERROR(msg)
#else
# define _LIBI86_DEPRECATED(msg) __attribute__ ((__deprecated__ (msg)))
# define _LIBI86_WARNING(msg)	__attribute__ ((__warning__ (msg)))
# define _LIBI86_ERROR(msg)	__attribute__ ((__error__ (msg)))
#endif
/*
 * Many of the header files have `extern inline' versions of functions which
 * can be used instead of the out-of-line versions, and they require this
 * rather long-winded incantation to work as intended...
 */
#define _LIBI86_ALT_INLINE	__attribute__ ((__gnu_inline__)) extern inline

/*
 * Redirect out-of-line calls to a 1-argument function NAME to call
 * ALIAS_TO, _and_ also define an inline version of NAME which calls
 * ALIAS_TO.  RET_TYPE is the (non-void) return type of the function, and
 * TYPE1 is the argument type.
 */
#define _LIBI86_REDIRECT_AND_INLINE_1(ret_type, name, type1, alias_to) \
	extern ret_type _LIBI86_REDIRECT (name, (type1), alias_to); \
	_LIBI86_ALT_INLINE ret_type name (type1 __arg1) \
	{ \
	  return alias_to (__arg1); \
	}
/*
 * Redirect out-of-line calls to a 2-argument function NAME to call
 * ALIAS_TO, _and_ also define an inline version of NAME which calls
 * ALIAS_TO.  RET_TYPE is the (non-void) return type of the function, and
 * TYPE1 and TYPE2 are the argument types.
 */
#define _LIBI86_REDIRECT_AND_INLINE_2(ret_type, name, type1, type2, alias_to) \
	extern ret_type _LIBI86_REDIRECT (name, (type1, type2), alias_to); \
	_LIBI86_ALT_INLINE ret_type name (type1 __arg1, type2 __arg2) \
	{ \
	  return alias_to (__arg1, __arg2); \
	}

/*
 * Try to define a Watcom-compatible __interrupt "type qualifier", if the
 * compiler does not already define or recognize it.
 */
#ifndef __INTERRUPT
# if defined __interrupt
#   warning "suspicious __interrupt macro definition"
# elif defined __IA16_FEATURE_ATTRIBUTE_INTERRUPT
#   define __interrupt	__attribute__ ((__interrupt__))
#   define __INTERRUPT
# endif
#endif

/* And... */
typedef __builtin_va_list _LIBI86_VA_LIST;
#ifdef _LIBI86_COMPILING_
typedef __typeof__ (__builtin_ia16_selector (0u)) _LIBI86_SEG_SELECTOR;
#endif

#endif
