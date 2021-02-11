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

	TEXT_ (dpmifreedosmemoryblock.S.LIBI86)
#ifdef __MSDOS__
	.global	_DPMIFreeDOSMemoryBlock
_DPMIFreeDOSMemoryBlock:
	ENTER_BX_ (2)
	MOV_ARG0W_BX_ (%dx)
	movw	$0x0101, %ax
	int	$0x31
	sbbw	%ax,	%ax
	RET_ (2)
#else
# warning "unknown target OS; _DPMIFreeDOSMemoryBlock (.) not implemented"
#endif
