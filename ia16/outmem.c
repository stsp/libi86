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
#include "graph.h"
#include "libi86/internal/graph.h"

void
_outmem (const char __far *text, short length)
{
  unsigned char pg_no;
  unsigned char x1z, x2z, y1z, y2z, attr, x, y;
  struct __libi86_vid_rccoord_t pxy;
  struct rccoord coord;

  if (! length)
    return;

  /* Get our current text window & output text colour attribute. */
  x1z = __libi86_vid_state.x1z;
  y1z = __libi86_vid_state.y1z;
  x2z = __libi86_vid_state.x2z;
  y2z = __libi86_vid_state.y2z;
  attr = __libi86_vid_state.attribute;

  /* Get the current cursor position according to the BIOS. */
  pg_no = __libi86_vid_get_curr_pg ();
  pxy = __libi86_vid_get_rccoord (pg_no);
  x = pxy.col;
  y = pxy.row;

  /* If the cursor is outside the window, mentally move it inside. */
  if (x < x1z || x > x2z || y < y1z || y > y2z)
    {
      x = x1z;
      y = y1z;
    }

  /* Display the characters.  Scroll the window as necessary. */
  while (length-- != 0)
    {
      unsigned xx1, xx2, xx3;
      char ch = *text++;

      pxy.col = x;
      pxy.row = y;
      __libi86_vid_set_rccoord_only (pg_no, pxy);

      __asm volatile ("int $0x10" : "=a" (xx1), "=b" (xx2), "=c" (xx3)
				  : "Rah" ((unsigned char) 0x09), "Ral" (ch),
				    "1" ((uint16_t) pg_no << 8 | attr),
				    "2" (1U)
				  : "cc", "dx", "memory");
      if (x >= x2z)
	{
	  if (y < y2z)
	    ++y;
	  else
	    _scrolltextwindow (_GSCROLLUP);
	  x = x1z;
	}
      else
	++x;
    }

  /* Move the cursor to where it should now be. */
  pxy.col = x;
  pxy.row = y;
  __libi86_vid_go_rccoord (pg_no, pxy);
}
