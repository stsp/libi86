/*
 * Copyright (c) 2018 TK Chia
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

#define _LIBI86_COMPILING_
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include "conio.h"

#ifdef __MSDOS__
char *
# ifdef _BORLANDC_SOURCE
__libi86_bc_cgets (char *buf)
# else
cgets (char *buf)
# endif
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
