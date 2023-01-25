/*
 * Copyright (c) 2018--2023 TK Chia
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
 * This module initializes the <conio.h> subsystem to make sure that there
 * is an input file descriptor which is open to the console device `CON'
 * (MS-DOS) or the controlling terminal `/dev/tty' (ELKS).
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
#ifdef __ELKS__
# include <stdbool.h>
# include <termios.h>
# include <unistd.h>
#endif

int __libi86_con_in_fd = 0;
#if defined __MSDOS__
static unsigned __libi86_con_in_info_word = 0;
#elif defined __ELKS__
static bool __libi86_con_in_reset_termios = false;
static struct termios __libi86_con_in_startup_termios;
#else
# error
#endif

#pragma GCC diagnostic ignored "-Wprio-ctor-dtor"
__attribute__ ((constructor (99))) static void
__libi86_con_in_fd_init (void)
{
  int fd;

#if defined __MSDOS__
  unsigned dw;

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
      fd = __libi86_con_open (__libi86_con_name, 0);
      if (fd != -1)
	{
	  __libi86_con_in_fd = fd;
	  if (fd != 0
	      && __libi86_con_get_dev_info_word (fd, &dw) == 0
	      && (dw & 0x0080u) != 0)
	    {
	      /*
	       * Ralf Brown's Interrupt List says, for int 0x21, ax =
	       * 0x4401, "DH [= high part of device information word] must
	       * be zero for DOS version prior to 6.x".
	       */
	      dw = (unsigned char) dw;

	      __libi86_con_in_info_word = dw;
	      __libi86_con_set_dev_info_word (fd, dw | 0x0020u);
	    }
	}
    }
#elif defined __ELKS__  /* !__MSDOS__ */
  /*
   * If fd 0 is not a terminal device, open an input fd on `/dev/tty'.  Set
   * the input fd to use raw mode.
   */
  fd = 0;
  if (! isatty (0))
    {
      fd = __libi86_tty_open (__libi86_con_name, O_RDONLY);
      if (fd >= 0)
	__libi86_con_in_fd = fd;
      else
	fd = 0;
    }

  if (tcgetattr (fd, &__libi86_con_in_startup_termios) == 0)
    {
      struct termios raw_mode = __libi86_con_in_startup_termios;
      cfmakeraw (&raw_mode);
      if (tcsetattr (fd, TCSAFLUSH, &raw_mode) == 0)
	__libi86_con_in_reset_termios = true;
    }
#endif  /* __ELKS__ */
}

__attribute__ ((destructor (100))) static void
__libi86_con_in_fd_fini (void)
{
#if defined __MSDOS__
  if (__libi86_con_in_fd != 0
      && (__libi86_con_in_info_word & 0x0080u) != 0)
    __libi86_con_set_dev_info_word (__libi86_con_in_fd,
				    __libi86_con_in_info_word);
#elif defined __ELKS__  /* ! __MSDOS__ */
  if (__libi86_con_in_reset_termios)
    tcsetattr (__libi86_con_in_fd, TCSAFLUSH,
	       &__libi86_con_in_startup_termios);
#endif  /* __ELKS__ */
}
