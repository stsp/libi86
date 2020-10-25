/*
 * Copyright (c) 2018 TK Chia
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

#ifndef _LIBI86_LIBI86_INTERNAL_GRAPH_H_
#define _LIBI86_LIBI86_INTERNAL_GRAPH_H_

#ifndef _LIBI86_COMPILING_
# error "<libi86/internal/graph.h> should only be used when compiling libi86!"
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <libi86/internal/cdefs.h>
#include <graph.h>
#include <i86.h>

_LIBI86_BEGIN_EXTERN_C

/* Various internal functions and variables used by <graph.h> routines. */

extern uint16_t __libi86_bios_ds;

struct __libi86_vid_state_t
{
  /* Current video mode number. */
  unsigned mode_num;
  /*
   * Maximum possible extent of text window, i.e. the actual display
   * dimensions, minus 1.  The <graph.h> interface uses 16-bit shortwords,
   * but internally we just use bytes (for now).
   */
  unsigned char max_x, max_y;
  /*
   * Current text window --- top left corner & bottom right corner.  These
   * are 0-based, i.e. the top left corner of the physical screen is (0, 0).
   */
  unsigned char x1z, y1z, x2z, y2z;
  /* Current colour attribute for text output. */
  unsigned char attribute;
  /*
   * "Normal" colour attribute for text output (for Borland-like <conio.h>). 
   * For text modes, this is taken from the attribute at the initial cursor
   * position at startup.
   */
  unsigned char borland_normal_attribute;
  /* Current screen border colour (& background colour, if graphics mode). */
  unsigned char border;
  /* Whether we are in a graphics mode. */
  unsigned char graph_p : 1;
};

/*
 * Internal (row, column) co-ordinates, in character units.  This must
 * _exactly_ match the data format used in the BIOS data area.  The top left
 * corner of the physical screen is (0, 0).
 */
struct __libi86_vid_rccoord_t
{
  unsigned char x, y;
};

extern struct __libi86_vid_state_t __libi86_vid_state;

/* Get the BIOS's idea of the current display page. */
static inline unsigned char
__libi86_vid_get_curr_pg (void)
{
  return * (volatile unsigned char __far *) MK_FP (__libi86_bios_ds, 0x0062U);
}

/*
 * Get the BIOS's idea of the current cursor position for the display page
 * PG_NO.
 */
static inline struct __libi86_vid_rccoord_t
__libi86_vid_get_rccoord (unsigned char pg_no)
{
  return * (volatile struct __libi86_vid_rccoord_t __far *)
	   MK_FP (__libi86_bios_ds, 0x0050U + 2 * pg_no);
}

/*
 * Get the BIOS's idea of the current cursor position for the display page
 * PG_NO.  If this cursor position is outside the current text window, return
 * instead the position for the window's top left corner.
 */
static inline struct __libi86_vid_rccoord_t
__libi86_vid_get_and_adjust_rccoord (unsigned char pg_no)
{
  unsigned char x1z = __libi86_vid_state.x1z;
  unsigned char y1z = __libi86_vid_state.y1z;
  unsigned char x2z = __libi86_vid_state.x2z;
  unsigned char y2z = __libi86_vid_state.y2z;

  struct __libi86_vid_rccoord_t pxy = __libi86_vid_get_rccoord (pg_no);

  if (pxy.x < x1z || pxy.x > x2z || pxy.y < y1z || pxy.y > y2z)
    {
      pxy.x = x1z;
      pxy.y = y1z;
    }

  return pxy;
}

/*
 * Change the BIOS's idea of the current cursor position for the display
 * page PG_NO, without actually moving the displayed cursor.
 */
static inline void
__libi86_vid_set_rccoord_only (unsigned char pg_no,
			       struct __libi86_vid_rccoord_t pxy)
{
  * (volatile struct __libi86_vid_rccoord_t __far *)
    MK_FP (__libi86_bios_ds, 0x0050U + 2 * pg_no) = pxy;
}

/* Really set the cursor position for the display page PG_NO. */
static inline void
__libi86_vid_go_rccoord (unsigned char pg_no,
			 struct __libi86_vid_rccoord_t pxy)
{
  unsigned xx1, xx2, xx3;
  __asm volatile ("int {$}0x10" : "=a" (xx1), "=b" (xx2), "=d" (xx3)
				: "Rah" ((uint8_t) 0x02),
				  "1" ((uint16_t) pg_no << 8),
				  "2" (pxy)
				: "cc", "cx", "memory");
}

/*
 * Update the given cursor position to scroll to the next line in the text
 * window.  Scroll the window area up if necessary.
 */
static inline struct __libi86_vid_rccoord_t
__libi86_vid_next_line (struct __libi86_vid_rccoord_t pxy)
{
  if (pxy.y < __libi86_vid_state.y2z)
    ++pxy.y;
  else
    _scrolltextwindow (_GSCROLLUP);
  pxy.x = __libi86_vid_state.x1z;
  return pxy;
}

/*
 * Common code for implementing _outmem (, ), _outtext (.), & Borland-like
 * putch (.), cputs (.), etc.
 */
static inline void
__libi86_vid_outmem_do (const char __far *text, size_t length,
			bool handle_cr_lf_p, bool handle_bel_bs_p)
{
  unsigned char pg_no;
  unsigned char x1z, x2z, attr;
  struct __libi86_vid_rccoord_t pxy;
  struct rccoord coord;

  if (! length)
    return;

  /* Get our current text window & output text colour attribute. */
  x1z = __libi86_vid_state.x1z;
  x2z = __libi86_vid_state.x2z;
  attr = __libi86_vid_state.attribute;

  /*
   * Get the current cursor position according to the BIOS.  If the cursor
   * is outside the window, mentally move it inside.
   */
  pg_no = __libi86_vid_get_curr_pg ();
  pxy = __libi86_vid_get_and_adjust_rccoord (pg_no);

  /* Display the characters.  Scroll the window as necessary. */
  while (length-- != 0)
    {
      unsigned xx1, xx2, xx3;
      char ch = *text++;

      if (handle_cr_lf_p)
	{
	  switch (ch)
	    {
	    case '\r':
	      pxy.x = x1z;
	      continue;
	    case '\n':
	      pxy = __libi86_vid_next_line (pxy);
	      continue;
	    default:
	      ;
	    }
	}

      if (handle_bel_bs_p)
	{
	  switch (ch)
	    {
	    case '\a':
	      /* Remember to move the physical cursor before beeping... */
	      __libi86_vid_go_rccoord (pg_no, pxy);
	      /* Beep. */
	      __asm volatile ("pushw %%bp; int $0x10; popw %%bp"
			      : "=a" (xx1), "=b" (xx2)
			      : "0" (0x0e00U | '\a'),
				"1" ((uint16_t) pg_no << 8 | attr)
			      : "cc", "cx", "dx", "memory");
	      /*
	       * If the '\a' is the last character, we can just return.
	       * Otherwise carry on.
	       */
	      if (! length)
		return;
	      continue;
	    case '\b':
	      if (pxy.x > x1z)
		--pxy.x;
		continue;
	    default:
	      ;
	    }
	}

      __libi86_vid_set_rccoord_only (pg_no, pxy);

      __asm volatile ("int $0x10" : "=a" (xx1), "=b" (xx2), "=c" (xx3)
				  : "Rah" ((unsigned char) 0x09), "Ral" (ch),
				    "1" ((uint16_t) pg_no << 8 | attr),
				    "2" (1U)
				  : "cc", "dx", "memory");
      if (pxy.x >= x2z)
	pxy = __libi86_vid_next_line (pxy);
      else
	++pxy.x;
    }

  /* Move the cursor to where it should now be. */
  __libi86_vid_go_rccoord (pg_no, pxy);
}

/* Scroll or clear an area of text characters on the current screen page. */
static inline void
__libi86_vid_scroll (unsigned char sx1z, unsigned char sy1z,
		     unsigned char sx2z, unsigned char sy2z,
		     unsigned char rows, bool scroll_up_p)
{
  unsigned char func = scroll_up_p ? 0x06 : 0x07;
  unsigned char attr
    = __libi86_vid_state.graph_p ? 0 : __libi86_vid_state.attribute;
  unsigned xx1, xx2, xx3, xx4;

  __asm volatile ("pushw %%bp; int $0x10; popw %%bp"
		  : "=a" (xx1), "=b" (xx2), "=c" (xx3), "=d" (xx4)
		  : "Rah" (func), "Ral" (rows),
		    "1" ((unsigned) attr << 8),
		    "c" (sy1z), "Rcl" (sx1z),
		    "Rdh" (sy2z), "Rdl" (sx2z));
}

extern void __libi86_vid_bc_insdelline (bool);
extern void __libi86_vid_bc_outmem_do (const char *, size_t);

_LIBI86_END_EXTERN_C

#endif
