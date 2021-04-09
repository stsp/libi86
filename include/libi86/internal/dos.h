/*
 * Copyright (c) 2021 TK Chia
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

#ifndef _LIBI86_LIBI86_INTERNAL_DOS_H_
#define _LIBI86_LIBI86_INTERNAL_DOS_H_

#ifndef _LIBI86_COMPILING_
# error "<libi86/internal/dos.h> should only be used when compiling libi86!"
#endif

#include <stdbool.h>
#include <i86.h>
#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

extern __attribute__ ((regparmcall)) unsigned
__libi86_ret_really_set_errno (unsigned);
#ifdef __IA16_FEATURE_PROTECTED_MODE
extern struct find_t __far *__libi86_dpmi_set_dta (void);
#endif

static inline bool
__libi86_msdos_drive_letter_p (char c)
{
  switch (c)
    {
    case 'A' ... 'Z':
    case 'a' ... 'z':
      return true;
    default:
      return false;
    }
}

static inline bool
__libi86_msdos_path_sep_p (char c)
{
  switch (c)
    {
    case '/':
    case '\\':
      return true;
    default:
      return false;
    }
}

static inline void
__libi86_msdos_set_dta (void __far *new_dta)
{
  unsigned xx1, xx2;
  __asm volatile ("int $0x21"
		  : "=a" (xx1), "=d" (xx2)
		  : "Rah" ((unsigned char) 0x1a),
		    "Rds" (FP_SEG (new_dta)), "1" (FP_OFF (new_dta))
		  : "cc", "bx", "cx", "memory");
}

#endif
