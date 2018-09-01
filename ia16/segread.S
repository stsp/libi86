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

#include "call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	.text
	.global	segread
segread:
	ENTER_BX_(2)
	MOV_ARG0W_BX_(%bx)
	movw	%es,	(%bx)
	movw	%cs,	2(%bx)
	movw	%ss,	4(%bx)
	movw	%ds,	6(%bx)
	RET_(2)
