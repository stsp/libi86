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
#include "dpmi.h"
#include "libi86/string.h"
#include "libi86/internal/dos.h"

/*
 * Under DPMI, determine if the buffer [BUF, BUF + COUNT) has a valid
 * corresponding address in real (or V86) mode.  NEED_WRITE says whether we
 * need write access.
 *
 * If the buffer has a valid real (or V86) mode address, fill *RM_SEG and
 * *RM_OFF with this address, and return 0.  If the buffer might be valid
 * but is not known to reside in conventional memory, return 1.  If there
 * are known problems with the permissions or segment limit, return 2.
 */

unsigned
__libi86_dpmi_pm_to_rm_buf (const void __far *buf, size_t count,
			    bool need_write, __libi86_segment_t *rm_seg,
			    size_t *rm_off)
{
  uint8_t szapc;
  descriptor desc;
  uint32_t base, lim, linear;
  uint16_t lwr_lim, upr_lim;

  __libi86_segment_t pm_sel = FP_SEG (buf);
  size_t pm_off = FP_OFF (buf);

  /*
   * Check that the buffer does not wrap around within its segment. (!)
   * FIXME: should this flag a more serious error?
   */
  if (count && pm_off + count - 1 < pm_off)
    return 2;

  /* Check that BUF's segment selector gives us the needed access. */
  if (need_write)
    __asm volatile ("verww %1; lahf" : "=Rah" (szapc) : "g" (pm_sel) : "cc");
  else
    __asm volatile ("verrw %1; lahf" : "=Rah" (szapc) : "g" (pm_sel) : "cc");
  if (! (szapc & 0x40))
    return 2;

  /* Try to get the underlying segment descriptor. */
  if (_DPMIGetDescriptor (pm_sel, &desc) != 0)
    return 1;

  /*
   * If COUNT is non-zero, get the minimum & maximum valid offsets within
   * the segment, & check that the buffer offsets fall in the range.
   */
  if (count)
    {
      lim = (uint32_t) desc.lim_16_19 << 16 | desc.lim_0_15;
      if (desc.xtype.page_gran)
	lim = lim << 12 | 0xfffUL;

      if (! desc.type.execute && desc.type.exp_down)
	{
	  if (lim >= 0xffffUL)
	    return 2;
	  lwr_lim = (uint16_t) lim + 1;
	  upr_lim = 0xffffU;
	}
      else
	{
	  lwr_lim = 0;
	  if (lim > 0xffffUL)
	    upr_lim = 0xffffU;
	  else
	    upr_lim = (uint16_t) lim;
	}

      if (pm_off < lwr_lim || pm_off + count - 1 > upr_lim)
	return 2;
    }

  /* Get the segment base.  Check that it is sane & in conventional memory. */
  if (desc.base_24_31)
    return 1;
  base = (uint32_t) desc.base_16_23 << 16 | desc.base_0_15;
  if (base > 0xfffffUL - pm_off - count)
    return 1;

  /*
   * Everything checks out.  Compute a linear address & turn it into a
   * normalized real mode address.
   */
  linear = base + pm_off;
  *rm_seg = (__libi86_segment_t) (linear >> 4);
  *rm_off = (size_t) (linear & 0xf);

  return 0;
}
