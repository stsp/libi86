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

/*
 * Variable for the current state of video output, & routines to update the
 * state.
 */

#define _LIBI86_COMPILING_
#include <stdint.h>
#include "dos.h"
#include "i86.h"
#include "libi86/internal/graph.h"

struct __libi86_vid_state_t __libi86_vid_state;

#ifdef __GNUC__
/*
 * Under GCC, if normvideo () is used, then this routine is overridden in
 * normvideo.c, to determine the colour attribute to use for normvideo ()
 * calls.
 */
# if defined __MEDIUM__ && __ia16__ - 0 < 20201106L
__attribute__ ((far_section))  /* bug workaround */
# endif
__attribute__ ((weak, noinline)) void
__libi86_vid_get_norm_attr (void)
{
}

/*
 * Again under GCC: if graphics output routines are used, then this routine
 * is overridden in libi86-graph-state.c .
 */
# if defined __MEDIUM__ && __ia16__ - 0 < 20201106L
__attribute__ ((far_section))  /* bug workaround */
# endif
__attribute__ ((weak, noinline)) void
__libi86_graph_mode_changed (unsigned mode)
{
}

static inline unsigned
__libi86_vid_get_mode (void)
{
  unsigned ax, mode;
  /*
   * Try to retrieve the current mode via VESA.  If VESA fails, use the
   * classical BIOS interface to get the video mode.
   */
  __asm volatile ("int $0x10" : "=a" (ax), "=b" (mode)
			      : "0" (0x4f03U)
			      : "cc", "cx", "dx");
  if (ax == 0x004fU)
    mode &= 0x7fffU;
  else
    {
      __asm volatile ("int $0x10" : "=a" (mode)
				  : "Rah" ((unsigned char) 0x0f)
				  : "cc", "bx", "cx", "dx");
      mode &= 0x007fU;
    }
  return mode;
}
#else  /* ! __GNUC__ */
static unsigned
__libi86_vid_get_mode (void)
{
  unsigned long res = __libi86_vid_int_0x10 (0x4f03U, 0, 0, 0);
  unsigned mode;
  if ((unsigned) res == 0x004fU)
    mode = (res >> 16) & 0x7fffU;
  else
    mode = __libi86_vid_int_0x10 (0x0f00U, 0, 0, 0) & 0x007fU;
  return mode;
}
#endif  /* ! __GNUC__ */

static bool
__libi86_vid_adapter_ega_p (void)
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
__libi86_vid_get_adapter_nonvga (unsigned mode)
{
  switch (mode)
    {
    _LIBI86_CASE_SUPPORTED_EGA_16COLOR_GRAPHICS_MODES
    case _ERESNOCOLOR:
    case _ERESCOLOR:
    case _VRES2COLOR:
      return _EGA;
    }

  if (__libi86_vid_adapter_ega_p ())
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
__libi86_vid_get_adapter (unsigned mode)
{
  uint8_t code;
#ifdef __GNUC__
  uint16_t ax, bx;
#else
  uint32_t res;
#endif

  switch (mode)
    {
    _LIBI86_CASE_SUPPORTED_SVGA_MODES
      return _SVGA;
    }

#ifdef __GNUC__
  __asm volatile ("int $0x10" : "=a" (ax), "=b" (bx)
			      : "0" (0x1a00U)
			      : "cc", "cx", "dx", "memory");
  if ((uint8_t) ax != 0x1a)
    return __libi86_vid_get_adapter_nonvga (mode);
  code = (uint8_t) bx;
#else
  res = __libi86_vid_int_0x10 (0x1a00U, 0, 0, 0);
  if ((uint8_t) res != 0x1a)
    return __libi86_vid_get_adapter_nonvga (mode);
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

static long
__libi86_vid_get_bk_colr (void)
{
  switch (__libi86_vid_state.adapter)
    {
    default:
      return 0;  /* ?!? */
    case _EGA:
      return (long) __libi86_vid_get_ega_pal_reg (0, 0);
    case _MCGA:
    case _VGA:
    case _SVGA:
      return __libi86_vid_get_vga_dac_reg (0);
    }
}

unsigned
__libi86_con_mode_changed (unsigned mode)
{
  unsigned char max_x, max_y;

  /* Record the mode number. */
  __libi86_vid_state.mode_num = mode;

  /* Record information about the active graphics adapter. */
  __libi86_vid_state.adapter = __libi86_vid_get_adapter (mode);

  /* Record the maximum (x, y) coordinates in character units. */
  max_x = __libi86_peekb_bios_ds (0x004aU) - 1;
  __libi86_vid_state.max_x = max_x;

  max_y = __libi86_peekb_bios_ds (0x0084U);
  if (! max_y)
    max_y = 24;
  __libi86_vid_state.max_y = max_y;

  /*
   * Decide if we are now in a text mode or a graphics mode, & whether the
   * cursor is (or should be) displayed.  If we are now in a graphics mode,
   * try to obtain the current background colour setting.
   */
  switch (mode)
    {
    _LIBI86_CASE_SUPPORTED_TEXT_MODES
      __libi86_vid_state.graph_p = 0;
      __libi86_vid_state.curs_p
	= ((__libi86_vid_get_curs_shape () & 0x2000) == 0);
      break;

    default:
      __libi86_vid_state.graph_p = 1;
      __libi86_vid_state.curs_p = 0;
      __libi86_vid_state.bk_colr = __libi86_vid_get_bk_colr ();
      __libi86_graph_mode_changed (mode);
    }

  /* Reset the colour attribute to use for text output. */
  __libi86_vid_state.attribute = 0x07;

  /*
   * Get the "normal" colour attribute to use for Borland normvideo () ---
   * but only if we need to.
   */
  __libi86_vid_get_norm_attr ();

  /* Reset the text window. */
  __libi86_vid_state.x1z = __libi86_vid_state.y1z = 0;
  __libi86_vid_state.x2z = max_x;
  __libi86_vid_state.y2z = max_y;

  return (unsigned) max_y + 1;
}

/*
 * Obtain information about the active video mode at startup.  If the video
 * mode is not a supported mode, then switch to one that is. :-)
 */
#ifdef __GNUC__
__attribute__ ((constructor (100))) static
#endif
void
__libi86_vid_state_init (void)
{
  unsigned mode;

#ifndef __GNUC__
  if (__libi86_vid_state.max_y)
    return;
#endif
  mode = __libi86_vid_get_mode ();
  switch (mode)
    {
    _LIBI86_CASE_SUPPORTED_MODES
      __libi86_con_mode_changed (mode);
      break;

    default:
      if (! _setvideomode (_TEXTC80))
	_setvideomode (_TEXTMONO);
    }

  __libi86_vid_state.startup_mode_num = __libi86_vid_state.mode_num;
}
