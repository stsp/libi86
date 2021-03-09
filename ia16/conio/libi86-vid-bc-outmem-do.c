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
 * Internal routine used by Borland-like cputs (.), putch (.), vcprintf (...),
 * etc.
 */

#define _BORLANDC_SOURCE
#define _LIBI86_COMPILING_
#include <stdlib.h>
#include <string.h>
#include "libi86/string.h"
#include "libi86/internal/graph.h"

void
__libi86_vid_bc_outmem_do (const char *text, size_t length)
{
  __libi86_vid_outmem_do (text, length, true, true);
}
