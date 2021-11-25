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
#include <stdlib.h>
#include <string.h>
#include "dos.h"
#include "libi86/stdlib.h"
#include "libi86/internal/dos-dbcs.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif
#include <stdio.h>

_dos_dbcs_lead_table_t
__libi86_msdos_get_dbcs_lead_table (void)
{
#ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    {
      rm_call_struct rmc;
      descriptor desc;
      int res;
      uint16_t off, seg_rm, seg_pm;
      int32_t res32;
      uint32_t seg_linear;
      _dos_dbcs_lead_table_t lt;

      memset (&rmc, 0, sizeof (rmc));  /* also set rmc.ss = rmc.sp := 0 */
      rmc.flags = 1;  /* pre-set carry flag */
      rmc.ax = 0x6300U;
      rmc.si = 0xffffU;
      res = _DPMISimulateRealModeInterrupt (0x21, 0, 0, &rmc);

      if (res != 0)
	abort ();

      if (_osmajor < 3)
	{
	  if ((rmc.flags & 1) != 0)
	    return _null_dbcs_lt;
	}
      else if ((uint8_t) rmc.ax != 0)
	return _null_dbcs_lt;

      off = rmc.si;
      seg_rm = rmc.ds;

      if (off == 0xffffU)
	return _null_dbcs_lt;

      res32 = _DPMIAllocateLDTDescriptors (1);
      if (res32 < 0)
	abort ();

      seg_pm = (uint16_t) res32;

      if (_DPMIGetDescriptor (seg_pm, &desc) != 0)
	abort ();

      seg_linear = (uint32_t) seg_rm * 0x10;
      desc.lim_0_15 = 0xffffU;
      desc.base_0_15 = (uint16_t) seg_linear;
      desc.base_16_23 = (uint8_t) (seg_linear >> 16);
      desc.type.rdwr = desc.type.exp_down = desc.type.execute = 0;
      desc.lim_16_19 = 0;
      desc.xtype.use32 = desc.xtype.page_gran = 0;

      if (_DPMISetDescriptor (seg_pm, &desc) != 0)
	abort ();

      lt = MK_FP (seg_pm, off);
      if (! *lt)
	{
	  _DPMIFreeLDTDescriptor (seg_pm);
	  return _null_dbcs_lt;
	}

      return lt;
    }
  else
#endif
    {
      union REGS r;
      struct SREGS sr;
      unsigned short off, seg;
      _dos_dbcs_lead_table_t lt;

      memset (&r, 0, sizeof (r));
      r.w.cflag = 1;  /* pre-set carry flag */
      r.w.ax = 0x6300U;
      r.w.si = 0xffffU;
      memset (&sr, 0, sizeof (sr));
      intdosx (&r, &r, &sr);

      if (_osmajor < 3)
	{
	  if (r.w.cflag)
	    return _null_dbcs_lt;
	}
      else if (r.h.al != 0)
	return _null_dbcs_lt;

      off = r.w.si;
      seg = sr.ds;

      if (off == 0xffffU)
	return _null_dbcs_lt;

      lt = MK_FP (seg, off);
      if (! *lt)
	return _null_dbcs_lt;

      return lt;
    }
}

#ifndef _LIBI86_INTERNAL_HAVE__DOS_GET_DBCS_LEAD_TABLE
_LIBI86_WEAK_ALIAS (__libi86_msdos_get_dbcs_lead_table) _dos_dbcs_lead_table_t
_dos_get_dbcs_lead_table (void);
#endif
