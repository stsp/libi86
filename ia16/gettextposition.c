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
#include "libi86/internal/conio.h"

struct rccoord
_gettextposition (void)
{
  unsigned char x1z, x2z, y1z, y2z;
  uint16_t raw_pos;
  uint8_t raw_y, raw_x, pg_no = bios_data_byte (0x0062);
  struct rccoord coord;

  raw_pos = bios_data_word (0x0050 + 2 * pg_no);
  raw_x = (raw_pos >> 8);
  raw_y = (uint16_t) (uint8_t) raw_pos;

  x1z = __libi86_con_vid_state.x1z;
  y1z = __libi86_con_vid_state.y1z;
  x2z = __libi86_con_vid_state.x2z;
  y2z = __libi86_con_vid_state.y2z;

  if (raw_x < x1z || raw_x > x2z || raw_y < y1z || raw_y > y2z)
    {
      coord.row = coord.col = 1;
    }
  else
    {
      coord.row = (short) raw_y - y1z + 1;
      coord.col = (short) raw_x - x1z + 1;
    }

  return coord;
}
