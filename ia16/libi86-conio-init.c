/*
 * Copyright (c) 2018 TK Chia
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
 * This module initializes the <conio.h> subsystem to make sure that there
 * are file descriptors which are open to the console device `CON'.
 *
 * This is (in a way) an improvement over the classical Turbo C and Open
 * Watcom C/C++'s <conio.h> code: they implement kbhit (), getch (), using
 * int $0x21 calls in a way that assume that standard input (fd 0) and
 * standard output (fd 1) point to `CON', whether or not they do.
 */

#define _LIBI86_COMPILING_
#include "libi86/internal/conio.h"

#ifdef __MSDOS__
int __libi86_con_in_fd = 0, __libi86_con_out_fd = 1;
static unsigned __libi86_con_in_info_word = 0;

typedef unsigned char byte;

static inline int
do_get_dev_info_word (int fd, unsigned *pdw)
{
  int rv;
  unsigned dw;
  __asm volatile ("int $0x21; sbbw %0, %0" : "=a" (rv), "=d" (dw)
					   : "0" (0x4400u), "b" (fd)
					   : "cc");
  if (rv == 0)
    *pdw = dw;
  return rv;
}

static inline void
do_set_dev_info_word (int fd, unsigned dw)
{
  __asm volatile ("int $0x21" : /* no outputs */
			      : "a" (0x4401u), "b" (fd),
				"d" ((unsigned) (byte) dw)
			      : "cc");
}

static inline int
do_open (const char *pathname, int flags)
{
  int fd;
  __asm volatile ("int $0x21; jnc 0f; sbbw %0, %0; 0:"
    : "=a" (fd)
    : "0" (0x3d00 | (byte) flags), "d" (pathname));
  return fd;
}

__attribute__ ((constructor (100))) static void
__libi86_conio_init (void)
{
  unsigned dw;
  int fd;

  /* If fd 0 is not the console input, open an input fd on `CON'.  Set the
     input fd to use raw binary mode, so that e.g. getch () can use int
     $0x21, %ah == $0x3f to read from it.

     Also store the original device information word so that we can restore
     the fd to cooked mode.  (TODO: figure out if this is really needed.)  */
  if (do_get_dev_info_word (0, &dw) != 0
      || (dw & 0x0081u) != 0x0081u)
    {
      fd = do_open ("CON", 0);
      if (fd != -1)
	{
	  __libi86_con_in_fd = fd;
	  if (fd != 0
	      && do_get_dev_info_word (fd, &dw) == 0
	      && (dw & 0x0080u) != 0)
	    {
	      __libi86_con_in_info_word = dw;
	      do_set_dev_info_word (fd, dw | 0x0020u);
	    }
	}
    }

  /* If fd 1 is not the console output, open an output fd on `CON'.  */
  if (do_get_dev_info_word (1, &dw) != 0
      || (dw & 0x0082u) != 0x0082u)
    {
      fd = do_open ("CON", 1);
      if (fd != -1)
	__libi86_con_out_fd = fd;
    }
}

__attribute__ ((destructor (100))) static void
__libi86_conio_fini (void)
{
  if (__libi86_con_in_fd != 0
      && (__libi86_con_in_info_word & 0x0080u) != 0)
    do_set_dev_info_word (__libi86_con_in_fd, __libi86_con_in_info_word);
}
#endif
