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

static unsigned char def_mode = 0xffU;

static void
__libi86_vid_def_mode_init (void)
{
  if (def_mode == 0xffU)
    {
      unsigned mode = __libi86_vid_state.mode_num;
      if ((mode & ~0x007fU) != 0)
	def_mode = _TEXTC80;
      else
	def_mode = (unsigned char) mode;
    }
}

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
      return __libi86_con_mode_changed ((unsigned) mode & 0x7fffU);
    }
  /*
   * If using VESA fails, try to retrieve the new video mode via the
   * traditional interface.
   */
  al = (uint8_t) __libi86_vid_int_0x10 (0x0f00U, 0, 0, 0);
  if (al != mode)
    return 0;
  return __libi86_con_mode_changed ((unsigned) al & 0x007fU);
}

static short
__libi86_setvideomode_svga (short mode)
{
  switch (mode)
    {
    case _URES256COLOR:
    case _VRES256COLOR:
    case _SVRES16COLOR:
    case _SVRES256COLOR:
    case _XRES16COLOR:
    case _XRES256COLOR:
    case _YRES16COLOR:
    case _YRES256COLOR:
    case _SVTEXTC80X60:
    case _SVTEXTC132X25:
    case _SVTEXTC132X43:
    case _SVTEXTC132X50:
    case _SVTEXTC132X60:
    case _MRES32KCOLOR:
    case _MRES64KCOLOR:
    case _MRESTRUECOLOR:
    case _VRES32KCOLOR:
    case _VRES64KCOLOR:
    case _VRESTRUECOLOR:
    case _SVRES32KCOLOR:
    case _SVRES64KCOLOR:
    case _SVRESTRUECOLOR:
    case _XRES32KCOLOR:
    case _XRES64KCOLOR:
    case _XRESTRUECOLOR:
    case _YRES32KCOLOR:
    case _YRES64KCOLOR:
    case _YRESTRUECOLOR:
    case _ZRES256COLOR:
    case _ZRES32KCOLOR:
    case _ZRES64KCOLOR:
    case _ZRESTRUECOLOR:
      if ((uint16_t) __libi86_vid_int_0x10 (0x4f02U, mode, 0, 0) != 0x004fU)
	return 0;
      return __libi86_con_mode_changed ((unsigned) mode);

    default:
      return 0;
    }
}

static short
__libi86_setvideomode_default (void)
{
  return __libi86_setvideomode_nonsvga (def_mode);
}

short
_setvideomode (short mode)
{
  __libi86_vid_state_init ();
  __libi86_vid_def_mode_init ();
  if (mode < (short) 0x0080)
    {
      if (mode >= (short) 0x0000)
	return __libi86_setvideomode_nonsvga (mode);
      else if (mode == _DEFAULTMODE)
	return __libi86_setvideomode_default ();
    }
  else
    return __libi86_setvideomode_svga (mode);

  return 0;
}
