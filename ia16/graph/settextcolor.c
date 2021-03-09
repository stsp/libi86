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
#include "graph.h"
#include "libi86/internal/graph.h"

#define BLINK		0x80
#define W_BLINK		0x10
#define FG_MASK		(BLINK | 0x0f)

short
_settextcolor (short pixval)
{
  unsigned char prev_val;

  if (__libi86_vid_state.graph_p)
    {
      prev_val = __libi86_vid_state.attribute;
      __libi86_vid_state.attribute = pixval;
    }
  else
    {
      unsigned char val = pixval & 0x0f;
      if ((pixval & W_BLINK) != 0)
	val |= BLINK;

      prev_val = __libi86_vid_state.attribute & 0x0f;
      if ((__libi86_vid_state.attribute & BLINK) != 0)
	prev_val |= W_BLINK;

      __libi86_vid_state.attribute = (__libi86_vid_state.attribute & ~FG_MASK)
				     | (val & FG_MASK);
    }

  return (short) prev_val;
}
