/*
 * Copyright (c) 2023 TK Chia
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

#ifndef _LIBI86_ERR_H_
#define _LIBI86_ERR_H_

#include <libi86/internal/cdefs.h>
#include <libi86/internal/farptr.h>

_LIBI86_BEGIN_EXTERN_C

#ifdef _LIBI86_COMPILING_
# ifndef __ACK
extern const char *__libi86_progname;
# else
extern const char *__libi86_get_progname (void);
#   define __libi86_progname (__libi86_get_progname ())
# endif
#endif

extern void err (int, const char *, ...)
	    _LIBI86_NORETURN _LIBI86_FORMAT (__printf__, 2, 3);
extern void warn (const char *, ...) _LIBI86_FORMAT (__printf__, 1, 2);

extern void _verr (int, const char *, __libi86_va_list_t)
	    _LIBI86_NORETURN _LIBI86_FORMAT (__printf__, 2, 0);
extern void _vwarn (const char *, __libi86_va_list_t)
	    _LIBI86_FORMAT (__printf__, 1, 0);
#ifndef _LIBI86_COMPILING_
_LIBI86_REDIRECT_VOID_3 (verr, int, const char *, __libi86_va_list_t, _verr)
_LIBI86_REDIRECT_VOID_2 (vwarn, const char *, __libi86_va_list_t, _vwarn)
#endif

_LIBI86_END_EXTERN_C

#endif
