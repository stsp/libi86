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
_clearscreen (short area)
{
  switch (area)
    {
    case _GWINDOW:
      __libi86_vid_scroll (__libi86_vid_state.x1z, __libi86_vid_state.y1z,
			   __libi86_vid_state.x2z, __libi86_vid_state.y2z,
			   0, true);
      _settextposition (1, 1);
      break;

    case _GVIEWPORT:
      /*
       * libi86 does not really support specifying a graphics viewport yet,
       * but we can kind of fake a default viewport.
       *
       * Text modes have no graphics viewport at all, so in text modes,
       * _clearscreen (_GVIEWPORT) should do nothing.  In graphics modes,
       * though, _clearscreen (_GVIEWPORT) should clear the whole screen,
       * but not reset the cursor position.
       */
      if (__libi86_vid_state.graph_p)
	__libi86_vid_scroll (0, 0,
			   __libi86_vid_state.max_x, __libi86_vid_state.max_y,
			   0, true);
      else
	return;

    default:
      __libi86_vid_scroll (0, 0,
			   __libi86_vid_state.max_x, __libi86_vid_state.max_y,
			   0, true);
      _settextposition (1, 1);
    }
}
