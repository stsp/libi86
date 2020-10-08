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

	TEXT_ (dpmisimulaterealmodeint.S.LIBI86)
#ifdef __MSDOS__
	.global	_DPMISimulateRealModeInterrupt
_DPMISimulateRealModeInterrupt:
	ENTER_BX_ (10)
	pushw	%di
	pushw	%es
	LES_ARG6W_BX_ (%di)
	MOV_ARG4W_BX_ (%cx)
	MOV_ARG0B_BX_ (%al)
	MOV_ARG2B_BX_ (%ah)
	xchgw	%ax,	%bx
	movw	$0x0300, %ax
	int	$0x31
	sbbw	%ax,	%ax
	popw	%es
	popw	%di
	RET_ (10)
#else
# warning "unknown target OS; _DPMISimulateRealModeInterrupt not implemented"
#endif
