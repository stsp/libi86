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

static void
__libi86_vid_do_xor_sw_cursor (void)
{
  unsigned char pg_no = __libi86_vid_get_curr_pg ();
  struct __libi86_vid_rccoord_t pxy
    = __libi86_vid_get_and_adjust_rccoord (pg_no);
  unsigned char cell_wd = __libi86_vid_get_cell_width (),
		attr = __libi86_vid_state.attribute;
  unsigned gy = ((unsigned) pxy.y + 1) * __libi86_vid_get_cell_height () - 1,
	   gx = (unsigned) pxy.x * cell_wd, left_x = cell_wd;

  switch (__libi86_vid_state.mode_num)
    {
    _LIBI86_CASE_SUPPORTED_SUB256COLOR_GRAPHICS_MODES
       while (left_x-- != 0)
	{
	  /*
	   * RBIL: "if bit 7 set, value is XOR'ed onto screen except in
	   * 256-color modes".
	   */
	  __libi86_vid_plot_pixel (gx, gy, attr | 1 << 7, pg_no);
	  ++gx;
	}
      break;

    _LIBI86_CASE_SUPPORTED_256COLOR_GRAPHICS_MODES
       while (left_x-- != 0)
	{
	  uint8_t was_pixel = __libi86_vid_get_pixel (gx, gy, pg_no);
	  __libi86_vid_plot_pixel (gx, gy, attr ^ was_pixel, pg_no);
	  ++gx;
	}
      break;

    default:
      /*
       * For now, do not handle graphics modes that allow lots of colours
       * (32K, 64K, etc.).  That is, do not display any software cursor...
       */
      ;
    }
}

void
__libi86_vid_xor_sw_cursor (void)
{
  if (__libi86_vid_state.curs_p)
    __libi86_vid_do_xor_sw_cursor ();
}

short
__libi86_displaycursor_on (short curs_mode)
{
  if (__libi86_vid_state.curs_p)
    return _GCURSORON;

  __libi86_vid_state.curs_p = 1;
  if (! __libi86_vid_state.graph_p)
    __libi86_vid_set_curs_shape (__libi86_vid_get_curs_shape () & ~0x2000U);
  else
    __libi86_vid_resume_sw_cursor ();

  return _GCURSOROFF;
}
