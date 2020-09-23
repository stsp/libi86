/*
 * Copyright (c) 2020 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING3.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
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
  uint16_t sel;

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
}
