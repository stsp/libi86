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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "dos.h"
#include "libi86/internal/dos.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

unsigned
_dos_findnext (struct find_t *buf)
{
#ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    {
      struct find_t __far *dta = __libi86_dpmi_set_dta ();
      rm_call_struct rmc;
      int res;

      if (! dta)
	return errno;

      *dta = *buf;

      rmc.ax = 0x4f00U;
      res = _DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc);

      *buf = *dta;

      if (res != 0)
	{
	  errno = res = EIO;
	  return res;
	}

      if ((rmc.flags & 1) != 0)
	{
	  errno = res = rmc.ax;
	  return res;
	}

      return 0;
    }
  else
#endif
    {
      unsigned ax, res, xx1, xx2;

      __libi86_msdos_set_dta (buf);
      __asm volatile ("int $0x21; sbbw %1, %1"
		      : "=a" (ax), "=bcd" (res), "=bcd" (xx1), "=bcd" (xx2)
		      : "Rah" ((unsigned char) 0x4f)
		      : "cc", "memory");

      if (res)
	return __libi86_ret_really_set_errno (ax);
      return 0;
    }
}
