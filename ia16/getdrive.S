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

	.code16
	.att_syntax prefix

	TEXT_ (_getdrive.S.LIBI86)
	.global	_getdrive
_getdrive:
	ENTER_BX_ (0)
	movb	$0x19,	%ah
	int	$0x21
	incw	%ax
	xorb	%ah,	%ah
	RET_ (0)
