/*
 * Copyright (c) 2020 TK Chia
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

/*
 * Initialize a segment (or protected mode selector) reference to the BIOS
 * data area.
 */

#define _LIBI86_COMPILING_
#include <stdint.h>
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include <stdlib.h>
# include "dpmi.h"
#endif

uint16_t __libi86_bios_ds = 0x0040u;

#ifdef __IA16_FEATURE_PROTECTED_MODE
/*
 * Note: this constructor must run before __libi86_con_mode_cache is
 * initialized.
 */
__attribute__ ((constructor (98))) static void
__libi86_bios_ds_init (void)
{
  if (__DPMI_hosted () == 1)
    {
      int32_t res = _DPMISegmentToDescriptor (0x0040u);
      if (res < 0 || res > (int32_t) 0xffffL)
	abort ();
      __libi86_bios_ds = (uint16_t) res;
    }
}
#endif
