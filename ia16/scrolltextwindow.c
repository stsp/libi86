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

#define _LIBI86_COMPILING_
#include <inttypes.h>
#include "libi86/internal/graph.h"

void
_scrolltextwindow (short rows)
{
  unsigned char x1z, x2z, y1z, y2z;
  short hm1;
  bool scroll_up_p;

  if (! rows)
    return;

  x1z = __libi86_vid_state.x1z;
  y1z = __libi86_vid_state.y1z;
  x2z = __libi86_vid_state.x2z;
  y2z = __libi86_vid_state.y2z;

  hm1 = y2z - y1z;  /* window height minus 1 */
  if (rows > 0)
    {
      scroll_up_p = true;
      if (rows > hm1)
	rows = 0;
    }
  else
    {
      scroll_up_p = false;
      if (rows < -hm1)
	rows = 0;
      else
	rows = -rows;
    }

  __libi86_vid_scroll (x1z, y1z, x2z, y2z, rows, scroll_up_p);
}
