/*
 * Various macros and type definitions, for use by libi86 modules and other
 * libi86 header files.
 *
 * Copyright (c) 2018--2021 TK Chia
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the developer(s) nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    unsigned __off_, __seg_;
  }
__libi86_fpv_t, __libi86_fpcv_t, __libi86_fpvv_t, __libi86_fpcvv_t,
__libi86_fpcc_t;
#endif

/*
 * Declare function prototypes that redirect calls to NAME to call ALIAS_TO
 * instead.  These macros work similarly to __REDIRECT (...) and friends in
 * glibc <sys/cdefs.h>.
 */
#define _LIBI86_REDIRECT_0(ret_type, name, alias_to) \
	extern ret_type alias_to (void); \
	extern ret_type name (void) __asm (_LIBI86_ASM_NAME (alias_to));
#define _LIBI86_REDIRECT_1(ret_type, name, type1, alias_to) \
	extern ret_type alias_to (type1); \
	extern ret_type name (type1) __asm (_LIBI86_ASM_NAME (alias_to));
#define _LIBI86_REDIRECT_2(ret_type, name, type1, type2, alias_to) \
	extern ret_type alias_to (type1, type2); \
	extern ret_type name (type1, type2) \
			__asm (_LIBI86_ASM_NAME (alias_to));
#define _LIBI86_REDIRECT_3(ret_type, name, type1, type2, type3, alias_to) \
	extern ret_type alias_to (type1, type2, type3); \
	extern ret_type name (type1, type2, type3) \
			__asm (_LIBI86_ASM_NAME (alias_to));
#define _LIBI86_REDIRECT_4(ret_type, name, type1, type2, type3, type4, \
			   alias_to) \
	extern ret_type alias_to (type1, type2, type3, type4); \
	extern ret_type name (type1, type2, type3, type4) \
			__asm (_LIBI86_ASM_NAME (alias_to));
#define _LIBI86_ASM_NAME(c_name) \
	_LIBI86_ASM_NAME_P2(__USER_LABEL_PREFIX__, #c_name)
#define _LIBI86_ASM_NAME_P2(prefix, c_name) _LIBI86_ASM_NAME_P3(prefix, c_name)
#define _LIBI86_ASM_NAME_P3(prefix, c_name) #prefix c_name

/*
 * If _BORLANDC_SOURCE is in effect, declare function prototypes that
 * redirect calls to NAME to call __libi86_bc_NAME instead.  Otherwise,
 * declare normal function prototypes.
 */
#ifdef _BORLANDC_SOURCE
# define _LIBI86_BC_REDIRECT_0(ret_type, name) \
	 _LIBI86_REDIRECT_0 (ret_type, name, __libi86_bc_ ## name)
# define _LIBI86_BC_REDIRECT_1(ret_type, name, type1) \
	 _LIBI86_REDIRECT_1 (ret_type, name, type1, __libi86_bc_ ## name)
# define _LIBI86_BC_REDIRECT_2(ret_type, name, type1, type2) \
	 _LIBI86_REDIRECT_2 (ret_type, name, type1, type2, \
			     __libi86_bc_ ## name)
# define _LIBI86_BC_REDIRECT_3(ret_type, name, type1, type2, type3) \
	 _LIBI86_REDIRECT_3 (ret_type, name, type1, type2, type3, \
			     __libi86_bc_ ## name)
# define _LIBI86_BC_REDIRECT_4(ret_type, name, type1, type2, type3, type4) \
	 _LIBI86_REDIRECT_4 (ret_type, name, type1, type2, type3, type4, \
			     __libi86_bc_ ## name)
/*
 * This macro is for corner cases which require special handling under GCC ---
 * e.g. variadic arguments, extra function attributes.
 */
# define _LIBI86_BC_REDIRECT_X(name, proto) \
	 name proto __asm (_LIBI86_BC_ASM_NAME (name))
# define _LIBI86_BC_ASM_NAME(c_name) \
	 _LIBI86_BC_ASM_NAME_P2(__USER_LABEL_PREFIX__, #c_name)
# define _LIBI86_BC_ASM_NAME_P2(prefix, c_name) \
	 _LIBI86_BC_ASM_NAME_P3(prefix, c_name)
# define _LIBI86_BC_ASM_NAME_P3(prefix, c_name) #prefix "__libi86_bc_" c_name
#else
# define _LIBI86_BC_REDIRECT_0(ret_type, name) \
	 extern ret_type name (void);
# define _LIBI86_BC_REDIRECT_1(ret_type, name, type1) \
	 extern ret_type name (type1);
# define _LIBI86_BC_REDIRECT_2(ret_type, name, type1, type2) \
	 extern ret_type name (type1, type2);
# define _LIBI86_BC_REDIRECT_3(ret_type, name, type1, type2, type3) \
	 extern ret_type name (type1, type2, type3);
# define _LIBI86_BC_REDIRECT_4(ret_type, name, type1, type2, type3, type4) \
	 extern ret_type name (type1, type2, type3, type4);
# define _LIBI86_BC_REDIRECT_X(name, proto) name proto
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
/* Various other useful function properties. */
#define _LIBI86_STATIC_INLINE	static inline
#define _LIBI86_NORETURN	__attribute__ ((__noreturn__))
/*
 * Many of the header files have `extern inline' versions of functions which
 * can be used instead of the out-of-line versions, and they require this
 * rather long-winded incantation to work as intended...
 */
#define _LIBI86_ALT_INLINE	__attribute__ ((__gnu_inline__)) extern inline
/*
 * Redirect out-of-line calls to a 0-, 1-, 2-, or 3-argument function NAME
 * to call ALIAS_TO, _and_ also define an inline version of NAME which calls
 * ALIAS_TO.  RET_TYPE is the (non-void) return type of the function, and
 * TYPE1, TYPE2, and TYPE3 are the argument types.
 */
#define _LIBI86_REDIRECT_AND_INLINE_0(ret_type, name, alias_to) \
	_LIBI86_REDIRECT_0 (ret_type, name, alias_to) \
	_LIBI86_ALT_INLINE ret_type name (void) \
	{ \
	  return alias_to (); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_1(ret_type, name, type1, alias_to) \
	_LIBI86_REDIRECT_1 (ret_type, name, type1, alias_to) \
	_LIBI86_ALT_INLINE ret_type name (type1 __arg1) \
	{ \
	  return alias_to (__arg1); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_2(ret_type, name, type1, type2, alias_to) \
	_LIBI86_REDIRECT_2 (ret_type, name, type1, type2, alias_to) \
	_LIBI86_ALT_INLINE ret_type name (type1 __arg1, type2 __arg2) \
	{ \
	  return alias_to (__arg1, __arg2); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_3(ret_type, name, type1, type2, type3, \
				      alias_to) \
	_LIBI86_REDIRECT_3 (ret_type, name, type1, type2, type3, alias_to) \
	_LIBI86_ALT_INLINE ret_type name (type1 __arg1, type2 __arg2, \
					  type3 __arg3) \
	{ \
	  return alias_to (__arg1, __arg2, __arg3); \
	}
/*
 * Redirect out-of-line calls to a 0-, 1-, 2-, or 3-argument function NAME
 * which returns void to call ALIAS_TO, _and_ also define an inline version
 * of NAME which calls ALIAS_TO.  TYPE1, TYPE2, and TYPE3 are the argument
 * types.
 */
#define _LIBI86_REDIRECT_AND_INLINE_VOID_0(name, alias_to) \
	_LIBI86_REDIRECT_0 (void, name, alias_to) \
	_LIBI86_ALT_INLINE void name (void) \
	{ \
	  alias_to (); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_VOID_1(name, type1, alias_to) \
	_LIBI86_REDIRECT_1 (void, name, type1, alias_to) \
	_LIBI86_ALT_INLINE void name (type1 __arg1) \
	{ \
	  alias_to (__arg1); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_VOID_2(name, type1, type2, alias_to) \
	_LIBI86_REDIRECT_2 (void, name, type1, type2, alias_to) \
	_LIBI86_ALT_INLINE void name (type1 __arg1, type2 __arg2) \
	{ \
	  alias_to (__arg1, __arg2); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_VOID_3(name, type1, type2, type3, \
					   alias_to) \
	_LIBI86_REDIRECT_3 (void, name, type1, type2, type3, alias_to) \
	_LIBI86_ALT_INLINE void name (type1 __arg1, type2 __arg2, \
				      type3 __arg3) \
	{ \
	  alias_to (__arg1, __arg2, __arg3); \
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
typedef __builtin_va_list __libi86_va_list_t;
typedef __SIZE_TYPE__ __libi86_size_t;
#ifdef __SEGMENT
typedef __segment __libi86_segment_t;
#else
typedef __typeof__ (__builtin_ia16_selector (0u)) __libi86_segment_t;
#endif

#endif
