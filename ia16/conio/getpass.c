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

#define _BORLANDC_SOURCE
#define _LIBI86_COMPILING_
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "conio.h"

static char pass[9];

char *
getpass (const char *prompt)
{
  size_t len = sizeof (pass) - 1, i = 0;
  int ch;

  if (prompt)
    cputs (prompt);

  memset (pass, 0, sizeof (pass));

  for (;;)
    {
      ch = _getch ();
      switch (ch)
	{
	case EOF:
	case (int) (unsigned char) '\r':
	  pass[i] = 0;
	  cputs ("\r\n");
	  return pass;

	case (int) (unsigned char) '\b':
	  if (i != 0)
	    {
	      --i;
	      pass[i] = 0;
	    }
	  break;

	case 0:
	  _getch ();
	  break;

	default:
	  if (i < len
	      && ! iscntrl ((unsigned char) ch))
	    {
	      pass[i] = ch;
	      ++i;
	    }
	}
    }
}
