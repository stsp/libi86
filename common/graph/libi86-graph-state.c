/*
 * Copyright (c) 2021--2022 TK Chia
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

/*
 * Variable for the current state of graphics output, & routines to update
 * the state.
 */

#define _LIBI86_COMPILING_
#include <stdbool.h>
#include <stdint.h>
#include "dos.h"
#include "i86.h"
#include "libi86/internal/graph.h"

struct __libi86_graph_state_t __libi86_graph_state;

void
__libi86_graph_mode_changed (unsigned mode)
{
  unsigned char max_colr;

  switch (mode)
    {
    _LIBI86_CASE_SUPPORTED_2COLOR_GRAPHICS_MODES
      max_colr = 0x01;
      break;

    _LIBI86_CASE_SUPPORTED_4COLOR_GRAPHICS_MODES
      max_colr = 0x03;
      break;

    _LIBI86_CASE_SUPPORTED_16COLOR_GRAPHICS_MODES
      max_colr = 0x0f;
      break;

      /*
       * Video mode 0x10 is a special case.  Apparently, this is a 4-colour
       * mode for EGA with 64 KiB of graphics memory, & a 16-colour mode for
       * VGA or EGA with 256 KiB of graphics memory.
       */
    case _ERESCOLOR:
	{
	  uint8_t mem = (uint8_t) (__libi86_vid_get_ega_info () >> 16);
	  if (! mem)
	    max_colr = 0x03;
	  else
	    max_colr = 0x0f;
	}
      break;

    default:
      max_colr = 0xff;
    }

  __libi86_graph_state.max_colr = __libi86_graph_state.draw_colr = max_colr;
}
