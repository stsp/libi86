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

#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

/*
 * Various internal functions and variables used by <conio.h> & <graph.h>
 * routines.
 */

#ifdef __MSDOS__
extern int __libi86_con_in_fd, __libi86_con_out_fd;
#endif
extern unsigned __libi86_ungetch_buf;

struct __libi86_text_wnd_t
  {
    /* Current text window --- top left corner & bottom right corner. */
    unsigned x1, y1, x2, y2;
    /*
     * Maximum possible extent of text window, i.e. the actual display
     * dimensions.
     */
    unsigned max_x, max_y;
  };

struct __libi86_text_wnd_t __libi86_con_wnd;

_LIBI86_END_EXTERN_C

#endif
