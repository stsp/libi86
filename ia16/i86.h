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

extern int __libi86_int86 (int, const union REGS *, union REGS *);
extern int __libi86_int86_do (const void *, const union REGS *, union REGS *);

#ifndef _LIBI86_COMPILING_
__attribute__ ((gnu_inline)) extern inline int
int86 (int intr_no, const union REGS *in_regs, union REGS *out_regs)
{
  if (__builtin_constant_p (intr_no))
    {
      const void *intr_call;
      __asm volatile ("movw $__libi86_intr_call_0%c1%c2%c3, %0"
		      : "=g" (intr_call)
		      : "n" ((intr_no >> 6) & 3), "n" ((intr_no >> 3) & 7),
			"n" (intr_no & 7));
      return __libi86_int86_do (intr_call, in_regs, out_regs);
    }

  return __libi86_int86 (intr_no, in_regs, out_regs);
}
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
