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
#include "libi86/internal/conio.h"

#ifdef __MSDOS__
int
_kbhit (void)
{
  signed char status;
  unsigned char scratch;

  if (__libi86_ungetch_buf)
    return -1;

  if (__libi86_con_in_fd == 0)
    __asm volatile ("int $0x21" : "=Ral" (status), "=Rah" (scratch)
				: "1" ((unsigned char) 0x0b)
				: "cc");
  else
    {
      /* FIXME?  The status is undefined if an error occurs (CF == 1). */
      __asm volatile ("int $0x21" : "=Ral" (status), "=Rah" (scratch)
				  : "0" ((unsigned char) 0x06),
				    "1" ((unsigned char) 0x44),
				    "b" (__libi86_con_in_fd)
				  : "cc");
    }

  return (int) status;
}

_LIBI86_WEAK_ALIAS (_kbhit) int
kbhit (void);
#else
# warning "unknown host OS"
#endif
