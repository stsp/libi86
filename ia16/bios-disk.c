/*
 * Copyright (c) 2019 TK Chia
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
#include <inttypes.h>
#include "i86.h"
#include "bios.h"

unsigned short
_bios_disk (unsigned service, struct diskinfo_t *diskinfo)
{
  unsigned short ax, di;
  __typeof__ (__builtin_ia16_selector (0u)) es;
  uint8_t ch, cl, dh, dl;

  switch (service)
    {
    case _DISK_RESET:
    case _DISK_STATUS:
      __asm volatile ("int $0x13"
		      : "=a" (ax)
		      : "0" (service << 8), "Rdl" ((uint8_t) diskinfo->drive)
		      : "cc", "memory");
      break;

    case _DISK_VERIFY:
      ax = service << 8 | (uint8_t) diskinfo->nsectors;
      ch = (uint8_t) diskinfo->track;
      cl = (uint8_t) diskinfo->sector
	   | ((uint8_t) (diskinfo->track >> 2) & 0xc0);
      dh = (uint8_t) diskinfo->head;
      dl = (uint8_t) diskinfo->drive;
      __asm volatile ("int $0x13"
		      : "=a" (ax)
		      : "0" (ax), "c" (ch), "Rcl" (cl), "Rdh" (dh), "Rdl" (dl)
		      : "cc", "memory");
      break;

    case _DISK_DRIVEPARAMS:
      ax = _DISK_DRIVEPARAMS << 8;
      dl = (uint8_t) diskinfo->drive;
      /* Ralf Brown's Interrupt List says to set %es = %di = 0 "to guard
	 against BIOS bugs".  */
      __asm volatile ("int $0x13"
		      : "=a" (ax), "=c" (ch), "=Rcl" (cl),
			"=Rdh" (dh), "=Rdl" (dl), "=e" (es), "=D" (di)
		      : "0" (ax), "4" (dl), "5" (__builtin_ia16_selector (0u)),
			"6" (0u)
		      : "bx", "cc", "memory");
      diskinfo->head = dh;
      diskinfo->track = (unsigned) ch | (unsigned) (cl & 0xc0) << 2;
      diskinfo->sector = cl & 0x3f;
      /* Actually the number of drives...  */
      diskinfo->drive = dl;
      break;

    default:
      ax = service << 8 | (uint8_t) diskinfo->nsectors;
      ch = (uint8_t) diskinfo->track;
      cl = (uint8_t) diskinfo->sector
	   | ((uint8_t) (diskinfo->track >> 2) & 0xc0);
      dh = (uint8_t) diskinfo->head;
      dl = (uint8_t) diskinfo->drive;
      __asm volatile ("int $0x13"
		      : "=a" (ax)
		      : "0" (ax), "c" (ch), "Rcl" (cl), "Rdh" (dh), "Rdl" (dl),
			"e" (FP_SEG (diskinfo->buffer)),
			"b" (FP_OFF (diskinfo->buffer))
		      : "cc", "memory");
    }

  return ax;
}
