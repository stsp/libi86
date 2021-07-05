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
#include "libi86/string.h"
#include "libi86/internal/dos.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

unsigned
_dos_read (int fd, void __far *buf, unsigned count, unsigned *bytes)
{
#ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    {
      _LIBI86_SEG_SELECTOR ds;
      size_t dx;
      rm_call_struct rmc;
      unsigned res;

      memset (&rmc, 0, sizeof rmc);
      rmc.ax = 0x3f00U;
      rmc.bx = fd;
      rmc.cx = count;

      switch (__libi86_dpmi_pm_to_rm_buf (buf, count, true, &ds, &dx))
	{
	case 0:
	  {
	    rmc.ds = ds;
	    rmc.dx = dx;
	    res = _DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc);

	    if (res)
	      res = EIO;
	    else if ((rmc.flags & 1) != 0)
	      res = rmc.ax;
	  }
	  break;

	case 1:
	  {
	    dpmi_dos_block buf_blk = _DPMIAllocateDOSMemoryBlock
				       ((count ? count - 1 : 0) / 0x10 + 1);

	    if (! buf_blk.pm)
	      return __libi86_ret_really_set_errno (ENOMEM);

	    rmc.ds = buf_blk.rm;
	    rmc.dx = 0;
	    res = _DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc);

	    if (res)
	      res = EIO;
	    else if ((rmc.flags & 1) != 0)
	      res = rmc.ax;

	    if (! res)
	      _fmemcpy (buf, MK_FP (buf_blk.pm, 0), rmc.ax);

	    _DPMIFreeDOSMemoryBlock (buf_blk.pm);
	  }
	  break;

	default:
	  abort ();
	}

      if (res)
	return __libi86_ret_really_set_errno (res);

      *bytes = rmc.ax;
      return 0;
    }
  else
#endif
    {
      unsigned ax, res;

      __asm volatile ("int $0x21; sbbw %1, %1"
		      : "=a" (ax), "=r" (res)
		      : "Rah" ((unsigned char) 0x3f), "b" (fd), "c" (count),
			"Rds" (FP_SEG (buf)), "d" (FP_OFF (buf))
		      : "cc", "memory");

      if (res)
	return __libi86_ret_really_set_errno (ax);

      *bytes = ax;
      return 0;
    }
}
