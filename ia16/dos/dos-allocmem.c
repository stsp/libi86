/*
 * Copyright (c) 2020 TK Chia
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
#include <stdint.h>
#include "libi86/internal/dos.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

unsigned
_dos_allocmem (unsigned size, unsigned *seg)
{
  unsigned ax, max_paras;
  int res;
#ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    {
      unsigned sel;
      __asm volatile ("int $0x31; sbbw %3, %3"
		      : "=a" (ax), "=d" (sel), "=b" (max_paras), "=c" (res)
		      : "0" (0x0100u), "2" (size)
		      : "cc", "memory");
      if (! res)
	{
	  *seg = sel;
	  return 0;
	}
    }
  else
#endif
    {
      int xx;
      __asm volatile ("int $0x21; sbbw %2, %2"
		      : "=a,a" (ax), "=b,b" (max_paras),
			"=c,d" (res), "=d,c" (xx)
		      : "Rah,Rah" ((unsigned char) 0x48), "1,1" (size)
		      : "cc", "memory");
      if (! res)
	{
	  *seg = ax;
	  return 0;
	}
    }

  *seg = max_paras;
  return __libi86_ret_really_set_errno (ax);
}
