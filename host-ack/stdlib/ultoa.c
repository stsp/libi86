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

/* Divide DIVIDEND by DIVISOR, update *REMAINDER, return quotient. */
extern unsigned long __libi86_ultoa_uldiv (unsigned long dividend,
					   unsigned divisor,
					   unsigned *remainder);

char *
_ultoa (unsigned long value, char *buffer, int radix)
{
  static const char digit_map[36] = "0123456789abcdefghijklmnopqrstuvwxyz";
  register size_t reverses;
  register char *ptr1, *ptr2;

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
      value = __libi86_ultoa_uldiv (value, (unsigned) radix, &digit);
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
