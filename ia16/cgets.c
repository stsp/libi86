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

#define _LIBI86_COMPILING_
#include <errno.h>
#include <stdio.h>
#include "conio.h"

#ifdef __MSDOS__
char *
cgets (char *buf)
{
  /* Open Watcom implements cgets (.) using int $0x21, %ah == 0x0a, but it
     is hard to replicate this service's functionality if stdin and/or
     stdout happen to be redirected.  So instead I just implement a very
     bare-bones line input facility using _getch (), putch (.), and cputs
     (.) ...  -- tkchia  */
  size_t len, i = 0;
  int ch;

  if (! buf)
    {
      errno = EINVAL;
      return NULL;
    }

  len = (size_t) (unsigned char) buf[0];
  if (len == 0)
    {
      errno = EINVAL;
      return NULL;
    }
  --len;

  for (;;)
    {
      ch = _getch ();
      switch (ch)
	{
	case EOF:
	  /* Something went wrong.  If we already have some input, return
	     that.  Otherwise return NULL to flag an error.  */
	  if (i == 0)
	    {
	      buf[1] = buf[2] = 0;
	      return NULL;
	    }
	  goto done;

	case (int) (unsigned char) '\r':
	  putch (ch);
	done:
	  buf[1] = i;
	  buf[2 + i] = 0;
	  return buf + 2;

	case (int) (unsigned char) '\b':
	  if (i != 0)
	    {
	      cputs ("\b \b");
	      --i;
	    }
	  break;

	case 0:
	  /* For now, ignore extended key codes.  */
	  _getch ();
	  break;

	default:
	  if (i < len
	      && ! iscntrl ((unsigned char) ch))
	    {
	      putch (ch);
	      buf[2 + i] = ch;
	      ++i;
	    }
	}
    }
}
#else
# warning "unknown host OS"
#endif
