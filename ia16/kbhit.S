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
	.global	_kbhit
	.weak	kbhit
_kbhit:
kbhit:
	movw	__libi86_ungetch_buf, %ax
	testw	%ax,	%ax
	jnz	.Lyes
	movb	$0x0b,	%ah
	jmp	__libi86_conio_int21_zx
.Lyes:
	RET_(0)
#else
# warning "unknown target OS; kbhit () not implemented"
#endif
