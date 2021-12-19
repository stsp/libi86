/*
 * Copyright (c) 2018--2021 TK Chia
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the developer(s) nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LIBI86_I86_H_
#define _LIBI86_I86_H_

#include <libi86/internal/cdefs.h>
#include <libi86/internal/farptr.h>

_LIBI86_BEGIN_EXTERN_C

/* This definition is not binary-compatible with that in Open Watcom C/C++
   --- the latter does not have a .bp field.  */
struct WORDREGS
{
  unsigned short ax, bx, cx, dx, si, di, bp, cflag;
#ifdef _BORLANDC_SOURCE
  unsigned flags;
#endif
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

/*
 * Used by the actual implementations of int86 (...), intr (...), etc.  Under
 * GCC, these are also used by the inline versions of these routines, defined
 * further below.
 */
#if defined __GNUC__ || defined _LIBI86_COMPILING_
# ifndef _BORLANDC_SOURCE
extern int __libi86_int86 (int, const union REGS *, union REGS *);
extern int __libi86_int86_do (const void *, const union REGS *, union REGS *);
extern int __libi86_int86x (int, const union REGS *, union REGS *,
			    struct SREGS *);
extern int __libi86_int86x_do (const void *, const union REGS *, union REGS *,
			       struct SREGS *);
# else  /* _BORLANDC_SOURCE */
extern int __libi86_bc_int86 (int, const union REGS *, union REGS *);
extern int __libi86_bc_int86_do (const void *, const union REGS *,
				 union REGS *);
extern int __libi86_bc_int86x (int, const union REGS *, union REGS *,
			       struct SREGS *);
extern int __libi86_bc_int86x_do (const void *, const union REGS *,
				  union REGS *, struct SREGS *);
# endif  /* _BORLANDC_SOURCE */
# ifndef __GNUC__
extern const void *__libi86_intr_dispatch (int);
# endif  /* __GNUC__ */
extern void __libi86_intr (int, union REGPACK *);
extern void __libi86_intrf (int, union REGPACK *);
extern void __libi86_intrf_do (const void *, union REGPACK *, unsigned);
/* Used by the inline version of sound (.) below.  */
extern void __libi86_sound (unsigned);
extern void __libi86_sound_by_divisor (unsigned);
#endif  /* __GNUC__ || _LIBI86_COMPILING_ */

#ifndef _LIBI86_COMPILING_
# if ! defined __GNUC__ || ! defined __OPTIMIZE__
extern void _disable (void);
extern void _enable (void);
_LIBI86_BC_REDIRECT_3 (int, int86, int, const union REGS *, union REGS *)
_LIBI86_BC_REDIRECT_4 (int, int86x, int, const union REGS *, union REGS *,
				    struct SREGS *)
extern void intr (int, union REGPACK *);
/* The _...f (...) functions are new functions, which I am also proposing
   (https://github.com/open-watcom/open-watcom-v2/issues/472) to add to Open
   Watcom. These do the same thing as int86 (...), int86x (...), etc., except
   they are also guaranteed to load at least the carry flag from the register
   structures, before raising the interrupt.

   (Of these, _intrf (...) has been accepted for inclusion in Open Watcom
   in late 2018, under the name intrf (...).  OW also kind of "redefined"
   intr (...)'s semantics to always clear the SZAPC flags before issuing
   the interrupt.)  */
_LIBI86_BC_REDIRECT_3 (int, _int86f, int, const union REGS *, union REGS *)
_LIBI86_BC_REDIRECT_4 (int, _int86xf, int, const union REGS *, union REGS *,
				      struct SREGS *)
_LIBI86_REDIRECT_2 (int, intrf, int, union REGPACK *, _intrf)
extern void sound (unsigned);
# else  /* __GNUC__ && __OPTIMIZE__ */
_LIBI86_ALT_INLINE void
_disable (void)
{
  __asm volatile ("cli" : : : "memory");
}

_LIBI86_ALT_INLINE void
_enable (void)
{
  __asm volatile ("sti" : : : "memory");
}

_LIBI86_ALT_INLINE int
_int86f (int __intr_no, const union REGS *__in_regs, union REGS *__out_regs)
{
  if (__builtin_constant_p (__intr_no))
    {
      const void *__intr_call;
      __asm volatile ("{movw $__libi86_intr_call_0%c1%c2%c3, %0"
		      "|mov %0, offset __libi86_intr_call_0%c1%c2%c3}"
		      : "=g" (__intr_call)
		      : "n" ((__intr_no >> 6) & 3), "n" ((__intr_no >> 3) & 7),
			"n" (__intr_no & 7));
#   ifndef _BORLANDC_SOURCE
      return __libi86_int86_do (__intr_call, __in_regs, __out_regs);
#   else
      return __libi86_bc_int86_do (__intr_call, __in_regs, __out_regs);
#   endif
    }

#   ifndef _BORLANDC_SOURCE
  return __libi86_int86 (__intr_no, __in_regs, __out_regs);
#   else
  return __libi86_bc_int86 (__intr_no, __in_regs, __out_regs);
#   endif
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
      __asm volatile ("{movw $__libi86_intr_call_0%c1%c2%c3, %0"
		      "|mov %0, offset __libi86_intr_call_0%c1%c2%c3}"
		      : "=g" (__intr_call)
		      : "n" ((__intr_no >> 6) & 3), "n" ((__intr_no >> 3) & 7),
			"n" (__intr_no & 7));
#   ifndef _BORLANDC_SOURCE
      return __libi86_int86x_do (__intr_call, __in_regs, __out_regs,
				 __seg_regs);
#   else
      return __libi86_bc_int86x_do (__intr_call, __in_regs, __out_regs,
				    __seg_regs);
#   endif
    }

#   ifndef _BORLANDC_SOURCE
  return __libi86_int86x (__intr_no, __in_regs, __out_regs, __seg_regs);
#   else
  return __libi86_bc_int86x (__intr_no, __in_regs, __out_regs, __seg_regs);
#   endif
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
      __asm volatile ("{movw $__libi86_intr_call_0%c1%c2%c3, %0"
		      "|mov %0, offset __libi86_intr_call_0%c1%c2%c3}"
		      : "=g" (__intr_call)
		      : "n" ((__intr_no >> 6) & 3), "n" ((__intr_no >> 3) & 7),
			"n" (__intr_no & 7));
      __libi86_intrf_do (__intr_call, __regs, __regs->w.flags);
    }
  else
    __libi86_intrf (__intr_no, __regs);
}

_LIBI86_ALT_INLINE void
intr (int __intr_no, union REGPACK *__regs)
{
  if (__builtin_constant_p (__intr_no))
    {
      const void *__intr_call;
      __asm volatile ("{movw $__libi86_intr_call_0%c1%c2%c3, %0"
		      "|mov %0, offset __libi86_intr_call_0%c1%c2%c3}"
		      : "=g" (__intr_call)
		      : "n" ((__intr_no >> 6) & 3), "n" ((__intr_no >> 3) & 7),
			"n" (__intr_no & 7));
      __libi86_intrf_do (__intr_call, __regs, 0);
    }
  else
    __libi86_intr (__intr_no, __regs);
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
# endif	/* __GNUC__ && __OPTIMIZE__ */
#endif	/* _LIBI86_COMPILING_ */

#define FP_SEG(__p)	__libi86_FP_SEG (__p)
#define FP_OFF(__p)	__libi86_FP_OFF (__p)
#define MK_FP(__s, __o)	__libi86_MK_FP ((__s), (__o))
#define _CV_FP(__p)	__libi86_CV_FP (__p)
#define _FP_EQ(__p, __q) __libi86_FP_EQ ((__p), (__q))

#define _FP_SEG(__p)	FP_SEG (__p)
#define _FP_OFF(__p)	FP_OFF (__p)
#define _MK_FP(__s, __o) MK_FP (__s, __o)

/* These are here for testing purposes. */
#ifdef __GNUC__
# define __libi86_get_cs() \
	({ \
	  unsigned short __cs; \
	  __asm volatile ("{movw %%cs, %0|mov %0, cs}" : "=rm" (__cs)); \
	  __cs; \
	})
# define __libi86_get_ss() \
	({ \
	  unsigned short __ss; \
	  __asm volatile ("{movw %%ss, %0|mov %0, ss}" : "=rm" (__ss)); \
	  __ss; \
	})
# define __libi86_get_flags() \
	({ \
	  unsigned short __flags; \
	  __asm volatile ("pushfw; pop{w} %0" : "=rm" (__flags)); \
	  __flags; \
	})
#else  /* ! __GNUC__ */
extern unsigned short __libi86_get_cs (void);
extern unsigned short __libi86_get_ss (void);
extern unsigned short __libi86_get_flags (void);
#endif  /* ! __GNUC__ */

_LIBI86_END_EXTERN_C

#endif
