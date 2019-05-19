/*
 * Copyright (c) 2019 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING3.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef _LIBI86_DPMI_H_
#define _LIBI86_DPMI_H_

#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

/* This follows Watcom's internally-used interface.  It should be defined by
   either the C runtime (e.g. Newlib), or the program using libi86.  */
extern int __DPMI_hosted (void) __attribute__ ((weak));

_LIBI86_END_EXTERN_C

#endif
