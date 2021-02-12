/*
 * Copyright (c) 2021 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING2.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
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
