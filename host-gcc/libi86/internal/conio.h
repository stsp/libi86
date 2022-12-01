/*
 * Copyright (c) 2020--2021 TK Chia
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

#ifndef _LIBI86_LIBI86_INTERNAL_CONIO_H_
#define _LIBI86_LIBI86_INTERNAL_CONIO_H_

#ifndef _LIBI86_COMPILING_
# error "<libi86/internal/conio.h> should only be used when compiling libi86!"
#endif

#include <stdint.h>
#include <libi86/internal/cdefs.h>
#include <i86.h>
#include <conio.h>
#ifdef __ELKS__
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
#endif

_LIBI86_BEGIN_EXTERN_C

/* Various internal variables & functions used by <conio.h> routines. */

extern unsigned __libi86_ungetch_buf;
extern unsigned char __libi86_vid_norm_attr;
#ifdef __ELKS__
extern bool __libi86_vid_is_console;
#endif

extern int __libi86_con_in_fd, __libi86_con_out_fd;
extern const char __libi86_con_name[];

#if defined __MSDOS__
static inline int
__libi86_con_get_dev_info_word (int fd, unsigned *pdw)
{
  int rv;
  unsigned dw;
  __asm volatile ("int $0x21; sbbw %0, %0" : "=a" (rv), "=d" (dw)
					   : "0" (0x4400u), "b" (fd)
					   : "cc");
  if (rv == 0)
    *pdw = dw;
  return rv;
}

static inline void
__libi86_con_set_dev_info_word (int fd, unsigned dw)
{
  __asm volatile ("int $0x21" : /* no outputs */
			      : "a" (0x4401u), "b" (fd),
				"d" ((unsigned) (unsigned char) dw)
			      : "cc");
}

static inline int
__libi86_con_open (const char *pathname, int flags)
{
  int fd, xx;
  __asm volatile ("int $0x21; jnc 0f; sbbw %0, %0; 0:"
    : "=a" (fd), "=d" (xx)
    : "0" (0x3d00 | (unsigned char) flags),
      "1" (FP_OFF (pathname)), "Rds" (FP_SEG (pathname))
    : "bx", "cx", "cc", "memory");
  return fd;
}
#elif defined __ELKS__
static inline int
__libi86_tty_open (const char *pathname, int flags)
{
  int res;
  __asm volatile ("int $0x80"
    : "=r" (res)
    : "0" (5 /* open syscall */), "b" (pathname), "c" (flags)
    : "cc", "memory");
  return res;
}

static inline int
__libi86_tty_ioctl (int fd, int cmd, void *arg)
{
  int res;
  __asm volatile ("int $0x80"
    : "=r" (res)
    : "0" (54 /* ioctl syscall */), "b" (fd), "c" (cmd), "d" (arg)
    : "cc", "memory");
  return res;
}
#endif  /* __ELKS__ */

_LIBI86_END_EXTERN_C

#endif
