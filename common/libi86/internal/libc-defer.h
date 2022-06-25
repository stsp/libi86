/*
 * Copyright (c) 2022 TK Chia
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

/*
 * Macros to defer to the underlying libc when it already implements certain
 * routines that libi86 also implements.
 */

#ifndef _LIBI86_LIBI86_INTERNAL_LIBC_DEFER_H_
#define _LIBI86_LIBI86_INTERNAL_LIBC_DEFER_H_

#include <libi86/internal/cdefs.h>

/*
 * Decide how to declare the 0-argument functions named NAME & _{NAME}.
 * For example, NAME might be `getpid'.
 *
 *   * If IN_LIBC is the token `no', then the underlying libc lacks the
 *     function NAME.  Declare both NAME & _{NAME} to be aliases of
 *     __libi86_{NAME}.
 *
 *   * If IN_LIBC is the token `yes', then it means the underlying libc
 *     already has NAME.  In this case, declare _{NAME} to be an alias of
 *     NAME --- i.e. the libc definition --- & also separtely declare
 *     __libi86_{NAME}, which will always be our libi86 definition.
 */
#define _LIBI86_LIBC_DEFER_0(ret_type, name, in_libc) \
	_LIBI86_LIBC_DEFER_ ## in_libc ## _0 (ret_type, name)
#define _LIBI86_LIBC_DEFER_yes_0(ret_type, name) \
	_LIBI86_REDIRECT_0 (ret_type, _ ## name, name) \
	ret_type __libi86_ ## name (void);
#define _LIBI86_LIBC_DEFER_no_0(ret_type, name) \
	_LIBI86_REDIRECT_0 (ret_type, name, __libi86_ ## name) \
	_LIBI86_REDIRECT_0 (ret_type, _ ## name, __libi86_ ## name)
/* Ditto for functions with 1 or 2 arguments. */
#define _LIBI86_LIBC_DEFER_1(ret_type, name, type1, in_libc) \
	_LIBI86_LIBC_DEFER_ ## in_libc ## _1 (ret_type, name, type1)
#define _LIBI86_LIBC_DEFER_2(ret_type, name, type1, type2, in_libc) \
	_LIBI86_LIBC_DEFER_ ## in_libc ## _2 (ret_type, name, type1, type2)
#define _LIBI86_LIBC_DEFER_yes_1(ret_type, name, type1) \
	_LIBI86_REDIRECT_1 (ret_type, _ ## name, type1, name) \
	ret_type __libi86_ ## name (type1);
#define _LIBI86_LIBC_DEFER_no_1(ret_type, name, type1) \
	_LIBI86_REDIRECT_1 (ret_type, name, type1, __libi86_ ## name) \
	_LIBI86_REDIRECT_1 (ret_type, _ ## name, type1, __libi86_ ## name)
#define _LIBI86_LIBC_DEFER_yes_2(ret_type, name, type1, type2) \
	_LIBI86_REDIRECT_2 (ret_type, _ ## name, type1, type2, name) \
	ret_type __libi86_ ## name (type1, type2);
#define _LIBI86_LIBC_DEFER_no_2(ret_type, name, type1, type2) \
	_LIBI86_REDIRECT_2 (ret_type, name, type1, type2, __libi86_ ## name) \
	_LIBI86_REDIRECT_2 (ret_type, _ ## name, type1, type2, \
				      __libi86_ ## name)

#endif
