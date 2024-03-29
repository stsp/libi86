/*
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

#define _LIBI86_COMPILING_
#include <stdlib.h>
#include "i86.h"
#include "libi86/string.h"

#ifndef __GNUC__
# define __builtin_expect(__thang, __expect) (__thang)
#endif

extern __libi86_fpv_t __libi86_fmemmove_forward (__libi86_fpv_t,
						 __libi86_fpcv_t, size_t);
extern __libi86_fpv_t __libi86_fmemmove_backward (__libi86_fpv_t,
						  __libi86_fpcv_t, size_t);

static unsigned long
linear_addr_rm (__libi86_fpcv_t p)
{
  return ((unsigned long) FP_SEG (p) << 4) + FP_OFF (p);
}

__libi86_fpv_t
_fmemmove (__libi86_fpv_t dest, __libi86_fpcv_t src, size_t n)
{
  unsigned long dest_start_l, src_start_l;

  if (! n)
    return dest;

#ifdef __FAR
  if (dest == src)
#else
  if (FP_OFF (dest) == FP_OFF (src) && FP_SEG (dest) == FP_SEG (src))
#endif
    return dest;

  if (FP_SEG (dest) == FP_SEG (src))
    {
      if (FP_OFF (dest) > FP_OFF (src))
	return __libi86_fmemmove_backward (dest, src, n);

      return __libi86_fmemmove_forward (dest, src, n);
    }

  dest_start_l = linear_addr_rm (dest);
  src_start_l = linear_addr_rm (src);

  /* If there is any possibility that the source buffer and destination
     start --- when considered as real mode segment:offset pairs --- might
     overlap, do a backward copy.

     Consider also the case where the A20 line is disabled.  */
  if (src_start_l < dest_start_l && src_start_l + n > dest_start_l)
    return __libi86_fmemmove_backward (dest, src, n);

  if (__builtin_expect (src_start_l > 0xffffful
			|| dest_start_l > 0xffffful, 0))
    {
      src_start_l &= 0xfffffUL;
      dest_start_l &= 0xfffffUL;

      if (src_start_l < dest_start_l && src_start_l + n > dest_start_l)
	return __libi86_fmemmove_backward (dest, src, n);
    }

  return __libi86_fmemmove_forward (dest, src, n);
}
