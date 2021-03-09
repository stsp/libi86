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

#define _LIBI86_COMPILING_
#include "libi86/internal/cdefs.h"
#include "dos.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

void
_dos_setvect (unsigned intr_no, __libi86_isr_t isr)
{
#ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    __asm volatile ("int $0x31"
		    : /* no output operands */
		    : "a" (0x0205u), "b" ((unsigned char) intr_no),
		      "c" (FP_SEG (isr)), "d" (FP_OFF (isr))
		    : "cc", "bh", "memory");
  else
#endif
    __asm volatile ("int $0x21"
		    : /* no output operands */
		    : "a" (0x2500u | (unsigned char) intr_no),
		      "Rds" (FP_SEG (isr)), "d" (FP_OFF (isr))
		    : "cc", "memory");
}
