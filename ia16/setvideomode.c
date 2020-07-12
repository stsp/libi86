/*
 * Copyright (c) 2020 TK Chia
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

/*
 * Out-of-line implementation of _setvideomode (MODE) for cases where MODE
 * is not a compile-time constant.
 */

#define _LIBI86_COMPILING_
#include <inttypes.h>
#include "graph.h"

__far __attribute__ ((far_section)) short
__libi86_setvideomode (short mode)
{
  return __libi86_setvideomode_inline_dispatch (mode);
}

_LIBI86_ALIAS (__libi86_setvideomode)
__far __attribute__ ((far_section)) short
_setvideomode (short mode);
