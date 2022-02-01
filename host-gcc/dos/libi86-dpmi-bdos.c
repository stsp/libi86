/*
 * Copyright (c) 2022 TK Chia
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
#include "libi86/string.h"
#include "libi86/internal/dos.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

#ifdef __IA16_FEATURE_PROTECTED_MODE
__libi86_bdos_res_t
__libi86_bdos_dsdxsz_al_cx (unsigned char dos_func, const char *dsdx,
			    unsigned char al, unsigned cx)
{
  __libi86_bdos_res_t res = { EIO, -1 };

  if (__DPMI_hosted () == 1)
    {
      __libi86_segment_t rm_ds;
      size_t count = strlen (dsdx) + 1, rm_dx;
      dpmi_dos_block dsdx_blk;
      rm_call_struct rmc;
      int rmi_res;

      rmc.ss = rmc.sp = rmc.flags = 0;
      rmc.ax = (unsigned) dos_func << 8 | al;
      rmc.cx = cx;

      switch (__libi86_dpmi_pm_to_rm_buf (dsdx, count, false, &rm_ds, &rm_dx))
	{
	case 0:
	  rmc.ds = rm_ds;
	  rmc.dx = rm_dx;
	  rmi_res = _DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc);
	  break;

	case 1:
	  dsdx_blk
	    = _DPMIAllocateDOSMemoryBlock ((count ? count - 1 : 0) / 0x10 + 1);
	  if (! dsdx_blk.pm)
	    {
	      errno = res.ax = ENOMEM;
	      return res;
	    }

	  _fmemcpy (MK_FP (dsdx_blk.pm, 0), dsdx, count);

	  rmc.ds = dsdx_blk.rm;
	  rmc.dx = 0;
	  rmi_res = _DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc);

	  _DPMIFreeDOSMemoryBlock (dsdx_blk.pm);
	  break;

	default:
	  abort ();
	}

      if (rmi_res != 0)
	errno = EIO;
      else if ((rmc.flags & 1) != 0)
	errno = res.ax = rmc.ax;
      else
	{
	  res.carry = 0;
	  res.ax = rmc.ax;
	}
    }
  else
    {
      unsigned xx1, xx2;
      __asm volatile ("int $0x21; sbbw %1, %1"
		      : "=a" (res.ax), "=bcd" (res.carry),
			"=bcd" (xx1), "=bcd" (xx2)
		      : "Rah" (dos_func), "Ral" (al), "c" (cx),
			"Rds" (FP_SEG (dsdx)), "d" (FP_OFF (dsdx))
		      : "cc", "memory");
      if (res.carry)
	errno = res.ax;
    }

  return res;
}
#endif  /* __IA16_FEATURE_PROTECTED_MODE */
