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

#define _LIBI86_COMPILING_
#ifndef __MSDOS__
# include <libgen.h>
#endif
#include <unistd.h>
#include "err.h"
#include "libi86/stdlib.h"
#include "libi86/internal/acconfig.h"
#include "libi86/internal/farptr.h"

#ifdef __MSDOS__
static char __libi86_progname_buf[_MAX_FNAME];
# ifndef __ACK
const char *__libi86_progname = __libi86_progname_buf;
# endif
#else
const char *__libi86_progname = "";
#endif

#ifndef __ACK
static __attribute__ ((constructor (99))) void
#else
const char *
#endif
__libi86_get_progname (void)
{
#ifndef _LIBI86_INTERNAL_HAVE___ARGV
  char fullname[_MAX_PATH] = "", *p = fullname, c;
  if (_osmajor >= 3)
    {
      __libi86_fpcc_t fp = (__libi86_fpcc_t)
			   __libi86_MK_FP (__libi86_peek (_psp, 0x2c), 0);
# ifdef __FAR
      while (*fp++ != 0)
	while (*fp++ != 0);
      fp += 2;
      while ((c = *fp++) != 0 && p != fullname + _MAX_PATH - 1)
	*p++ = c;
# else  /* ! __FAR */
      while (__libi86_peekfpbi (&fp) != 0)
	while (__libi86_peekfpbi (&fp) != 0);
      __libi86_incfpb (&fp);
      __libi86_incfpb (&fp);
      while ((c = __libi86_peekfpbi (&fp)) != 0
	     && p != fullname + _MAX_PATH - 1)
	*p++ = c;
# endif  /* ! __FAR */
      *p = 0;
    }
#else
  const char *fullname = __argv[0];
#endif
#ifdef __MSDOS__
  _splitpath (fullname, NULL, NULL, __libi86_progname_buf, NULL);
# ifdef __ACK
  return __libi86_progname_buf;
# endif  /* __ACK */
#else  /* ! __MSDOS__ */
  __libi86_progname = basename (fullname);
#endif  /* ! __MSDOS__ */
}
