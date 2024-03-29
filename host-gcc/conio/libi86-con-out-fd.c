/*
 * Copyright (c) 2018--2022 TK Chia
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
 * is an output file descriptor which is open to the console device `CON'
 * (MS-DOS) or the controlling terminal `/dev/tty' (ELKS).
 */

#define _LIBI86_COMPILING_
#include "libi86/internal/conio.h"
#ifdef __ELKS__
# include <unistd.h>
#endif

int __libi86_con_out_fd = 1;

#pragma GCC diagnostic ignored "-Wprio-ctor-dtor"
__attribute__ ((constructor (99))) static void
__libi86_con_out_fd_init (void)
{
  int fd;

#if defined __MSDOS__
  unsigned dw;

  /* If fd 1 is not the console output, open an output fd on `CON'. */
  if (__libi86_con_get_dev_info_word (1, &dw) != 0
      || (dw & 0x0082u) != 0x0082u)
    {
      fd = __libi86_con_open (__libi86_con_name, 1);
      if (fd != -1)
	__libi86_con_out_fd = fd;
    }
#elif defined __ELKS__  /* ! __MSDOS__ */
  /* If fd 1 is not a terminal device, open an output fd on `/dev/tty'. */
  if (! isatty (1))
    {
      fd = __libi86_tty_open (__libi86_con_name, O_WRONLY);
      if (fd >= 0)
	__libi86_con_out_fd = fd;
    }
#else  /* ! __MSDOS__ && ! __ELKS__ */
# error
#endif
}
