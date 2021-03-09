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
#include "conio.h"
#include "graph.h"
#include "libi86/internal/conio.h"
#include "libi86/internal/graph.h"

void
gettextinfo (struct text_info *r)
{
  struct rccoord coord = _gettextposition ();
  r->winleft	  = __libi86_vid_state.x1z + 1;
  r->wintop	  = __libi86_vid_state.y1z + 1;
  r->winright	  = __libi86_vid_state.x2z + 1;
  r->winbottom	  = __libi86_vid_state.y2z + 1;
  r->attribute	  = __libi86_vid_state.attribute;
  r->normattr	  = __libi86_vid_norm_attr;
  /*
   * FIXME?!?  There is no really good way to square SuperVGA mode numbers
   * with an 8-byte .curr_mode field.
   */
  if (__libi86_vid_state.mode_num < 0xff)
    r->currmode   = (unsigned char) __libi86_vid_state.mode_num;
  else
    r->currmode   = 0xff;
  r->screenheight = __libi86_vid_state.max_y + 1;
  r->screenwidth  = __libi86_vid_state.max_x + 1;
  r->curx	  = coord.col;
  r->cury	  = coord.row;
}
