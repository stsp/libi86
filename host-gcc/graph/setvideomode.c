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
#include <inttypes.h>
#include "graph.h"
#include "libi86/internal/graph.h"

short
_setvideomode (short mode)
{
  unsigned ax, bx, new_mode;

  switch (mode)
    {
    case _DEFAULTMODE:
      return _setvideomode (__libi86_vid_state.startup_mode_num);

    _LIBI86_CASE_SUPPORTED_NONSVGA_MODES
      /* Set the video mode. */
      __asm volatile ("int $0x10" : "=a" (ax)
				  : "0" (mode)
				  : "cc", "bx", "cx", "dx");
      /* Try to retrieve the new video mode via VESA, & check it. */
      __asm volatile ("int $0x10" : "=a" (ax), "=b" (new_mode)
				  : "0" (0x4f03U)
				  : "cc", "cx", "dx");
      /*
       * If using VESA fails, try to retrieve the new video mode via the
       * traditional interface.
       */
      if (ax != 0x004fU)
	{
	  __asm volatile ("int $0x10" : "=a" (ax)
				      : "Rah" ((uint8_t) 0x0f)
				      : "cc", "bx", "cx", "dx");
	  new_mode = (uint8_t) ax;
	}
      /*
       * If we successfully set the video mode, reset the text window &
       * return the text row count, else return 0.
       */
      if (new_mode != mode)
	return 0;
      return __libi86_con_mode_changed (mode);

    _LIBI86_CASE_SUPPORTED_SVGA_MODES
      __asm volatile ("int $0x10" : "=a" (ax), "=b" (bx)
				  : "0" (0x4f02U), "1" (mode)
				  : "cc", "cx", "dx");
      if (ax != 0x004fU)
	return 0;
      return __libi86_con_mode_changed (mode);

    default:
      return 0;
    }
}
