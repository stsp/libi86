/*
 * Copyright (c) 2021 TK Chia
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
#include "graph.h"
#include "libi86/internal/graph.h"

extern unsigned __libi86_con_mode_changed (unsigned);

static short
__libi86_setvideomode_nonsvga (short mode)
{
  uint32_t bxax;
  uint16_t ax, bx;
  uint8_t al;
  /* Set the video mode. */
  __libi86_vid_int_0x10 (0x0000 | (uint8_t) mode, 0, 0, 0);
  /*
   * Try to retrieve the new video mode via VESA, & check it.  If we
   * successfully set the video mode, reset the text window & return the
   * text row count, else say 0.
   */
  bxax = __libi86_vid_int_0x10 (0x4f03U, 0, 0, 0);
  ax = (uint16_t) bxax;
  if (ax == 0x004fU)
    {
      bx = (uint16_t) (bxax >> 16);
      if (bx != mode)
	return 0;
      return __libi86_con_mode_changed (mode);
    }
  /*
   * If using VESA fails, try to retrieve the new video mode via the
   * traditional interface.
   */
  al = (uint8_t) __libi86_vid_int_0x10 (0x0f00U, 0, 0, 0);
  if (al != mode)
    return 0;
  return __libi86_con_mode_changed (mode);
}

static short
__libi86_setvideomode_svga (short mode)
{
  if ((uint16_t) __libi86_vid_int_0x10 (0x4f02U, mode, 0, 0) != 0x004fU)
    return 0;
  return __libi86_con_mode_changed ((unsigned) mode);
}

short
_setvideomode (short mode)
{
  __libi86_vid_state_init ();

  switch (mode)
    {
    case _DEFAULTMODE:
      return _setvideomode (__libi86_vid_state.startup_mode_num);

    _LIBI86_CASE_SUPPORTED_NONSVGA_MODES
      return __libi86_setvideomode_nonsvga (mode);

    _LIBI86_CASE_SUPPORTED_SVGA_MODES
      return __libi86_setvideomode_svga (mode);

    default:
      return 0;
    }
}
