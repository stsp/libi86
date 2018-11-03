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

extern void delay (unsigned);
extern void nosound (void);
extern void segread (struct SREGS *__seg_regs);

/* Used by the inline versions of int86 (...), intr (...), etc. below.  */
extern int __libi86_int86 (int, const union REGS *, union REGS *);
extern int __libi86_int86_do (const void *, const union REGS *, union REGS *);
extern int __libi86_int86x (int, const union REGS *, union REGS *,
			    struct SREGS *);
extern int __libi86_int86x_do (const void *, const union REGS *, union REGS *,
			       struct SREGS *);
extern void __libi86_intr (int, union REGPACK *);
extern void __libi86_intr_do (const void *, union REGPACK *);
/* Used by the inline version of sound (.) below.  */
extern void __libi86_sound (unsigned);
extern void __libi86_sound_by_divisor (unsigned);

#ifndef _LIBI86_COMPILING_
# ifndef __OPTIMIZE__
extern void _disable (void);
extern void _enable (void);
extern int int86 (int, const union REGS *, union REGS *);
extern int int86x (int, const union REGS *, union REGS *, struct SREGS *);
extern void intr (int, union REGPACK *);
/* The _...f (...) functions are new functions, which I am also proposing
   (https://github.com/open-watcom/open-watcom-v2/issues/472) to add to Open
   Watcom. These do the same thing as int86 (...), intr (...), etc., except
   they are also guaranteed to load at least the carry flag from the register
   structures, before raising the interrupt.  */
extern int _int86f (int, const union REGS *, union REGS *);
extern int _int86xf (int, const union REGS *, union REGS *, struct SREGS *);
extern void _intrf (int, union REGPACK *);
extern void sound (unsigned);
# else
_LIBI86_ALT_INLINE void
_disable (void)
{
  __asm volatile ("cli");
}

_LIBI86_ALT_INLINE void
_enable (void)
{
  __asm volatile ("sti");
}

_LIBI86_ALT_INLINE int
_int86f (int __intr_no, const union REGS *__in_regs, union REGS *__out_regs)
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

_LIBI86_ALT_INLINE int
int86 (int __intr_no, const union REGS *__in_regs, union REGS *__out_regs)
{
  return _int86f (__intr_no, __in_regs, __out_regs);
}

_LIBI86_ALT_INLINE int
_int86xf (int __intr_no, const union REGS *__in_regs, union REGS *__out_regs,
	  struct SREGS *__seg_regs)
{
  if (__builtin_constant_p (__intr_no))
    {
      const void *__intr_call;
      __asm volatile ("movw $__libi86_intr_call_0%c1%c2%c3, %0"
		      : "=g" (__intr_call)
		      : "n" ((__intr_no >> 6) & 3), "n" ((__intr_no >> 3) & 7),
			"n" (__intr_no & 7));
      return __libi86_int86x_do (__intr_call, __in_regs, __out_regs,
				 __seg_regs);
    }

  return __libi86_int86x (__intr_no, __in_regs, __out_regs, __seg_regs);
}

_LIBI86_ALT_INLINE int
int86x (int __intr_no, const union REGS *__in_regs, union REGS *__out_regs,
	struct SREGS *__seg_regs)
{
  return _int86xf (__intr_no, __in_regs, __out_regs, __seg_regs);
}

_LIBI86_ALT_INLINE void
_intrf (int __intr_no, union REGPACK *__regs)
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

_LIBI86_ALT_INLINE void
intr (int __intr_no, union REGPACK *__regs)
{
  _intrf (__intr_no, __regs);
}

_LIBI86_ALT_INLINE void
sound (int __freq)
{
  /* If the frequency is a compile-time constant, we can precompute the
     timer counter divisor value to send to the 8253/8254 PIT.  */
  if (__builtin_constant_p (__freq))
    {
      unsigned __divisor = 0;
      if (__freq > 18)
	__divisor = (1193181ul + __freq / 2) / __freq;
      __libi86_sound_by_divisor (__divisor);
    }
  else
    __libi86_sound (__freq);
}
# endif
#endif

#define FP_SEG(__p)	((unsigned) \
			  ((unsigned long) (void __far *) (__p) >> 16))
#define FP_OFF(__p)	__builtin_ia16_FP_OFF (__p)

_LIBI86_END_EXTERN_C

#endif
