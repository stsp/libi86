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

#define _BORLANDC_SOURCE
#define _LIBI86_COMPILING_
#include <inttypes.h>
#include "conio.h"
#include "libi86/internal/graph.h"

unsigned char __libi86_vid_norm_attr = 0x07;

void
normvideo (void)
{
  __libi86_vid_state.attribute = __libi86_vid_norm_attr;
}

void
__libi86_vid_get_norm_attr (void)
{
  /*
   * If normvideo () is used, then __libi86_con_mode_changed () calls this
   * routine to determine the "normal" character colour, for normvideo (). 
   * For text modes, this is the colour attribute of the character cell at
   * the current cursor position.
   *
   * Watcom <graph.h> does not use this attribute.
   */
  unsigned char ch;
  unsigned bx;
  if (__libi86_vid_state.graph_p)
    __libi86_vid_norm_attr = 0x07;
  else
    __asm volatile ("int $0x10" : "=Rah" (__libi86_vid_norm_attr), "=Ral" (ch),
				  "=b" (bx)
				: "0" ((unsigned char) 0x08),
				  "2" ((unsigned) __libi86_vid_get_curr_pg ()
				       << 8)
				: "cc", "cx", "dx");
}
