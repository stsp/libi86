/*
 * Copyright (c) 2020 TK Chia
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
#include <stdint.h>
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

extern __attribute__ ((regparmcall)) unsigned
__libi86_ret_really_set_errno (unsigned);

unsigned
_dos_freemem (unsigned seg)
{
  unsigned err, xx;
#ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    __asm volatile ("int $0x31; "
		    "jc 0f; "
		    "xorw %0, %0; "
		    "0:"
		    : "=a" (err), "=d" (xx)
		    : "0" (0x0101u), "1" (seg)
		    : "cc", "bx", "cx", "memory");
  else
#endif
    __asm volatile ("int $0x21; "
		    "jc 0f; "
		    "xorw %0, %0; "
		    "0:"
		    : "=a" (err)
		    : "Rah" ((unsigned char) 0x49),
		      "e" (__builtin_ia16_selector (seg))
		    : "cc", "bx", "cx", "dx", "memory");
  return __libi86_ret_really_set_errno (err);
}
