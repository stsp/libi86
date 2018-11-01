/*
 * Various macros for use by other libi86 header files.
 *
 * Copyright (c) 2018 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING3.LIB.  If not see
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

/*
 * Declare function prototypes that redirect calls to NAME to call ALIAS_TO
 * instead.  These macros work similarly to __REDIRECT (...) and friends in
 * glibc <sys/cdefs.h>.
 */
#define _LIBI86_REDIRECT(name, proto, alias_to) \
	name proto __asm (_LIBI86_ASM_NAME (__USER_LABEL_PREFIX__, #alias_to))
#define _LIBI86_ASM_NAME(prefix, c_name) _LIBI86_ASM_NAME_2(prefix, c_name)
#define _LIBI86_ASM_NAME_2(prefix, c_name) #prefix c_name

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

#endif