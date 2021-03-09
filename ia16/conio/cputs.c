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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libi86/internal/conio.h"

#ifdef __MSDOS__
int
cputs (const char *buf)
{
  char *p_nl;
  size_t len;
  ssize_t n;

  while ((p_nl = strchr (buf, '\n')) != NULL)
    {
      len = p_nl - buf;
      n = write (__libi86_con_out_fd, buf, len);
      if (n != len)
	{
	  if (n >= 0)
	    errno = EIO;
	  return EOF;
	}

      n = write (__libi86_con_out_fd, "\r\n", 2);
      if (n != 2)
	{
	  if (n >= 0)
	    errno = EIO;
	  return EOF;
	}

      buf = p_nl + 1;
    }

  len = strlen (buf);
  n = write (__libi86_con_out_fd, buf, len);

  if (n == len)
    return 0;

  if (n >= 0)
    errno = EIO;

  return EOF;
}
#else
# warning "unknown host OS"
#endif
