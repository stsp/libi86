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

#include <stdint.h>
#include "libi86/internal/cdefs.h"
#include "i86.h"

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
  /* Current attribute for text output. */
  unsigned char attribute;
  /* Current screen border colour (& background colour, if graphics mode). */
  unsigned char border;
  /* Whether we are in a graphics mode. */
  unsigned graph_p : 1;
};

/*
 * Internal (row, column) co-ordinates, in character units.  This must
 * _exactly_ match the data format used in the BIOS data area.  The top left
 * corner of the physical screen is (0, 0).
 */
struct __libi86_vid_rccoord_t
{
  unsigned char col, row;
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
 * Change the BIOS's idea of the current cursor position for the display
 * page PG_NO, without actually moving the displayed cursor.
 */
static inline void
__libi86_vid_set_rccoord_only (unsigned char pg_no,
			       struct __libi86_vid_rccoord_t rc)
{
  * (volatile struct __libi86_vid_rccoord_t __far *)
    MK_FP (__libi86_bios_ds, 0x0050U + 2 * pg_no) = rc;
}

/* Really set the cursor position for the display page PG_NO. */
static inline void
__libi86_vid_go_rccoord (unsigned char pg_no, struct __libi86_vid_rccoord_t rc)
{
  unsigned xx1, xx2, xx3;
  __asm volatile ("int {$}0x10" : "=a" (xx1), "=b" (xx2), "=d" (xx3)
				: "Rah" ((uint8_t) 0x02),
				  "1" ((uint16_t) pg_no << 8),
				  "2" (rc)
				: "cc", "cx", "memory");
}

_LIBI86_END_EXTERN_C

#endif
