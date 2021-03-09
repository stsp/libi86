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
#include "libi86/internal/graph.h"

void
_settextwindow (short y1, short x1, short y2, short x2)
{
  short x1z, y1z, x2z, y2z;

  if (x1 < 1 || y1 < 1 || x1 > x2 || y1 > y2)
    return;

  x1z = x1 - 1;
  y1z = y1 - 1;
  x2z = x2 - 1;
  y2z = y2 - 1;

  if (x2z > __libi86_vid_state.max_x || y2z > __libi86_vid_state.max_y)
    return;

  __libi86_vid_state.x1z = (unsigned char) x1z;
  __libi86_vid_state.y1z = (unsigned char) y1z;
  __libi86_vid_state.x2z = (unsigned char) x2z;
  __libi86_vid_state.y2z = (unsigned char) y2z;

  _settextposition (1, 1);
}
