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

#ifndef _LIBI86_LIBI86_INTERNAL_PORT_IO_H_
#define _LIBI86_LIBI86_INTERNAL_PORT_IO_H_

#include <libi86/internal/acconfig.h>
#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

/*
 * I did not add an "N" constraint for immediate port numbers until late
 * October 2018, so some versions of gcc-ia16 may not understand it.  For
 * those, I use a combination of integer constraints that together cover the
 * range 0 ... 0xff of possible immediate port numbers.  -- tkchia
 */
#if _LIBI86_HAVE_ASM_N_CONSTRAINT - 0
# define _LIBI86_IO_PORT_CONSTRAINT "Nd"
#else
# define _LIBI86_IO_PORT_CONSTRAINT "LomZLbmd"
#endif

_LIBI86_ALT_INLINE
unsigned __libi86_inp (unsigned __port)
{
  unsigned char __val;
  __asm volatile ("inb %1, %0" : "=Ral" (__val)
			       : _LIBI86_IO_PORT_CONSTRAINT (__port));
  return (unsigned) __val;
}

_LIBI86_ALT_INLINE
unsigned __libi86_inpw (unsigned __port)
{
  unsigned __val;
  __asm volatile ("inw %1, %0" : "=a" (__val)
			       : _LIBI86_IO_PORT_CONSTRAINT (__port));
  return __val;
}

_LIBI86_ALT_INLINE
unsigned __libi86_outp (unsigned __port, unsigned __val)
{
  __asm volatile ("outb %1, %0" : /* no outputs */
				: _LIBI86_IO_PORT_CONSTRAINT (__port),
				  "Ral" ((unsigned char) __val));
  return __val;
}

_LIBI86_ALT_INLINE
unsigned __libi86_outpw (unsigned __port, unsigned __val)
{
  __asm volatile ("outw %1, %0" : /* no outputs */
				: _LIBI86_IO_PORT_CONSTRAINT (__port),
				  "a" (__val));
  return __val;
}

_LIBI86_END_EXTERN_C

#endif
