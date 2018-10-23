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

/*
 * __libi86_conio_int21_zx initializes the <conio.h> subsystem is needed,
 * then invokes int $0x21 with the values of %ax, %bx, %cx, and %dx from the
 * caller, and returns the value of %al zero-extended to %ax.
 */

	.arch	i186, jumps
	.code16
	.att_syntax prefix

	.text

	.global	__libi86_conio_int21_zx
__libi86_conio_int21_zx:
	call	__libi86_conio_int21
	movb	$0,	%ah
	ret
