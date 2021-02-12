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
#include "libi86/string.h"
#include "libi86/internal/dos.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

#ifdef __IA16_FEATURE_PROTECTED_MODE
/*
 * When under DPMI, we cannot simply set the DOS Disk Transfer Area (DTA) to
 * point to the user's `struct find_t', since it might not be in low memory
 * (i.e. below the 1 MiB mark).  We need to set up an area in low memory to
 * serve as the DTA.
 *
 * In fact, even the file specification for _dos_findfirst (...) must be
 * temporarily copied to low memory.
 */
struct find_t __far *
__libi86_dpmi_set_dta (void)
{
  static dpmi_dos_block dta_blk = { 0, 0 };
  rm_call_struct rmc;

  if (! dta_blk.pm)
    {
      dta_blk
	= _DPMIAllocateDOSMemoryBlock ((sizeof (struct find_t) + 15) / 16);
      if (! dta_blk.pm)
	{
	  errno = ENOMEM;
	  return NULL;
	}
    }

  rmc.ax = 0x1a00U;
  rmc.ds = dta_blk.rm;
  rmc.dx = 0;
  if (_DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc) != 0)
    {
      errno = EIO;
      return NULL;
    }

  return (struct find_t __far *) MK_FP (dta_blk.pm, 0);
}

static dpmi_dos_block
__libi86_dpmi_low_dup_str (const char *str)
{
  size_t size = strlen (str) + 1;
  dpmi_dos_block str_blk = _DPMIAllocateDOSMemoryBlock ((size + 15) / 16);

  if (! str_blk.pm)
    errno = ENOMEM;
  else
    _fmemcpy (MK_FP (str_blk.pm, 0), str, size);

  return str_blk;
}

static void
__libi86_dpmi_low_free_str (dpmi_dos_block str_blk)
{
  _DPMIFreeDOSMemoryBlock (str_blk.pm);
}
#endif

unsigned
_dos_findfirst (const char *path, unsigned attr, struct find_t *buf)
{
#ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    {
      struct find_t __far *dta = __libi86_dpmi_set_dta ();
      dpmi_dos_block path_blk;
      rm_call_struct rmc;
      int res;

      if (! dta)
	return errno;

      path_blk = __libi86_dpmi_low_dup_str (path);
      if (! path_blk.pm)
	return errno;

      rmc.ax = 0x4e00U;
      rmc.cx = attr;
      rmc.ds = path_blk.rm;
      rmc.dx = 0;
      res = _DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc) != 0;

      __libi86_dpmi_low_free_str (path_blk);
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
      unsigned ax, res, xx;

      __libi86_msdos_set_dta (buf);
      __asm volatile ("int $0x21; sbbw %1, %1"
		      : "=a" (ax), "=cd" (res), "=cd" (xx)
		      : "0" (0x4e00U), "c" (attr),
			"Rds" (FP_SEG (path)), "d" (FP_OFF (path))
		      : "cc", "bx", "memory");

      if (res)
	return __libi86_ret_really_set_errno (ax);
      return 0;
    }
}