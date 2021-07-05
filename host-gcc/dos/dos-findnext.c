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

      rmc.ss = rmc.sp = rmc.flags = 0;
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
