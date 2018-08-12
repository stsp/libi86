/*
 * Copyright (c) 2018 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING3.  If not see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef _LIBI86_I86_H_
#define _LIBI86_I86_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* This definition is not binary-compatible with that in Open Watcom C/C++,
   which has 2 bytes of padding after each of .ax, .bx, .cx, .dx, .si, and
   .di (apparently to accommodate values of %eax, etc).  Open Watcom also does
   not have .bp or .flags fields.  */
struct WORDREGS
{
  unsigned short ax, bx, cx, dx, si, di, bp, cflag, flags;
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

extern int int86 (int inter_no, const union REGS *in_regs,
		  union REGS *out_regs);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
