/*
 * Copyright (c) 2018 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING2.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
 */

#define _LIBI86_COMPILING_
#include <stdlib.h>
#include "i86.h"

extern void __far *__libi86_fmemmove_forward (void __far *,
					      const void __far *, size_t);
extern void __far *__libi86_fmemmove_backward (void __far *,
					       const void __far *, size_t);

static unsigned long
linear_addr_rm (const void __far *p)
{
  return ((unsigned long) FP_SEG (p) << 4) + FP_OFF (p);
}

void __far *
_fmemmove (void __far *dest, const void __far *src, size_t n)
{
  unsigned long dest_start_l, src_start_l;

  if (dest == src || ! n)
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
      src_start_l &= 0xffffful;
      dest_start_l &= 0xffffful;

      if (src_start_l < dest_start_l && src_start_l + n > dest_start_l)
	return __libi86_fmemmove_backward (dest, src, n);
    }

  return __libi86_fmemmove_forward (dest, src, n);
}
