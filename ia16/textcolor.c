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

#define FG_MASK		(BLINK | 0x0f)

void
textcolor (int new_colour)
{
  if (__libi86_vid_state.graph_p)
    __libi86_vid_state.attribute = new_colour;
  else
    __libi86_vid_state.attribute = (__libi86_vid_state.attribute & ~FG_MASK)
				   | (new_colour & FG_MASK);
}
