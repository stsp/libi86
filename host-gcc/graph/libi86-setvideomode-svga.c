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

/* Internal implementation of _setvideomode (.) for SuperVGA video modes. */

#define _LIBI86_COMPILING_
#include <stdint.h>
#include <stdlib.h>
#include "dos.h"
#include "i86.h"
#include "libi86/internal/graph.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

short
__libi86_setvideomode_svga (short mode)
{
  unsigned ax;

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
      __asm volatile ("int $0x10" : "=a" (ax), "=b" (mode)
				  : "0" (0x4f02U)
				  : "cc", "cx", "dx");
      if (ax != 0x004fU)
	return 0;
      return __libi86_con_mode_changed (mode);

    default:
      return 0;
    }
}
