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

#ifndef _LIBI86_INTERNAL_FARPTR_H_
#define _LIBI86_INTERNAL_FARPTR_H_

#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

#if defined __FAR
# define __libi86_FP_SEG(__p)	__builtin_ia16_selector ((unsigned) \
				  ((unsigned long) (void __far *) (__p) >> 16))
# define __libi86_FP_OFF(__p)	__builtin_ia16_FP_OFF (__p)
# define __libi86_MK_FP(__s, __o) \
				((void __far *) \
				 ((unsigned long) (unsigned) (__s) << 16 | \
				  (unsigned) (__o)))

static inline void __far *
__libi86_CV_FP (const volatile void *__p)
{
  return (void __far *) __p;
}
#elif defined __cplusplus  /* ! __FAR */
# define __libi86_FP_SEG(__p)	((__libi86_fpcvv_t (__p)).__FP_SEG ())
# define __libi86_FP_OFF(__p)	((__libi86_fpcvv_t (__p)).__FP_OFF ())
# define __libi86_MK_FP(__s, __o) \
				(__libi86_fpv_t ((__s), (__o)))
# define __libi86_CV_FP(__p)	(__libi86_fpv_t (__p))
#else  /* ! __FAR && ! __cplusplus */
# define __libi86_FP_SEG(__p)	((__p).__seg_)
# define __libi86_FP_OFF(__p)	((__p).__off_)

_LIBI86_ALT_INLINE __libi86_fpv_t
__libi86_MK_FP (unsigned __seg, unsigned __off)
{
  __libi86_fpv_t __p;
  __p.__seg_ = __seg;
  __p.__off_ = __off;
  return __p;
}

extern __libi86_fpv_t __libi86_CV_FP (const volatile void *);
#endif  /* ! __FAR && ! __cplusplus */

_LIBI86_END_EXTERN_C

#endif
