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

/*
 * Internal implementation of _setvideomode (.) for the "classical" MDPA,
 * CGA, & EGA video modes.
 */

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/arch.h"
#include "graph.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (setvideomode_default.S.LIBI86)

.Lctor_setvideomode:
	movw	$0x4f03, %ax		/* Try to retrieve the initial */
	int	$0x10			/* mode via VESA */
	cmpw	$0x004f, %ax
	jz	.error
	testw	$~0x807f, %bx		/* If this is a SuperVGA video */
	jz	.good			/* mode & not a classical video */
	movb	$_TEXTC80, %bl		/* mode, then to simplify things we
					   treat _TEXTC80 as the "default"
					   mode to return to (in fact Open
					   Watcom does not really handle
					   this case!) */
.good:					/* If not a SuperVGA mode, just */
	movb	%bl,	.Ldef_mode	/* remember the actual screen mode */
	RET_ (0)
.error:
	movb	$0x0f,	%ah		/* If VESA fails, use the classical */
	int	$0x10			/* interface to get the video mode */
	andb	$0x7f,	%al
	movb	%al,	.Ldef_mode
	RET_ (0)

	.global	__libi86_setvideomode_default
__libi86_setvideomode_default:
	movb	.Ldef_mode, %al
	cbtw
#ifdef __IA16_CALLCVT_REGPARMCALL
	JMP_ (__libi86_setvideomode_nonsvga)
#else
	pushw	%ax
	CALL_ (__libi86_setvideomode_nonsvga)
# ifndef __IA16_CALLCVT_STDCALL
	popw	%dx
# endif
	RET_ (0)
#endif

	.section .ctors.65535

	.balign	2
	TEXT_PTR_ (.Lctor_setvideomode)

	.bss

.Ldef_mode:
	.skip	1
	.balign	2
