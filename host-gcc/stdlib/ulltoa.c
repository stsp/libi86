/*
 * Copyright (c) 2021 TK Chia
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
#include <stdlib.h>
#include "libi86/stdlib.h"

typedef union __attribute__ ((transparent_union))
  {
    unsigned long long qw;
    unsigned sw[4];
  }
qw_as_sw_t;

/*
 * Well, it looks like Open Watcom has the right idea here.  Since the base
 * is always in the range 2 to 36, we really only need to divide a 64-bit
 * dividend by a (!) 6-bit divisor --- & we do not have to rope in a generic
 * (& expensive) 64-bit-by-64-bit division routine.
 *
 * The only wrinkle is that this simpler division, for now, must be
 * implemented using customized code.  -- tkchia 20210211
 */
static void
ulldiv (unsigned long long dividend, unsigned divisor,
	unsigned long long *quotient, unsigned *remainder)
{
  qw_as_sw_t divid = (qw_as_sw_t) dividend, quot;
  unsigned divid3 = divid.sw[3], divid2 = divid.sw[2],
	   divid1 = divid.sw[1], divid0 = divid.sw[0],
	   quot3 = 0, quot2 = 0, quot1 = 0, quot0,
	   rem3 = 0, rem2 = 0, rem1 = 0, rem0;

  if (divid3)
    goto start3;
  if (divid2)
    goto start2;
  if (divid1)
    goto start1;
  goto start0;

start3:
  __asm ("divw %4" : "=a" (quot3), "=d" (rem3)
		   : "0" (divid3), "1" (0U), "rm" (divisor)
		   : "cc");
start2:
  __asm ("divw %4" : "=a" (quot2), "=d" (rem2)
		   : "0" (divid2), "1" (rem3), "rm" (divisor)
		   : "cc");
start1:
  __asm ("divw %4" : "=a" (quot1), "=d" (rem1)
		   : "0" (divid1), "1" (rem2), "rm" (divisor)
		   : "cc");
start0:
  __asm ("divw %4" : "=a" (quot0), "=d" (rem0)
		   : "0" (divid0), "1" (rem1), "rm" (divisor)
		   : "cc");
  quot.sw[3] = quot3;
  quot.sw[2] = quot2;
  quot.sw[1] = quot1;
  quot.sw[0] = quot0;
  *quotient = quot.qw;
  *remainder = rem0;
}

char *
_ulltoa (unsigned long long value, char *buffer, int radix)
{
  static const char digit_map[36] = "0123456789abcdefghijklmnopqrstuvwxyz";
  size_t reverses;
  char *ptr1, *ptr2;

  if (radix < 2 || radix > 36)
    {
      buffer[0] = 0;
      return NULL;
    }

  /* Compute the digits in reverse order. */
  ptr1 = ptr2 = buffer;
  do
    {
      unsigned digit;
      ulldiv (value, (unsigned) radix, &value, &digit);
      *ptr2++ = digit_map[(unsigned char) digit];
    }
  while (value);

  /* Remember the terminating null byte. */
  *ptr2 = 0;

  /* Reverse the digits. */
  reverses = (size_t) (ptr2 - ptr1) / 2;
  while (reverses-- != 0)
    {
      char c = *ptr1;
      *ptr1++ = *--ptr2;
      *ptr2 = c;
    }

  /* We are done. */
  return buffer;
}

_LIBI86_WEAK_ALIAS (_ulltoa) char *
ulltoa (unsigned long long, char *, int);
