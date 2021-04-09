/*
 * Copyright (c) 2019 TK Chia
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
#include <inttypes.h>
#include <libi86/internal/cdefs.h>
#include "i86.h"
#include "bios.h"

unsigned short
_bios_disk (unsigned service, struct diskinfo_t *diskinfo)
{
  unsigned short ax, di;
  _LIBI86_SEG_SELECTOR es;
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
