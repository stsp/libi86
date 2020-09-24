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

#include "libi86/internal/call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (ungetch.S.LIBI86)
#ifdef __MSDOS__
	.global	_ungetch
	.weak	ungetch
_ungetch:
ungetch:
	ENTER_BX_(2)
	MOV_ARG0B_BX_(%al)
	movw	$__libi86_ungetch_buf, %bx
	cmpw	$0,	(%bx)
	jnz	.Lerr
	lahf				/* put something non-zero into %ah;
					   we know ZF is set at this point */
	movw	%ax,	(%bx)
	movb	$0,	%ah
	RET_(2)
.Lerr:
	movw	$-1,	%ax
	RET_(2)
#else
# warning "unknown target OS; ungetch (.) not implemented"
#endif
