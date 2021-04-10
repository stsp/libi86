/*
 * Various macros and type definitions, for use by libi86 modules and other
 * libi86 header files, when compiling with the Amsterdam Compiler Kit.
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

#define _LIBI86_BEGIN_EXTERN_C
#define _LIBI86_END_EXTERN_C

typedef struct
  {
    unsigned long __p_;
  }
__libi86_fpv_t, __libi86_fpcv_t, __libi86_fpvv_t, __libi86_fpcvv_t,
__libi86_fpcc_t;

#define _LIBI86_DEPRECATED(msg)
#define _LIBI86_WARNING(msg)
#define _LIBI86_ERROR(msg)

/*
 * ACK lacks support post-C89 C language features and C extensions,
 * including the `inline' keyboard.  However --- if optimization is enabled
 * --- it does know how to inline calls to local functions into their callers.
 */
#define _LIBI86_ALT_INLINE	static
/*
 * Define a local (and most likely inline) 0-, 1, or 2-argument function NAME
 * which calls ALIAS_TO.  RET_TYPE is the (non-void) return type of the
 * function, and TYPE1 and TYPE2 are argument types.
 */
#define _LIBI86_REDIRECT_AND_INLINE_0(ret_type, name, alias_to) \
	extern ret_type alias_to (void); \
	_LIBI86_ALT_INLINE ret_type name (void) \
	{ \
	  return alias_to (); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_1(ret_type, name, type1, alias_to) \
	extern ret_type alias_to (type1); \
	_LIBI86_ALT_INLINE ret_type name (type1 __arg1) \
	{ \
	  return alias_to (__arg1); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_2(ret_type, name, type1, type2, alias_to) \
	extern ret_type alias_to (type1, type2); \
	_LIBI86_ALT_INLINE ret_type name (type1 __arg1, type2 __arg2) \
	{ \
	  return alias_to (__arg1, __arg2); \
	}

/* Yep, `va_list' is a pointer-to-char on ACK for the i86 target. */
typedef char *_LIBI86_VA_LIST;

#endif