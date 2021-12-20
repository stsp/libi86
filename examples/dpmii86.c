/*
 * Copyright (c) 2020--2021 TK Chia
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

/*
 * Simple test program which, when run under a DPMI environment, will dump
 * some information about the DPMI implementation.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <dpmi.h>
#include <i86.h>
#include <libi86/stdlib.h>

static uint16_t env_sel = 0;

static uint16_t
get_cs (void)
{
  unsigned cs;
  __asm volatile ("movw %%cs, %0" : "=r" (cs));
  return cs;
}

static uint16_t
get_ss (void)
{
  unsigned ss;
  __asm volatile ("movw %%ss, %0" : "=r" (ss));
  return ss;
}

static void
dump_sel_info (uint16_t sel)
{
  uint16_t rpl = 0;
  descriptor desc;
  uint16_t other_sel;

  memset (&desc, 0, sizeof desc);

  while (_DPMIGetDescriptor (sel | rpl, &desc) != 0)
    {
      if (rpl == 3)
	return;
      ++rpl;
    }

  if (! desc.type.present)
    return;

  printf ("  0x%04x 0x%02x%02x%04x 0x%x%04x "
	  "%u   %u %u  %u   %u   %u %u %u %u %u   %u",
	  sel | rpl,
	  (unsigned) desc.base_24_31, (unsigned) desc.base_16_23,
	  desc.base_0_15,
	  (unsigned) desc.lim_16_19, desc.lim_0_15,
	  desc.type.present, desc.type.dpl, desc.type.mustbe_1,
	  desc.type.execute, desc.type.exp_down, desc.type.rdwr,
	  desc.type.accessed,
	  desc.xtype.page_gran, desc.xtype.use32, desc.xtype.mustbe_0,
	  desc.xtype.useravail);

  if ((get_cs () & ~3u) == sel)
    printf (" * cs = 0x%x", get_cs ());
  else if ((get_ss () & ~3u) == sel)
    printf (" * ss = 0x%x", get_ss ());
  else if ((_psp & ~3u) == sel)
    printf (" * psp = 0x%x", _psp);
  else if ((env_sel & ~3u) == sel)
    printf (" * env = 0x%x", env_sel);

  putchar ('\n');
}

int
main (void)
{
  uint16_t sel, caps1, caps2, caps3;
  dpmi_host_info host;

  if (__DPMI_hosted () != 1)
    {
      printf ("Not running under DPMI!\n");
      return 1;
    }

  env_sel = *(const uint16_t __far *) MK_FP (_psp, 0x2c);

  printf ("GDT descriptors present:\n"
	  "  sel#   base       lim     P DPL 1 Ex D/C R/W A G D 0 Avl\n");

  sel = 0;
  do
    {
      sel += 8;
      dump_sel_info (sel);
    }
  while (sel != 0);

  printf ("\n"
	  "LDT descriptors present:\n"
	  "  sel#   base       lim     P DPL 1 Ex D/C R/W A G D 0 Avl\n");

  sel = 4;
  do
    {
      sel += 8;
      dump_sel_info (sel);
    }
  while (sel != 4);

  if (_DPMIGetCapabilities (&caps1, &caps2, &caps3, &host) != 0)
    printf ("\n"
	    "Cannot get DPMI 1.0 capabilities (if any)\n");
  else
    {
      size_t len = strlen (host.vendor);
      printf ("\n"
	      "DPMI 1.0 capabilities: 0x%04x 0x%04x 0x%04x\n"
	      "  host version: %u.%u\n"
	      "  vendor: %.64s\n",
	      caps1, caps2, caps3,
	      host.ver_major, host.ver_minor, host.vendor);
      if (len > 64)
	printf ("          %s\n", host.vendor + 64);
    }

  return 0;
}
