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
#include <stdbool.h>
#include <stdint.h>
#include "dos.h"
#include "i86.h"
#include "libi86/internal/graph.h"
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include "dpmi.h"
#endif

struct __libi86_graph_state_t __libi86_graph_state;

static bool
__libi86_graph_adapter_ega_p (void)
{
  uint32_t bxcx = __libi86_vid_get_ega_info ();

  uint8_t cl = (uint8_t) bxcx;
  uint16_t bx;

  if (cl > 0xb)
    return false;

  bx = (uint16_t) (bxcx >> 16);
  if ((bx & ~0x0301U) != 0)
    return false;

  return true;
}

static char
__libi86_graph_get_adapter_nonvga (unsigned mode)
{
  switch (mode)
    {
    _LIBI86_CASE_SUPPORTED_EGA_16COLOR_GRAPHICS_MODES
    case _ERESNOCOLOR:
    case _ERESCOLOR:
    case _VRES2COLOR:
      return _EGA;
    }

  if (__libi86_graph_adapter_ega_p ())
    return _EGA;

  switch (mode)
    {
    _LIBI86_CASE_SUPPORTED_CGA_4COLOR_GRAPHICS_MODES
    case _HRESBW:
      return _CGA;

    case _HERCMONO:
      return _HERCULES;

    default:
      return _MDPA;  /* FIXME: detect Hercules? */
    }
}

static char
__libi86_graph_get_adapter (unsigned mode)
{
  uint8_t code;

#ifdef __GNUC__
  uint16_t ax, bx;
  __asm volatile ("int $0x10" : "=a" (ax), "=b" (bx)
			      : "0" (0x1a00U)
			      : "cc", "cx", "dx", "memory");
  if ((uint8_t) ax != 0x1a)
    return __libi86_graph_get_adapter_nonvga (mode);
  code = (uint8_t) bx;
#else
  uint32_t res;
  res = __libi86_vid_int_0x10 (0x1a00U, 0, 0, 0);
  if ((uint8_t) res != 0x1a)
    return __libi86_graph_get_adapter_nonvga (mode);
  code = (uint8_t) (res >> 16);
#endif

  switch (code)
    {
    case 0x00:
      return _NODISPLAY;
    case 0x01:
      return _MDPA;
    case 0x02:
      return _CGA;
      /*
       * RBIL says "03h" is "reserved", but Open Watcom treats it as a type
       * of EGA...
       */
    case 0x03:
    case 0x04:
    case 0x05:
      return _EGA;
    case 0x07:
    case 0x08:
      return _VGA;
      break;
    case 0x0a:
    case 0x0b:
    case 0x0c:
      return _MCGA;
      break;
      /*
       * These display codes are interpreted thus by Open Watcom.  They are
       * undocumented in RBIL...
       */
    case 0x0d:
    case 0x0e:
    case 0x0f:
      return _HERCULES;
    case 0x10:
    case 0x11:
      return _SVGA;
    default:
      return _UNKNOWN;
    }
}

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
  __libi86_graph_state.adapter = __libi86_graph_get_adapter (mode);
  __libi86_graph_state.bk_colr = __libi86_vid_get_ega_pal_reg (0, 0);
}
