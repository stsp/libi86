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

#define _LIBI86_COMPILING_
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "libi86/internal/conio.h"

#ifdef __MSDOS__
int
_getch (void)
{
  unsigned char ch, scratch;
  ssize_t n;

  if (__libi86_ungetch_buf)
    {
      ch = (unsigned char) __libi86_ungetch_buf;
      __libi86_ungetch_buf = 0;
      return ch;
    }

  if (__libi86_con_in_fd == 0)
    {
      __asm volatile ("int $0x21" : "=Ral" (ch), "=Rah" (scratch)
				  : "1" ((unsigned char) 0x07)
				  : "cc");
      return ch;
    }

  n = read (__libi86_con_in_fd, &ch, 1);
  if (n == 1)
    return ch;
  if (n >= 0)
    errno = EIO;
  return EOF;
}

_LIBI86_WEAK_ALIAS (_getch) int
getch (void);
#else
# warning "unknown host OS"
#endif
