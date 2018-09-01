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
   not have a .bp field.  */
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

/* Again, not compatible with Open Watcom.  */
struct REGPACKW
{
  unsigned short ax, bx, cx, dx, bp, si, di, ds, es, flags;
};

struct REGPACKB
{
  unsigned char al, ah, bl, bh, cl, ch, dl, dh;
};

union REGPACK
{
  struct REGPACKB h;
  struct REGPACKW w;
  struct REGPACKW x;
};

extern void segread (struct SREGS *__seg_regs);

/* Used by the inline versions of int86 (...) and intr (...) below.  */
extern int __libi86_int86 (int, const union REGS *, union REGS *);
extern int __libi86_int86_do (const void *, const union REGS *, union REGS *);
extern void __libi86_intr (int, union REGPACK *);
extern void __libi86_intr_do (const void *, union REGPACK *);

#ifndef _LIBI86_COMPILING_
__attribute__ ((__gnu_inline__)) extern inline int
int86 (int __intr_no, const union REGS *__in_regs, union REGS *__out_regs)
{
  if (__builtin_constant_p (__intr_no))
    {
      const void *__intr_call;
      __asm volatile ("movw $__libi86_intr_call_0%c1%c2%c3, %0"
		      : "=g" (__intr_call)
		      : "n" ((__intr_no >> 6) & 3), "n" ((__intr_no >> 3) & 7),
			"n" (__intr_no & 7));
      return __libi86_int86_do (__intr_call, __in_regs, __out_regs);
    }

  return __libi86_int86 (__intr_no, __in_regs, __out_regs);
}

__attribute__ ((__gnu_inline__)) extern inline void
intr (int __intr_no, union REGPACK *__regs)
{
  if (__builtin_constant_p (__intr_no))
    {
      const void *__intr_call;
      __asm volatile ("movw $__libi86_intr_call_0%c1%c2%c3, %0"
		      : "=g" (__intr_call)
		      : "n" ((__intr_no >> 6) & 3), "n" ((__intr_no >> 3) & 7),
			"n" (__intr_no & 7));
      __libi86_intr_do (__intr_call, __regs);
    }
  else
    __libi86_intr (__intr_no, __regs);
}
#endif

#define FP_SEG(__p)	((unsigned) \
			  ((unsigned long) (void __far *) (__p) >> 16))
#define FP_OFF(__p)	__builtin_ia16_FP_OFF (__p)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
