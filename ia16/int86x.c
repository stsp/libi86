/*
 * Copyright (c) 2018 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING3.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
 */

/*
 * Out-of-line implementation of int86x (intr_no, ...) for cases where
 * intr_no is not a compile-time constant.
 */

#define _LIBI86_COMPILING_
#include <inttypes.h>
#include "i86.h"

extern const void * const __libi86_intr_dispatch[0x100];

int
__libi86_int86x (int intr_no, const union REGS *in_regs, union REGS *out_regs,
		 struct SREGS *seg_regs)
{
  return __libi86_int86x_do (__libi86_intr_dispatch[(uint8_t) intr_no],
			     in_regs, out_regs, seg_regs);
}

_LIBI86_ALIAS (__libi86_int86x) int
int86x (int intr_no, const union REGS *in_regs, union REGS *out_regs,
	struct SREGS *seg_regs);

_LIBI86_ALIAS (__libi86_int86x) int
_int86xf (int intr_no, const union REGS *in_regs, union REGS *out_regs,
	  struct SREGS *seg_regs);
