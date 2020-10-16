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

#ifndef _LIBI86_LIBI86_INTERNAL_CONIO_H_
#define _LIBI86_LIBI86_INTERNAL_CONIO_H_

#ifndef _LIBI86_COMPILING_
# error "<libi86/internal/conio.h> should only be used when compiling libi86!"
#endif

#include <stdint.h>
#include <libi86/internal/cdefs.h>
#include <i86.h>
#include <conio.h>
#ifdef __IA16_FEATURE_PROTECTED_MODE
# include <dpmi.h>
# include <fcntl.h>
#endif

_LIBI86_BEGIN_EXTERN_C

/* Various internal variables & functions used by <conio.h> routines. */

extern unsigned __libi86_ungetch_buf;

#ifdef __MSDOS__
extern int __libi86_con_in_fd, __libi86_con_out_fd;

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
  int fd;
# ifdef __IA16_FEATURE_PROTECTED_MODE
  if (__DPMI_hosted () == 1)
    return open (pathname, flags ? O_WRONLY : O_RDONLY);
# endif
  __asm volatile ("int $0x21; jnc 0f; sbbw %0, %0; 0:"
    : "=a" (fd)
    : "0" (0x3d00 | (unsigned char) flags), "d" (pathname));
  return fd;
}
#endif /* __MSDOS__ */

_LIBI86_END_EXTERN_C

#endif
