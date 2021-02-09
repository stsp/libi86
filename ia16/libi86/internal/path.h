/*
 * Copyright (c) 2021 TK Chia
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

#ifndef _LIBI86_LIBI86_INTERNAL_PATH_H_
#define _LIBI86_LIBI86_INTERNAL_PATH_H_

#ifndef _LIBI86_COMPILING_
# error "<libi86/internal/path.h> should only be used when compiling libi86!"
#endif

#include <stdbool.h>
#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

static inline bool
__libi86_msdos_drive_letter_p (char c)
{
  switch (c)
    {
    case 'A' ... 'Z':
    case 'a' ... 'z':
      return true;
    default:
      return false;
    }
}

static inline bool
__libi86_msdos_path_sep_p (char c)
{
  switch (c)
    {
    case '/':
    case '\\':
      return true;
    default:
      return false;
    }
}

#endif
