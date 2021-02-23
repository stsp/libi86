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
extern unsigned _dos_commit (int __handle);
extern unsigned _dos_findfirst (const char *__path, unsigned __attr,
				struct find_t *__buf);
extern unsigned _dos_findnext (struct find_t *__buf);
extern unsigned _dos_findclose (struct find_t *__buf);
extern unsigned _dos_freemem (unsigned __seg);
extern void _dos_getdrive (unsigned *__drive);
extern unsigned _dos_getfileattr (const char *__path, unsigned *__attributes);
extern void _dos_setdrive (unsigned __drive, unsigned *__total);
extern unsigned _dos_setfileattr (const char *__path, unsigned __attributes);
/*
 * Note:  The Open Watcom C Library Reference has
 *	int _getdrive (void);
 * but the actual <dos.h> and <direct.h> files in Open Watcom both give an
 * unsigned return type.
 */
extern unsigned _getdrive (void);

#ifdef __IA16_FEATURE_ATTRIBUTE_INTERRUPT
typedef void __far (*__libi86_isr_t) (/* ... */)
				     __attribute__ ((__interrupt__));
extern __libi86_isr_t _dos_getvect (unsigned __intr_no);
extern void _dos_setvect (unsigned __intr_no, __libi86_isr_t __isr);
#else
typedef const void __far *__libi86_isr_t;
extern __libi86_isr_t _dos_getvect (unsigned __intr_no)
		      _LIBI86_WARNING ("_dos_getvect (.) not fully supported: "
				       "interrupt attribute unrecognized");
/*
 * If the compiler does not know about __attribute__ ((interrupt)) at all,
 * it may simply ignore the attribute & treat interrupt routines as normal
 * routines.
 *
 * This may result in incorrect code for interrupt routines, which will be
 * bad.
 *
 * To try to avert such a situation, if a program tries to install its own
 * interrupt handler with _dos_setvect (, ), & __attribute__ ((interrupt))
 * is not recognized, then flag an error.
 */
extern void _dos_setvect (unsigned __intr_no, __libi86_isr_t __isr);
	    _LIBI86_ERROR ("_dos_setvect (.) not supported: "
			   "interrupt attribute unrecognized");
#endif

_LIBI86_ALT_INLINE unsigned
_dos_findclose (struct find_t *__buf)
{
  return 0;
}

_LIBI86_END_EXTERN_C

#endif
