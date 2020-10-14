/*
 * Copyright (c) 2020 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING2.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
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

__attribute__ ((regparmcall)) unsigned
__libi86_con_mode_changed (unsigned mode)
{
  unsigned ax, bx;
  unsigned char max_x, max_y;
  uint8_t mode_ctl_reg;

  /* Record the mode number. */
  __libi86_vid_state.mode_num = mode;

  /* Record the maximum (x, y) coordinates in character units. */
  max_x = * (volatile unsigned char __far *) MK_FP (__libi86_bios_ds, 0x004aU)
	  - 1;
  __libi86_vid_state.max_x = max_x;

  max_y = * (volatile unsigned char __far *) MK_FP (__libi86_bios_ds, 0x0084U);
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
      mode_ctl_reg
	= * (volatile uint8_t __far *) MK_FP (__libi86_bios_ds, 0x0065U);
      __libi86_vid_state.graph_p = ((mode_ctl_reg & 0x02) != 0);
    }

  /* Reset the colour attribute to use for text output. */
  __libi86_vid_state.attribute = 0x07;

  /* Read the current screen border colour, if possible.  If not, assume 0. */
  __asm volatile ("int $0x10" : "=a" (ax), "=b" (bx)
			      : "0" (0x1008U), "1" (0)
			      : "cc", "cx", "dx");
  __libi86_vid_state.border = bx >> 8;

  /* Reset the text window. */
  __libi86_vid_state.x1z = __libi86_vid_state.y1z = 0;
  __libi86_vid_state.x2z = max_x;
  __libi86_vid_state.y2z = max_y;

  return (unsigned) max_y + 1;
}

/*
 * Note: this constructor must run before __libi86_setvideomode_default ()
 * default () is primed.
 */
__attribute__ ((constructor (99))) static void
__libi86_vid_state_init (void)
{
  unsigned ax, mode;

  /*
   * Try to retrieve the current mode via VESA.  If VESA fails, use the
   * classical BIOS interface to get the video mode.
   */
  __asm volatile ("int $0x10" : "=a" (ax), "=b" (mode)
			      : "0" (0x4f03U)
			      : "cc", "cx", "dx");
  if (ax == 0x004f)
    mode &= 0x7fffU;
  else
    {
      __asm volatile ("int $0x10" : "=a" (mode)
				  : "Rah" ((unsigned char) 0x0f)
				  : "cc", "bx", "cx", "dx");
      mode &= 0x007fU;
    }

  /* Update the mode cache. */
  __libi86_con_mode_changed (mode);
}
