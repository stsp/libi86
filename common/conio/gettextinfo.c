/*
 * Copyright (c) 2020 TK Chia
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the developer(s) nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
