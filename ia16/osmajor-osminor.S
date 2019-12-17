/*
 * Copyright (c) 2019 TK Chia
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

	TEXT_ (osmajor_osminor.S.LIBI86)
.Lctor_osmajor_osminor:
	movb	$0x30,	%ah
	int	$0x21
	movw	%ax,	0f
	RET_(0)

	.section .ctors.65535

	.balign	2
	TEXT_PTR_ (.Lctor_osmajor_osminor)

	.bss

/* (1) Make these variables weak, in case the underlying libc has its own
       definitions (and initializations) of _osmajor and _osminor.
   (2) The variables must be arranged like this, so that a single store of
       %ax can correctly fill both _osmajor and _osminor.  */
0:
	.weak	_osmajor
_osmajor:
	.skip	1
	.weak	_osminor
_osminor:
	.skip	1
