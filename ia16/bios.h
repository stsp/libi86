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

#ifndef _LIBI86_BIOS_H_
#define _LIBI86_BIOS_H_

#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

#ifdef __FAR
struct diskinfo_t
{
  unsigned drive, head, track, sector, nsectors;
  void __far *buffer;
};
#else
/* The GNU C++ parser does not support the __far keyword yet.  Ugh.  */
struct diskinfo_t;
#endif

/* Not in Open Watcom.  */
union _joyinfo_t
{
  struct
    {
      unsigned __ax, __bx, __cx, __dx;
    } __raw;
  unsigned char trig;
  struct
    {
      unsigned x_a, y_a, x_b, y_b;
    } pos;
};

_LIBI86_ALT_INLINE unsigned short
_bios_equiplist (void)
{
  unsigned a;
  __asm volatile ("int $0x11" : "=a" (a));
  return a;
}

_LIBI86_ALT_INLINE unsigned short
_bios_memsize (void)
{
  unsigned a;
  __asm volatile ("int $0x12" : "=a" (a));
  return a;
}

extern unsigned short _bios_disk (unsigned __service,
				  struct diskinfo_t *__diskinfo);
extern unsigned short _bios_keybrd (unsigned __service);
/* Note:  The Open Watcom C Library Reference gives the prototype
	int _bios_timeofday (int service, long *__timeval);
   with different argument and return types.  I follow the actual Open Watcom
   <bios.h> prototype here.  */
extern unsigned short _bios_timeofday (unsigned __service, long *__timeval);
/* Not in Open Watcom.  */
extern unsigned short _bios_joystick (unsigned __service,
				      union _joyinfo_t *__joyinfo);

#define _DISK_RESET		0
#define _DISK_STATUS		1
#define _DISK_READ		2
#define _DISK_WRITE		3
#define _DISK_VERIFY		4
#define _DISK_FORMAT		5
/* Not in Open Watcom.  */
#define _DISK_DRIVEPARAMS	8

#define _COM_INIT		0
#define _COM_SEND		1
#define _COM_RECEIVE		2
#define _COM_STATUS		3

#define _KEYBRD_READ		0x00u
#define _KEYBRD_READY		0x01u
#define _KEYBRD_SHIFTSTATUS	0x02u
#define _NKEYBRD_READ		0x10u
#define _NKEYBRD_READY		0x11u
#define _NKEYBRD_SHIFTSTATUS	0x12u

#define _PRINTER_WRITE		0
#define _PRINTER_INIT		1
#define _PRINTER_STATUS		2

#define _TIME_GETCLOCK		0x00u
#define _TIME_SETCLOCK		0x01u

/* Not in Open Watcom.  */
#define _JOY_READTRIG		0
#define _JOY_READPOS		1

_LIBI86_END_EXTERN_C

#endif
