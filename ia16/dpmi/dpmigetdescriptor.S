/*
 * Copyright (c) 2020 TK Chia
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

	TEXT_ (dpmigetdescriptor.S.LIBI86)
#ifdef __MSDOS__
	.global	_DPMIGetDescriptor
_DPMIGetDescriptor:
	ENTER_BX_ (6)
	pushw	%di
	pushw	%es
	LES_ARG2W_BX_ (%di)
	MOV_ARG0W_BX_ (%bx)
	movw	$0x000b, %ax
	int	$0x31
	sbbw	%ax,	%ax
	popw	%es
	popw	%di
	RET_ (6)
#else
# warning "unknown target OS; _DPMIGetDescriptor (, ) not implemented"
#endif
