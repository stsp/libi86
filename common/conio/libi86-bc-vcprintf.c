/*
 * Copyright (c) 2020--2021 TK Chia
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

#define _BORLANDC_SOURCE
#define _LIBI86_COMPILING_
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "conio.h"
#include "i86.h"
#include "libi86/internal/graph.h"
#include "libi86/internal/acconfig.h"

int __libi86_bc_vcprintf (const char *, va_list) __libi86_autofloat_stdio;

int
__libi86_bc_vcprintf (const char *fmt, va_list ap)
{
  va_list ap_2;
  int res;

  /*
   * Find out how many characters we will be writing, so that we can allocate
   * a large enough buffer.
   */
#if __STDC_VERSION__ - 0 >= 199901L
  va_copy (ap_2, ap);
#else
  ap_2 = ap;
#endif
  res = vsnprintf (NULL, (size_t) 0, fmt, ap_2);
  va_end (ap_2);

  if (res < 0)
    return res;

  /*
   * Do an snprintf (...) into a temporary buffer, then write everything out
   * from there.
   */
  {
    size_t count = (size_t) res + 1;
#if __STDC_VERSION__ - 0 >= 199901L
    char buf[count];
#else
    char *buf = malloc (count);
    if (! buf)
      return -1;
#endif

    res = vsnprintf (buf, count, fmt, ap);

    if (res > 0)
      __libi86_vid_bc_outmem_do (buf, (size_t) res);

#if __STDC_VERSION__ - 0 < 199901L
    free (buf);
#endif
  }

  return res;
}
