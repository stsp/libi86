/*
 * Copyright (c) 2018--2019 TK Chia
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

/*
 * For the %cs value, this implementation simply fills in segread (.)'s text
 * segment value, following Open Watcom.  For the medium memory model, it
 * might make a bit more sense to fill in the _caller's_ %cs value...
 */

#include "libi86/internal/call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (segread.S.LIBI86)
	.global	segread
segread:
	ENTER_BX_(2)
	MOV_ARG0W_BX_CLOBBER_(%bx)
	movw	%es,	(%bx)
	movw	%cs,	2(%bx)
	movw	%ss,	4(%bx)
	movw	%ds,	6(%bx)
	RET_(2)
