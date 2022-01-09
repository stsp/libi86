/*
 * Copyright (c) 2022 TK Chia
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
#include <stdint.h>
#include "graph.h"
#include "libi86/internal/graph.h"

#define BLINK		0x80
#define FG_MASK		(BLINK | 0x0f)
#define BG_SHIFT	4

long
__libi86_setbkcolor_indexed (long pixval)
{
  long prev_val;

#ifndef __GNUC__
  __libi86_vid_state_init ();
#endif

  if (! __libi86_vid_state.graph_p)
    {
      prev_val = (__libi86_vid_state.attribute & ~FG_MASK) >> BG_SHIFT;
      __libi86_vid_state.attribute = (__libi86_vid_state.attribute & FG_MASK)
				     | ((unsigned char) pixval << BG_SHIFT
					& ~FG_MASK);
    }
  else
    {
      prev_val = __libi86_graph_state.overscan_colr;
#ifdef __GNUC__
      unsigned ax, bx;
      __asm volatile ("int {$}0x10" : "=a" (ax), "=b" (bx)
				    : "Rah" ((uint8_t) 0x0b),
				      "1" ((uint16_t) (uint8_t) pixval)
				    : "cc", "cx", "dx", "memory");
#else
      __libi86_vid_int_0x10 (0x0b00U, (uint16_t) (uint8_t) pixval, 0, 0);
#endif
      __libi86_graph_state.overscan_colr = (uint8_t) pixval;
    }

  return prev_val;
}
