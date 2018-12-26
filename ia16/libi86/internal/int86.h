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
 * Declarations of data types used by both <i86.h> and <dos.h> for invoking
 * software interrupts.
 */

#ifndef _LIBI86_INTERNAL_INT86_H_
#define _LIBI86_INTERNAL_INT86_H_

#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

/* This definition is not binary-compatible with that in Open Watcom C/C++
   --- the latter does not have a .bp field.  */
struct WORDREGS
{
  unsigned short ax, bx, cx, dx, si, di, bp, cflag;
};

struct BYTEREGS
{
  unsigned char al, ah, bl, bh, cl, ch, dl, dh;
};

union REGS
{
  struct WORDREGS x;
  struct WORDREGS w;
  struct BYTEREGS h;
};

struct SREGS
{
  unsigned short es, cs, ss, ds;
};

_LIBI86_END_EXTERN_C

#endif
