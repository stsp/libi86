/*
 * Copyright (c) 2021 TK Chia
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
#include "dos.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

__libi86_isr_t
_dos_getvect (unsigned intr_no)
{
  typedef __typeof__ ((__builtin_ia16_selector (8))) SEG_t;
  SEG_t seg;
  unsigned off;
#ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    __asm volatile ("int $0x31"
		    : "=c" (seg), "=d" (off)
		    : "a" (0x0204u), "b" ((unsigned char) intr_no)
		    : "cc", "bh", "memory");
  else
#endif
    __asm volatile ("int $0x21"
		    : "=e" (seg), "=b" (off)
		    : "a" (0x3500u | (unsigned char) intr_no)
		    : "cc", "memory");
  return (__libi86_isr_t) MK_FP (seg, off);
}
