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

static uint8_t
__libi86_vid_rgb_to_ega_color (long pixval)
{
  /*
   * The Open Watcom v2 source code says in bld/graphlib/c/remappal.c,
   *
   *	"In mode 16 [i.e. _ERESCOLOR], each of the colour indices may be
   *	 assigned one of the 64 possible colours. These colour values are of
   *	 the form 00rgbRGB, where rgb are the secondary intensities and RGB
   *	 are the high intensities."
   *
   * Open Watcom picks the top 2 bits of each of the three colour components
   * from the original RGB triplet, to compute the high & secondary
   * intensities.  This code performs the same function, but differently.
   */
  uint8_t colr;
  /* Process blue component. */
  colr  =	     (pixval &		 0x200000) >> (21 - 0);
  colr |=	     (pixval &		 0x100000) >> (20 - 3);
  /* Process green component. */
  colr |= ((uint16_t) pixval &		 0x002000) >> (13 - 1);
  colr |= ((uint16_t) pixval &		 0x001000) >> (12 - 4);
  /* Process red component. */
  colr |=  ((uint8_t) pixval & (uint8_t) 0x000020) >>  (5 - 2);
  colr |=  ((uint8_t) pixval & (uint8_t) 0x000010) << -(4 - 5);
  return colr;
}

static uint8_t
__libi86_vid_rgb_to_cga_color (long pixval)
{
  /*
   * Precomputed table to map EGA colours (bits 00rgbRGB) to CGA colours. 
   * This is roughly based on Euclidean distances between the corresponding
   * RGB values, & the Hamming distances between the 00rgbRGB & CGA colour
   * codes.  (This is still not very rigorous.  FIXME?)
   */
  static const uint8_t ega_to_cga_colr_map[] = {
     0,  1,  2,  3,  4,  5,  6,  7,
     0,  1,  2,  3,  4,  5,  7,  7,
     0,  1,  2,  3,  6,  5, 14,  7,
     8,  9, 10, 11, 12, 13, 14, 15,
     0,  1,  2,  3,  4,  5,  6,  7,
     8,  9, 10, 11, 12, 13, 14, 15,
     8,  9, 10, 11, 12, 13, 14, 15,
     8,  9, 10, 11, 12, 13, 14, 15,
  };
  uint8_t ega_val = __libi86_vid_rgb_to_ega_color (pixval);
  return ega_to_cga_colr_map[ega_val];
}

static long
__libi86_setbkcolor_via_cga_color (long pixval)
{
  return __libi86_setbkcolor_indexed (__libi86_vid_rgb_to_cga_color (pixval));
}

long
__libi86_setbkcolor_rgb (long pixval)
{
  char adapter;
  long prev_val;
  uint8_t ega_val;

#ifndef __GNUC__
  __libi86_vid_state_init ();
#endif

  adapter = __libi86_vid_state.adapter;
  if (! __libi86_vid_state.graph_p || adapter == _CGA)
    return __libi86_setbkcolor_via_cga_color (pixval);

  prev_val = __libi86_vid_state.bk_colr;

  switch (adapter)
    {
    default:
      return -1;

    case _EGA:
      switch (__libi86_vid_state.mode_num)
	{
	default:
	  ega_val = __libi86_vid_rgb_to_ega_color (pixval);
	  break;
	case _MRES16COLOR:
	case _HRES16COLOR:
	  ega_val = __libi86_vid_rgb_to_cga_color (pixval);
	}
      __libi86_vid_set_ega_pal_reg (0, ega_val);
      break;

    case _MCGA:
    case _VGA:
    case _SVGA:
      __libi86_vid_set_vga_dac_reg (0, pixval);
    }

  __libi86_vid_state.bk_colr = pixval;
  return prev_val;
}
