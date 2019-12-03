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

/*
 * This implements an internal routine to set errno to an error code output
 * by MS-DOS, and then return the same value to the caller.  It makes no
 * assumptions about the actual form that errno takes (e.g. it might be a
 * macro which calls a function), but it assumes that MS-DOS error codes can
 * be directly treated as errno values.
 */

#define _LIBI86_COMPILING_
#include <errno.h>

#ifdef __MSDOS__
/* Do some basic sanity checking of errno values at compile time.  */
# if defined ENOENT && ENOENT - 0 != 2
#   error "weird ENOENT value for MS-DOS target"
# endif
# if defined EBADF && EBADF - 0 != 6
#   error "weird EBADF value for MS-DOS target"
# endif
# if defined ENOMEM && ENOMEM - 0 != 8
#   error "weird ENOMEM value for MS-DOS target"
# endif

__attribute__ ((regparmcall)) unsigned
__libi86_ret_set_errno (unsigned err)
{
  errno = (int) err;
  return err;
}
#endif
