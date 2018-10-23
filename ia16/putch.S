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

#include "libi86/internal/call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	.text
#ifdef __MSDOS__
	.global	putch
putch:
/*
 * Open Watcom uses %ah == 0x60, but this does not really work if %dl == 0xff
 * (see Ralf Brown's Interrupt List).  So I use %ah == 0x02 instead.
 *
 * A subtle difference is that %ah == 0x02 checks for ^C, while %ah == 0x06
 * does not.
 */
	xchgw	%ax,	%dx
	pushw	%dx
	movb	$0x02,	%ah
	call	__libi86_conio_int21
	popw	%ax
	movb	$0,	%ah
	RET_(0)
#else
# warning "unknown target OS; kbhit () not implemented"
#endif
