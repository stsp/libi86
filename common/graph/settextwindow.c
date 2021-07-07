/*
 * Copyright (c) 2020--2021 TK Chia
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
#include "libi86/internal/graph.h"

void
_settextwindow (short y1, short x1, short y2, short x2)
{
  short x1z, y1z, x2z, y2z;

#ifndef __GNUC__
  __libi86_vid_state_init ();
#endif

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
