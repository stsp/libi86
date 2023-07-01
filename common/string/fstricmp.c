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
#include <ctype.h>
#include "libi86/internal/cdefs.h"
#include "libi86/internal/farptr.h"
#include "libi86/string.h"

int
_fstricmp (__libi86_fpcc_t s1, __libi86_fpcc_t s2)
{
  __libi86_fpcc_t p = s1, q = s2;
  unsigned char c1, c2;
  int lc1, lc2;
  do
    {
#ifdef __FAR
      c1 = *p++;
      lc1 = tolower (c1);
      c2 = *q++;
      lc2 = tolower (c2);
#else  /* ! __FAR */
      c1 = (unsigned char) __libi86_peekfpbi (&p);
      lc1 = tolower (c1);
      c2 = (unsigned char) __libi86_peekfpbi (&q);
      lc2 = tolower (c2);
#endif  /* ! __FAR */
      if (lc1 != lc2)
	return lc1 - lc2;
    }
  while (c1 != 0);
  return 0;
}
