/*
 * Copyright (c) 2018--2020 TK Chia
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
 * This module initializes the <conio.h> subsystem to make sure that there
 * is an output file descriptor which is open to the console device `CON'.
 */

#define _LIBI86_COMPILING_
#include "libi86/internal/conio.h"

#ifdef __MSDOS__
int __libi86_con_out_fd = 1;

__attribute__ ((constructor (100))) static void
__libi86_con_out_fd_init (void)
{
  unsigned dw;
  int fd;

  /* If fd 1 is not the console output, open an output fd on `CON'. */
  if (__libi86_con_get_dev_info_word (1, &dw) != 0
      || (dw & 0x0082u) != 0x0082u)
    {
      fd = __libi86_con_open ("CON", 1);
      if (fd != -1)
	__libi86_con_out_fd = fd;
    }
}
#endif /* __MSDOS__ */
