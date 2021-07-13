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

#define _BORLANDC_SOURCE
#define _LIBI86_COMPILING_
#include <stdint.h>
#include "conio.h"
#include "libi86/internal/graph.h"

unsigned char __libi86_vid_norm_attr = 0x07;

void
normvideo (void)
{
#ifndef __GNUC__
  __libi86_vid_state_init ();
#endif
  __libi86_vid_state.attribute = __libi86_vid_norm_attr;
}

void
__libi86_vid_get_norm_attr (void)
{
  /*
   * If normvideo () is used, then __libi86_con_mode_changed () calls this
   * routine to determine the "normal" character colour, for normvideo (). 
   * For text modes, this is the colour attribute of the character cell at
   * the current cursor position.
   *
   * Watcom <graph.h> does not use this attribute.
   */
  unsigned char ch;
  unsigned bx;
  if (__libi86_vid_state.graph_p)
    __libi86_vid_norm_attr = 0x07;
  else
#ifdef __GNUC__
    __asm volatile ("int $0x10" : "=Rah" (__libi86_vid_norm_attr), "=Ral" (ch),
				  "=b" (bx)
				: "0" ((unsigned char) 0x08),
				  "2" ((unsigned) __libi86_vid_get_curr_pg ()
				       << 8)
				: "cc", "cx", "dx");
#else
  __libi86_vid_norm_attr
    = (uint8_t) __libi86_vid_int_0x10
		  (0x0800U, (unsigned) __libi86_vid_get_curr_pg () << 8, 0, 0);
#endif
}
