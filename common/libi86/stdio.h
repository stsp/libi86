/*
 * Copyright (c) 2019--2021 TK Chia
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

#ifndef _LIBI86_LIBI86_STDIO_H_
#define _LIBI86_LIBI86_STDIO_H_

#include <stdio.h>

#include <libi86/internal/acconfig.h>
#include <libi86/internal/cdefs.h>
#include <libi86/internal/features.h>

_LIBI86_BEGIN_EXTERN_C

/*
 * If the C library has vsscanf (...), then define _vsscanf (...) as an
 * alias of vsscanf (...).
 *
 * If not, but if the necessary feature test macros are present, then do the
 * opposite: define vsscanf (...) as an alias of _vsscanf (...).
 */
#ifdef _LIBI86_INTERNAL_HAVE_VSSCANF
_LIBI86_REDIRECT_AND_INLINE_3 (int, _vsscanf, const char *, const char *,
			       __libi86_va_list_t, vsscanf)
#else
extern int _vsscanf (const char *__str, const char *__fmt,
		    __libi86_va_list_t __ap);
# if _LIBI86_ISO_C_VISIBLE >= 1999
_LIBI86_REDIRECT_AND_INLINE_3 (int, vsscanf, const char *, const char *,
			       __libi86_va_list_t, _vsscanf)
# endif
#endif

_LIBI86_END_EXTERN_C

#endif
