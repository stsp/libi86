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
#include "bios.h"

unsigned short
_bios_timeofday (unsigned service, long *timeval)
{
  unsigned char midnight;
  unsigned tv_hi, tv_lo;
  unsigned xx1, xx2, xx3;
  long tv;
  switch (service)
    {
    case _TIME_GETCLOCK:
      __asm volatile ("int $0x1a"
		      : "=Ral" (midnight), "=c" (tv_hi), "=d" (tv_lo)
		      : "Rah" ((unsigned char) 0)
		      : "cc", "memory");
      *timeval = (long) tv_hi << 16 | tv_lo;
      return (unsigned short) midnight;

    case _TIME_SETCLOCK:
      tv = *timeval;
      tv_hi = (unsigned) (tv >> 16);
      tv_lo = (unsigned) tv;
      __asm volatile ("int $0x1a"
		      : "=a" (xx1), "=c" (xx2), "=d" (xx3)
		      : "Rah" ((unsigned char) 1), "1" (tv_hi), "2" (tv_lo)
		      : "cc", "memory");
      return 0;
    }

  return (unsigned short) -1;
}
