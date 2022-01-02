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
#include <stdint.h>
#include "graph.h"
#include "libi86/internal/graph.h"

#define BLINK		0x80
#define W_BLINK		0x10
#define FG_MASK		(BLINK | 0x0f)

grcolor
_settextcolor (grcolor pixval)
{
  unsigned char prev_val;

#ifndef __GNUC__
  __libi86_vid_state_init ();
#endif

  if (__libi86_vid_state.graph_p)
    {
      prev_val = __libi86_vid_state.attribute;
      __libi86_vid_state.attribute = pixval;
    }
  else
    {
      unsigned char val = pixval & 0x0f;
      if ((pixval & W_BLINK) != 0)
	val |= BLINK;

      prev_val = __libi86_vid_state.attribute & 0x0f;
      if ((__libi86_vid_state.attribute & BLINK) != 0)
	prev_val |= W_BLINK;

      __libi86_vid_state.attribute = (__libi86_vid_state.attribute & ~FG_MASK)
				     | (val & FG_MASK);
    }

  return (short) prev_val;
}
