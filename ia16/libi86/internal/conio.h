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

#ifndef _LIBI86_LIBI86_INTERNAL_CONIO_H_
#define _LIBI86_LIBI86_INTERNAL_CONIO_H_

#include <stdint.h>
#include "libi86/internal/cdefs.h"
#include "i86.h"

_LIBI86_BEGIN_EXTERN_C

/*
 * Various internal functions and variables used by <conio.h> & <graph.h>
 * routines.
 */

#ifdef __MSDOS__
extern int __libi86_con_in_fd, __libi86_con_out_fd;
#endif
extern unsigned __libi86_ungetch_buf;

extern uint16_t __libi86_bios_ds;

struct __libi86_con_vid_state_t
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

extern struct __libi86_con_vid_state_t __libi86_con_vid_state;

static inline unsigned char
bios_data_byte (unsigned off)
{
  return * (volatile unsigned char __far *) MK_FP (__libi86_bios_ds, off);
}

static inline unsigned
bios_data_word (unsigned off)
{
  return * (volatile unsigned __far *) MK_FP (__libi86_bios_ds, off);
}

_LIBI86_END_EXTERN_C

#endif
