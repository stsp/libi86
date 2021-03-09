/*
 * Copyright (c) 2018 TK Chia
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

#define _LIBI86_COMPILING_
#include "bios.h"

unsigned short
_bios_keybrd (unsigned service)
{
  unsigned short ax = __builtin_bswap16 (service);

  switch (service)
    {
    case _KEYBRD_READY:
    case _NKEYBRD_READY:
      __asm volatile ("int $0x16; jnz 0f; xorw %0, %0; 0:"
		      : "=a" (ax)
		      : "0" (ax)
		      : "cc", "memory");
      break;

    case _KEYBRD_READ:
    case _KEYBRD_SHIFTSTATUS:
    case _NKEYBRD_READ:
    case _NKEYBRD_SHIFTSTATUS:
      __asm volatile ("int $0x16" : "=a" (ax) : "0" (ax) : "cc", "memory");
      break;

    default:
      __asm volatile ("pushw %%bp; int $0x16; popw %%bp"
		      : "=a" (ax)
		      : "0" (ax)
		      : "bx", "cx", "dx", "si", "di", "cc", "memory");
    }

  return ax;
}
