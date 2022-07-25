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
#include <stdlib.h>
#include <string.h>
#include "dos.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

__libi86_fpv_t
_getsysvars (void)
{
#ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    {
      static volatile __libi86_segment_t cached_seg_pm = 0;
      static volatile uint16_t cached_seg_rm = 0;
      rm_call_struct rmc;
      int res;
      uint16_t off, seg_rm;
      __libi86_segment_t seg_pm;
      int32_t res32;

      /* also set rmc.es = rmc.bx = rmc.ss = rmc.sp := 0 */
      memset (&rmc, 0, sizeof (rmc));
      rmc.ax = 0x5200U;
      res = _DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc);

      if (res != 0)
	abort ();

      if (! rmc.bx && ! rmc.es)
	return __libi86_fnullptr;

      off = rmc.bx;
      seg_rm = rmc.es;

      seg_pm = cached_seg_pm;
      if (! seg_pm || seg_rm != cached_seg_rm)
	{
	  res32 = _DPMISegmentToDescriptor (seg_rm);
	  if (res32 < 0)
	    abort ();

	  seg_pm = (__libi86_segment_t) res32;
	  cached_seg_pm = 0;
	  cached_seg_rm = seg_rm;
	  cached_seg_pm = seg_pm;
	}

      return MK_FP (seg_pm, off);
    }
  else
#endif
    {
      unsigned short off, xx;
      __segment seg;
      __asm volatile ("int $0x21" : "=a" (xx), "=e" (seg), "=b" (off)
				  : "0" (0x5200),
				    "1" (__builtin_ia16_selector (0)), "2" (0)
				  : "cx", "dx", "cc");
      return MK_FP (seg, off);
    }
}
