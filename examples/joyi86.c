/*
 * Copyright (c) 2020 TK Chia
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the developer(s) nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Simple test program to read the status of a joystick (if attached) via
 * BIOS calls and via direct port I/O.
 */

#include <bios.h>
#include <conio.h>
#include <i86.h>
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

      x_a = x_b = y_a = y_b = 0;
      _disable ();

      /* "some adapters won't work unless the one-shot sent to 201h has all
	 bits set" (Jim Leonard https://github.com/tkchia/GW-BASIC/issues/4) */
      outp (0x201, 0xff);

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

      _enable ();

      cprintf ("%c%c %c%c  %-5u %-5u %-5u %-5u\r",
	       byte & 0x10 ? '-' : '*', byte & 0x20 ? '-' : '*',
	       byte & 0x40 ? '-' : '*', byte & 0x80 ? '-' : '*',
	       x_a, y_a, x_b, y_b);
    }

  getch ();
  putch ('\n');
  return 0;
}
