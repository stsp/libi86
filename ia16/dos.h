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

#ifndef _LIBI86_DOS_H_
#define _LIBI86_DOS_H_

#include <libi86/internal/cdefs.h>

/* <dos.h> should contain everything that <i86.h> has (at least for the IA-16
   platform...).  */
#include <i86.h>

_LIBI86_BEGIN_EXTERN_C

extern int bdos (int __dos_func, unsigned __dx, unsigned __al);
extern int bdosptr (int _dos_func, void *__dx, unsigned __al);
extern int intdos (const union REGS *, union REGS *);
extern int intdosx (const union REGS *, union REGS *, struct SREGS *);

_LIBI86_END_EXTERN_C

#endif
