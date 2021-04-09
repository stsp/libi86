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
