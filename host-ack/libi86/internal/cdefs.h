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

#include <libi86/internal/types.h>

#define _LIBI86_BEGIN_EXTERN_C
#define _LIBI86_END_EXTERN_C
#define _LIBI86_EXTERN_C	extern

typedef struct
  {
    unsigned __off_, __seg_;
  }
__libi86_fpv_t, __libi86_fpcv_t, __libi86_fpvv_t, __libi86_fpcvv_t,
__libi86_fpc_t, __libi86_fpuc_t, __libi86_fpcc_t;

#define _LIBI86_DEPRECATED(__msg)
#define _LIBI86_WARNING(__msg)
#define _LIBI86_ERROR(__msg)
#define _LIBI86_NORETURN
#define _LIBI86_SENTINEL0
#define _LIBI86_SENTINEL1
#define _LIBI86_FORMAT(__archetype, __index, __first)

/*
 * ACK lacks support for post-C89 C language features and C extensions,
 * including the `inline' keyboard.  However --- if optimization is enabled
 * --- it does know how to inline calls to local functions into their callers.
 */
#define _LIBI86_STATIC_INLINE	static
#define _LIBI86_ALT_INLINE	static
/*
 * Define a local (and most likely inline) 0-, 1-, 2-, 3, or 4-argument
 * function NAME which calls ALIAS_TO.  RET_TYPE is the (non-void) return
 * type of the function, and TYPE1, TYPE2, TYPE3, and TYPE4 are argument types.
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
#define _LIBI86_REDIRECT_AND_INLINE_3(ret_type, name, type1, type2, type3, \
				      alias_to) \
	extern ret_type alias_to (type1, type2, type3); \
	_LIBI86_ALT_INLINE ret_type name (type1 __arg1, type2 __arg2, \
					  type3 __arg3) \
	{ \
	  return alias_to (__arg1, __arg2, __arg3); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_4(ret_type, name, type1, type2, type3, \
				      type4, alias_to) \
	extern ret_type alias_to (type1, type2, type3, type4); \
	_LIBI86_ALT_INLINE ret_type name (type1 __arg1, type2 __arg2, \
					  type3 __arg3, type4 __arg4) \
	{ \
	  return alias_to (__arg1, __arg2, __arg3, __arg4); \
	}
/*
 * Define a local (and most likely inline) 0-, 1-, 2-, 3, or 4-argument
 * function NAME which returns void and calls ALIAS_TO.  TYPE1, TYPE2, TYPE3,
 * and TYPE4 are argument types.
 */
#define _LIBI86_REDIRECT_AND_INLINE_VOID_0(name, alias_to) \
	extern void alias_to (void); \
	_LIBI86_ALT_INLINE void name (void) \
	{ \
	  alias_to (); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_VOID_1(name, type1, alias_to) \
	extern void alias_to (type1); \
	_LIBI86_ALT_INLINE void name (type1 __arg1) \
	{ \
	  alias_to (__arg1); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_VOID_2(name, type1, type2, alias_to) \
	extern void alias_to (type1, type2); \
	_LIBI86_ALT_INLINE void name (type1 __arg1, type2 __arg2) \
	{ \
	  alias_to (__arg1, __arg2); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_VOID_3(name, type1, type2, type3, \
					   alias_to) \
	extern void alias_to (type1, type2, type3); \
	_LIBI86_ALT_INLINE void name (type1 __arg1, type2 __arg2, \
					  type3 __arg3) \
	{ \
	  alias_to (__arg1, __arg2, __arg3); \
	}
#define _LIBI86_REDIRECT_AND_INLINE_VOID_4(name, type1, type2, type3, \
					   type4, alias_to) \
	extern void alias_to (type1, type2, type3, type4); \
	_LIBI86_ALT_INLINE void name (type1 __arg1, type2 __arg2, \
				      type3 __arg3, type4 __arg4) \
	{ \
	  alias_to (__arg1, __arg2, __arg3, __arg4); \
	}
#define _LIBI86_REDIRECT_0	_LIBI86_REDIRECT_AND_INLINE_0
#define _LIBI86_REDIRECT_1	_LIBI86_REDIRECT_AND_INLINE_1
#define _LIBI86_REDIRECT_2	_LIBI86_REDIRECT_AND_INLINE_2
#define _LIBI86_REDIRECT_3	_LIBI86_REDIRECT_AND_INLINE_3
#define _LIBI86_REDIRECT_4	_LIBI86_REDIRECT_AND_INLINE_4
#define _LIBI86_REDIRECT_VOID_0	_LIBI86_REDIRECT_AND_INLINE_VOID_0
#define _LIBI86_REDIRECT_VOID_1	_LIBI86_REDIRECT_AND_INLINE_VOID_1
#define _LIBI86_REDIRECT_VOID_2	_LIBI86_REDIRECT_AND_INLINE_VOID_2
#define _LIBI86_REDIRECT_VOID_3	_LIBI86_REDIRECT_AND_INLINE_VOID_3
/*
 * If _BORLANDC_SOURCE is in effect, define a local 0-, 1-, 2-, 3- or
 * 4-argument function function NAME which calls __libi86_bc_NAME. 
 * Otherwise, define a normal function prototype.
 */
#ifdef _BORLANDC_SOURCE
# define _LIBI86_BC_REDIRECT_AND_INLINE_0(ret_type, name) \
	 _LIBI86_REDIRECT_AND_INLINE_0(ret_type, name, __libi86_bc_ ## name)
# define _LIBI86_BC_REDIRECT_AND_INLINE_1(ret_type, name, type1) \
	 _LIBI86_REDIRECT_AND_INLINE_1(ret_type, name, type1, \
				       __libi86_bc_ ## name)
# define _LIBI86_BC_REDIRECT_AND_INLINE_2(ret_type, name, type1, type2) \
	 _LIBI86_REDIRECT_AND_INLINE_2(ret_type, name, type1, type2, \
				       __libi86_bc_ ## name)
# define _LIBI86_BC_REDIRECT_AND_INLINE_3(ret_type, name, type1, type2, type3)\
	 _LIBI86_REDIRECT_AND_INLINE_3(ret_type, name, type1, type2, type3, \
				       __libi86_bc_ ## name)
# define _LIBI86_BC_REDIRECT_AND_INLINE_4(ret_type, name, type1, type2, type3,\
					  type4) \
	 _LIBI86_REDIRECT_AND_INLINE_4(ret_type, name, type1, type2, type3, \
				       type4, __libi86_bc_ ## name)
# define _LIBI86_BC_REDIRECT_AND_INLINE_VOID_2(name, type1, type2) \
	 _LIBI86_REDIRECT_AND_INLINE_VOID_2(name, type1, type2, \
					    __libi86_bc_ ## name)
#else  /* ! _BORLANDC_SOURCE */
# define _LIBI86_BC_REDIRECT_AND_INLINE_0(ret_type, name) \
	 extern ret_type name (void);
# define _LIBI86_BC_REDIRECT_AND_INLINE_1(ret_type, name, type1) \
	 extern ret_type name (type1);
# define _LIBI86_BC_REDIRECT_AND_INLINE_2(ret_type, name, type1, type2) \
	 extern ret_type name (type1, type2);
# define _LIBI86_BC_REDIRECT_AND_INLINE_3(ret_type, name, type1, type2, type3)\
	 extern ret_type name (type1, type2, type3);
# define _LIBI86_BC_REDIRECT_AND_INLINE_4(ret_type, name, type1, type2, type3,\
					  type4) \
	 extern ret_type name (type1, type2, type3, type4);
# define _LIBI86_BC_REDIRECT_AND_INLINE_VOID_2(name, type1, type2) \
	 extern void name (type1, type2);
#endif  /* ! _BORLANDC_SOURCE */
#define _LIBI86_BC_REDIRECT_0	_LIBI86_BC_REDIRECT_AND_INLINE_0
#define _LIBI86_BC_REDIRECT_1	_LIBI86_BC_REDIRECT_AND_INLINE_1
#define _LIBI86_BC_REDIRECT_2	_LIBI86_BC_REDIRECT_AND_INLINE_2
#define _LIBI86_BC_REDIRECT_3	_LIBI86_BC_REDIRECT_AND_INLINE_3
#define _LIBI86_BC_REDIRECT_4	_LIBI86_BC_REDIRECT_AND_INLINE_4
#define _LIBI86_BC_REDIRECT_VOID_2 _LIBI86_BC_REDIRECT_AND_INLINE_VOID_2

/* Yep, `va_list' is a pointer-to-char on ACK for the i86 target. */
typedef char *__libi86_va_list_t;
/* And... */
typedef unsigned short __libi86_size_t, __libi86_segment_t;
#define __libi86_autofloat_stdio
#define __libi86_packed

#endif
