/*
 * Copyright (c) 2021 TK Chia
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

	.code16
	.att_syntax prefix

	TEXT_ (dos_setdrive.S.LIBI86)
	.global	_dos_setdrive
_dos_setdrive:
	ENTER_BX_ (4)
#ifndef __IA16_CALLCVT_REGPARMCALL
	MOV_ARG0W_BX_ (%dx)
	MOV_ARG2W_BX_ (%bx)
#else
	movw	%dx,	%bx
	xchgw	%ax,	%dx
#endif
	decw	%dx
	testb	%dh,	%dh		/* if drive number > 0x100, then */
	jz	0f			/* map it to something bogus; but */
	movb	$0xff,	%dl		/* we still need to return the */
0:					/* drive count... */
	movb	$0x0e,	%ah
	int	$0x21
	xorb	%ah,	%ah
	movw	%ax,	(%bx)
	RET_ (4)
