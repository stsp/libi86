/*
 * Copyright (c) 2018--2021 TK Chia
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

#ifndef _LIBI86_DOS_H_
#define _LIBI86_DOS_H_

#include <libi86/internal/cdefs.h>

/* <dos.h> should contain everything that <i86.h> has (at least for the IA-16
   platform...).  */
#include <i86.h>

#define _A_NORMAL	0x00u
#define _A_RDONLY	0x01u
#define _A_HIDDEN	0x02u
#define _A_SYSTEM	0x04u
#define _A_VOLID	0x08u
#define _A_SUBDIR	0x10u
#define _A_ARCH		0x20u

_LIBI86_BEGIN_EXTERN_C

struct __attribute__ ((__packed__)) find_t
{
  char reserved[21];
  char attrib;
  unsigned short wr_time;
  unsigned short wr_date;
  unsigned long size;
  char name[13];
};

extern int bdos (int __dos_func, unsigned __dx, unsigned __al);
extern int bdosptr (int __dos_func, void *__dx, unsigned __al);
extern int _LIBI86_BC_REDIRECT (intdos, (const union REGS *, union REGS *));
extern int _LIBI86_BC_REDIRECT
	     (intdosx, (const union REGS *, union REGS *, struct SREGS *));
extern unsigned _dos_allocmem (unsigned __size, unsigned *__seg);
extern unsigned _dos_close (int __handle);
extern unsigned _dos_findfirst (const char *__path, unsigned __attr,
				struct find_t *__buf);
extern unsigned _dos_findnext (struct find_t *__buf);
extern unsigned _dos_findclose (struct find_t *__buf);
extern unsigned _dos_freemem (unsigned __seg);
extern void _dos_getdrive (unsigned *__drive);
extern unsigned _dos_getfileattr (const char *__path, unsigned *__attributes);
extern void _dos_setdrive (unsigned __drive, unsigned *__total);
extern unsigned _dos_setfileattr (const char *__path, unsigned __attributes);
/* Note:  The Open Watcom C Library Reference has
	int _getdrive (void);
   but the actual <dos.h> and <direct.h> files in Open Watcom both give an
   unsigned return type.  */
extern unsigned _getdrive (void);

_LIBI86_ALT_INLINE unsigned
_dos_findclose (struct find_t *__buf)
{
  return 0;
}

_LIBI86_END_EXTERN_C

#endif
