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

struct rccoord
_gettextposition (void)
{
  unsigned char x1z, x2z, y1z, y2z;
  struct __libi86_vid_rccoord_t pxy;
  struct rccoord coord;

  pxy = __libi86_vid_get_rccoord (__libi86_vid_get_curr_pg ());

  x1z = __libi86_vid_state.x1z;
  y1z = __libi86_vid_state.y1z;
  x2z = __libi86_vid_state.x2z;
  y2z = __libi86_vid_state.y2z;

  if (pxy.col < x1z || pxy.col > x2z || pxy.row < y1z || pxy.row > y2z)
    {
      coord.row = coord.col = 1;
    }
  else
    {
      coord.row = (short) pxy.row - y1z + 1;
      coord.col = (short) pxy.col - x1z + 1;
    }

  return coord;
}
