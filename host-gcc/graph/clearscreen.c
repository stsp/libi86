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
      return;

    default:
      __libi86_vid_scroll (0, 0,
			   __libi86_vid_state.max_x, __libi86_vid_state.max_y,
			   0, true);
      _settextposition (1, 1);
    }
}
