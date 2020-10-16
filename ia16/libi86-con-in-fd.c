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
 * is an input file descriptor which is open to the console device `CON'.
 *
 * This is (in a way) an improvement over the classical Turbo C and Open
 * Watcom C/C++'s <conio.h> code: they implement kbhit (), getch (), etc.,
 * using int $0x21 calls in a way that assume that standard input (fd 0)
 * points to `CON', whether or not it does.
 *
 * (Watcom in addition assumes that standard output, fd 1, points to `CON'. 
 * libi86 initializes the fd for console output in libi86-con-out-fd.c .)
 */

#define _LIBI86_COMPILING_
#include "libi86/internal/conio.h"

#ifdef __MSDOS__
int __libi86_con_in_fd = 0;
static unsigned __libi86_con_in_info_word = 0;

__attribute__ ((constructor (100))) static void
__libi86_con_in_fd_init (void)
{
  unsigned dw;
  int fd;

  /*
   * If fd 0 is not the console input, open an input fd on `CON'.  Set the
   * input fd to use raw binary mode, so that e.g. getch () can use int
   * $0x21, %ah == $0x3f to read from it.
   *
   * Also store the original device information word so that we can restore
   * the fd to cooked mode.  (TODO: figure out if this is really needed.)
   */
  if (__libi86_con_get_dev_info_word (0, &dw) != 0
      || (dw & 0x0081u) != 0x0081u)
    {
      fd = __libi86_con_open ("CON", 0);
      if (fd != -1)
	{
	  __libi86_con_in_fd = fd;
	  if (fd != 0
	      && __libi86_con_get_dev_info_word (fd, &dw) == 0
	      && (dw & 0x0080u) != 0)
	    {
	      __libi86_con_in_info_word = dw;
	      __libi86_con_set_dev_info_word (fd, dw | 0x0020u);
	    }
	}
    }
}

__attribute__ ((destructor (100))) static void
__libi86_con_in_fd_fini (void)
{
  if (__libi86_con_in_fd != 0
      && (__libi86_con_in_info_word & 0x0080u) != 0)
    __libi86_con_set_dev_info_word (__libi86_con_in_fd,
				    __libi86_con_in_info_word);
}
#endif /* __MSDOS__ */
