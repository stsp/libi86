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
 * Out-of-line implementation of intr (intr_no, ...) for cases where intr_no
 * is not a compile-time constant.
 */

#define _LIBI86_COMPILING_
#include <inttypes.h>
#include "i86.h"

extern const void * const __libi86_intr_dispatch[0x100];

void
__libi86_intr (int intr_no, union REGPACK *regs)
{
  __libi86_intr_do (__libi86_intr_dispatch[(uint8_t) intr_no], regs);
}

__attribute__ ((alias ("__libi86_intr"))) int
intr (int intr_no, union REGPACK *regs);

__attribute__ ((alias ("__libi86_intr"))) int
_intrf (int intr_no, union REGPACK *regs);
