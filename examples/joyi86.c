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
 * Simple test program to read the status of a joystick (if attached) via
 * BIOS calls and via direct port I/O.
 */

#include <bios.h>
#include <conio.h>
#include <stdio.h>

int
main (void)
{
  union _joyinfo_t info;
  unsigned char byte;
  unsigned x_a, y_a, x_b, y_b;

  cputs ("Press any key to stop!\n"
	 "\n"
	 "Joystick status via int 0x15   Joystick status via port 0x201\n"
	 "tA tB (xA,   yA)  (xB,   yB)   tA tB (xA,   yA)  (xB,   yB)\n");

  while (! kbhit ())
    {
      if (_bios_joystick (0, &info) != 0)
	cputs ("?? ?? ");
      else
	{
	  byte = info.trig;
	  cprintf ("%c%c %c%c ",
		   byte & 0x10 ? '-' : '*', byte & 0x20 ? '-' : '*',
		   byte & 0x40 ? '-' : '*', byte & 0x80 ? '-' : '*');
	}

      if (_bios_joystick (1, &info) != 0)
	cputs (" ????? ????? ????? ????? ");
      else
	cprintf (" %-5u %-5u %-5u %-5u ", info.pos.x_a, info.pos.y_a,
					  info.pos.x_b, info.pos.y_b);

      outp (0x201, 0);
      x_a = x_b = y_a = y_b = 0;

      while (((byte = inp (0x201)) & 0x0f) != 0)
	{
	  if ((byte & 0x01) != 0)
	    {
	      ++x_a;
	      if (! x_a)
		{
		  --x_a;
		  break;
		}
	    }
	  if ((byte & 0x02) != 0)
	    {
	      ++y_a;
	      if (! y_a)
		{
		  --y_a;
		  break;
		}
	    }
	  if ((byte & 0x04) != 0)
	    {
	      ++x_b;
	      if (! x_b)
		{
		  --x_b;
		  break;
		}
	    }
	  if ((byte & 0x08) != 0)
	    {
	      ++y_b;
	      if (! y_b)
		{
		  --y_b;
		  break;
		}
	    }
	}

      cprintf ("%c%c %c%c  %-5u %-5u %-5u %-5u\r",
	       byte & 0x10 ? '-' : '*', byte & 0x20 ? '-' : '*',
	       byte & 0x40 ? '-' : '*', byte & 0x80 ? '-' : '*',
	       x_a, y_a, x_b, y_b);
    }

  getch ();
  putch ('\n');
  return 0;
}
