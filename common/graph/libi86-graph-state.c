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

/*
 * Variable for the current state of graphics output, & routines to update
 * the state.
 */

#define _LIBI86_COMPILING_
#include <stdint.h>
#include "dos.h"
#include "i86.h"
#include "libi86/internal/graph.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

struct __libi86_graph_state_t __libi86_graph_state;

void
__libi86_graph_mode_changed (unsigned mode)
{
  unsigned char max_colr, overscan_colr;
#ifdef __GNUC__
  unsigned ax, bx;
#endif

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
       * EGA or VGA with 256 KiB of graphics memory.
       */
    case _ERESCOLOR:
	{
	  uint8_t mem;
#ifdef __GNUC__
	  __asm volatile ("int $0x10" : "=a" (ax), "=b" (bx)
				      : "Rah" ((uint8_t) 0x12), "1" (0x0010U)
				      : "cc", "cx", "dx", "memory");
	  mem = (uint8_t) bx;
#else
	  mem = (uint8_t) __libi86_vid_int_0x10 (0x1200U, 0x0010U, 0, 0) >> 16;
#endif
	  if (! mem)
	    max_colr = 0x03;
	  else
	    max_colr = 0x0f;
	}
      break;

    default:
      max_colr = 0xff;
    }

#ifdef __GNUC__
  __asm volatile ("int $0x10" : "=a" (ax), "=b" (bx)
			      : "0" (0x1008U), "1" (0xffffU)
			      : "cc", "cx", "dx", "memory");
  overscan_colr = bx >> 8;
#else
  overscan_colr =__libi86_vid_int_0x10 (0x1008U, 0xffffU, 0, 0) >> 24;
#endif
  if (overscan_colr == 0xff)
    overscan_colr = 0x00;  /* FIXME? */

  __libi86_graph_state.max_colr = __libi86_graph_state.draw_colr = max_colr;
  __libi86_graph_state.overscan_colr = overscan_colr;
}
