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

	.code16
	.att_syntax prefix

	.data
	.balign	2
	.global	__libi86_intr_dispatch
__libi86_intr_dispatch:
	inter_no = 0
	.rept	0x100
	.data
	.hword	0f
	.text
0:
	.if	inter_no == 3
	int3
	ret
	.else
	int	$inter_no
#ifndef __MSDOS__
	ret
#else
	/* MS-DOS `int $0x25' and `int $0x26' leave the original flags on
	   the stack (see RBIL, www.ctyme.com/intr/rb-4117.htm).  */
	.if	inter_no == 0x25
1:
	jc	2f
	popfw
	clc
	ret
2:
	popfw
	stc
	ret
	.else
	.if	inter_no == 0x26
	jmp	1b
	.else
	ret
	.endif
	.endif
#endif
	.endif
	inter_no = inter_no + 1
	.endr
