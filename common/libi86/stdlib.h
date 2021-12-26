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

#ifndef _LIBI86_LIBI86_STDLIB_H_
#define _LIBI86_LIBI86_STDLIB_H_

#include <stdlib.h>

#include <libi86/internal/cdefs.h>
#include <libi86/internal/acconfig.h>

_LIBI86_BEGIN_EXTERN_C

/*
 * Try to defer to the underlying C runtime's idea of the maximum length of
 * file name paths.
 *
 * If _PATH_MAX is defined in <sys/syslimits.h> (as detected by the
 * `configure' script), or PATH_MAX is defined in <limits.h>, then use that.
 *
 * Otherwise, follow Open Watcom in defining a maximum length of 144. 
 * (FIXME: why 144?).
 */
#if defined _LIBI86_INTERNAL_HAVE__PATH_MAX
/*
 * Try to coax <sys/syslimits.h> into giving us _only_ _PATH_MAX.  If
 * <sys/syslimits.h> does not honour __need__PATH_MAX, then too bad.  :-|
 */
# define __need__PATH_MAX
# ifdef PATH_MAX
#   include <sys/syslimits.h>
# else
#   include <sys/syslimits.h>
#   ifdef PATH_MAX
#     pragma GCC warning \
		 "<libi86/stdlib.h> pollutes user namespace with PATH_MAX"
#   endif
# endif
# undef __need_PATH_MAX
# define _MAX_PATH	_PATH_MAX
#elif defined _LIBI86_INTERNAL_HAVE_PATH_MAX
# include <limits.h>
# pragma GCC warning \
	     "<libi86/stdlib.h> pollutes user namespace with <limits.h> stuff"
# define _MAX_PATH	PATH_MAX
#else
# define _MAX_PATH	144
#endif
#define _MAX_DRIVE	3
#define _MAX_DIR	_MAX_PATH
#define _MAX_FNAME	9
#define _MAX_EXT	5

#ifndef __ACK
extern unsigned _psp;
extern unsigned char _osmajor, _osminor;
#else  /* __ACK */
/*
 * The Amsterdam Compiler Kit does not (yet) allow library modules to insert
 * initialization functions that are to be invoked before main (...).  This is
 * a rather inefficient workaround that computes _psp, _osmajor, & _osminor
 * as needed at run time.  -- 20211018
 */
extern unsigned __libi86_get_psp (void), __libi86_get_osmajor_osminor (void);

# define _psp		(__libi86_get_psp ())
# define _osmajor	((unsigned char) __libi86_get_osmajor_osminor ())
# define _osminor	((unsigned char) \
			 (__libi86_get_osmajor_osminor () >> 8))
#endif  /* __ACK */

#ifdef _LIBI86_INTERNAL_HAVE_LONG_LONG_INT
extern char *_lltoa (long long __value, char *__buffer, int __radix);
extern char *_ulltoa (unsigned long long __value, char *__buffer, int __radix);
#endif
extern char *_fullpath (char *__buffer, const char *__path,
			__libi86_size_t __size);
extern char *_ltoa (long __value, char *__buffer, int __radix);
extern int _makepath (char __path[_MAX_PATH], const char *__drive,
		      const char *__dir, const char *__fname,
		      const char *__ext);
extern void _splitpath (const char *__path, char __drive[_MAX_DRIVE],
			char __dir[_MAX_DIR], char __fname[_MAX_FNAME],
			char __ext[_MAX_EXT]);
extern char *_ultoa (unsigned long __value, char *__buffer, int __radix);
#ifndef _LIBI86_COMPILING_
# ifndef __STRICT_ANSI__
#   ifdef _LIBI86_INTERNAL_HAVE_LONG_LONG_INT
_LIBI86_REDIRECT_3 (char *, lltoa, long long, char *, int, _lltoa)
_LIBI86_REDIRECT_3 (char *, ulltoa, unsigned long long, char *, int, _ulltoa)
#   endif
_LIBI86_REDIRECT_3 (char *, ltoa, long, char *, int, _ltoa)
_LIBI86_REDIRECT_3 (char *, ultoa, unsigned long, char *, int, _ultoa)
# endif
#endif

_LIBI86_END_EXTERN_C

#endif
