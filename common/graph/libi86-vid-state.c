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

static inline unsigned char
__libi86_vid_get_overscan (void)
{
  unsigned ax, bx;
  __asm volatile ("int $0x10" : "=a" (ax), "=b" (bx)
			      : "0" (0x1008U), "1" (0)
			      : "cc", "cx", "dx");
  return (unsigned char) (bx >> 8);
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
extern void __libi86_vid_get_norm_attr (void);

static unsigned char
__libi86_vid_get_overscan (void)
{
  return (unsigned char) (__libi86_vid_int_0x10 (0x1008U, 0, 0, 0) >> 24);
}

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

#ifdef __GNUC__
__attribute__ ((regparmcall))
#endif
unsigned
__libi86_con_mode_changed (unsigned mode)
{
  unsigned char max_x, max_y, ch;
  uint8_t mode_ctl_reg;

  /* Record the mode number. */
  __libi86_vid_state.mode_num = mode;

  /* Record the maximum (x, y) coordinates in character units. */
  max_x = __libi86_peekb_bios_ds (0x004aU) - 1;
  __libi86_vid_state.max_x = max_x;

  max_y = __libi86_peekb_bios_ds (0x0084U);
  if (! max_y)
    max_y = 24;
  __libi86_vid_state.max_y = max_y;

  /*
   * Decide if we are now in a text mode or a graphics mode.
   *
   * If we already know about the mode number, then this is easy.
   *
   * Otherwise, try to guess using the stored "mode control register" value
   * in the BIOS data area (FIXME?).
   */
  switch (mode)
    {
    case _TEXTBW40:
    case _TEXTC40:
    case _TEXTBW80:
    case _TEXTC80:
    case _TEXTMONO:
    case _SVTEXTC80X60:
    case _SVTEXTC132X25:
    case _SVTEXTC132X43:
    case _SVTEXTC132X50:
    case _SVTEXTC132X60:
      __libi86_vid_state.graph_p = 0;
      break;

    case _MRES4COLOR:
    case _MRESNOCOLOR:
    case _HRESBW:
    case _HERCMONO:
    case _MRES16COLOR:
    case _HRES16COLOR:
    case _ERESNOCOLOR:
    case _ERESCOLOR:
    case _VRES2COLOR:
    case _VRES16COLOR:
    case _MRES256COLOR:
    case _URES256COLOR:
    case _VRES256COLOR:
    case _SVRES16COLOR:
    case _SVRES256COLOR:
    case _XRES16COLOR:
    case _XRES256COLOR:
    case _YRES16COLOR:
    case _YRES256COLOR:
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
      __libi86_vid_state.graph_p = 1;
      break;

    default:
      mode_ctl_reg = __libi86_peekb_bios_ds (0x0065U);
      __libi86_vid_state.graph_p = ((mode_ctl_reg & 0x02) != 0);
    }

  /* Reset the colour attribute to use for text output. */
  __libi86_vid_state.attribute = 0x07;

  /*
   * Get the "normal" colour attribute to use for Borland normvideo () ---
   * but only if we need to.
   */
  __libi86_vid_get_norm_attr ();

  /* Read the current screen border colour, if possible.  If not, assume 0. */
  __libi86_vid_state.border = __libi86_vid_get_overscan ();

  /* Reset the text window. */
  __libi86_vid_state.x1z = __libi86_vid_state.y1z = 0;
  __libi86_vid_state.x2z = max_x;
  __libi86_vid_state.y2z = max_y;

  return (unsigned) max_y + 1;
}

/*
 * Note: this constructor must run before __libi86_setvideomode_default ()
 * is primed.
 */
#ifdef __GNUC__
__attribute__ ((constructor (99))) static
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
  __libi86_con_mode_changed (mode);
}
