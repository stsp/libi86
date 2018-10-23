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

/*
 * __libi86_conio_int21 invokes int $0x21 with the values of %ax, %bx, %cx,
 * and %dx from the caller, and returns whatever register values and flags are
 * returned by int $0x21.
 *
 * __libi86_conio_int21 also does some initialization for <conio.h> if
 * needed.
 *
 * Specifically, the MS-DOS implementation of the <conio.h> routines kbhit
 * kbhit (), getch (), etc. here do their work by using int $0x21 calls, but
 * these calls assume that standard input (file descriptor 0) and standard
 * output (file descriptor 1) point to the console device `CON'.
 *
 * Unlike Turbo C nor Open Watcom C/C++ --- which simply go ahead and read
 * whatever is on fd 0, and write to fd 1 --- I arrange to ensure that fd 0
 * and fd 1 are indeed open to the console device, and reopen them otherwise.
 */

	.arch	i186, jumps
	.code16
	.att_syntax prefix

	.text

.Lcheck_fd:
	movw	$0x4400, %ax
	movw	%si,	%bx
	int	$0x21
	jc	.Lreopen
	andw	%di,	%dx
	cmpw	%di,	%dx
	jz	.Ldone
.Lreopen:
	movw	%si,	%ax
	movb	$0x3d,	%ah
	movw	$.Lcon,	%dx
	int	$0x21
	jc	.Ldone
	cmpw	%si,	%ax
	jz	.Ldone
	xchgw	%ax,	%bx
	pushw	%bx
	movb	$0x46,	%ah
	movw	%si,	%cx
	int	$0x21
	popw	%bx
	movb	$0x3e,	%ah
	int	$0x21
.Ldone:
	ret

.Linit:
	movw	$.Lnoinit, .Ldispatch
#ifdef __IA16_FEATURE_PUSHA
	pushaw
#else
	pushw	%ax
	pushw	%bx
	pushw	%cx
	pushw	%dx
	pushw	%si
	pushw	%di
#endif
	/* Check if fd 0 is the console input; if not, open it to `CON'. */
	xorw	%si,	%si
	movw	$0x81,	%di
	call	.Lcheck_fd
	/* Do the same for fd 1. */
	incw	%si
	incw	%di
	call	.Lcheck_fd
#ifdef __IA16_FEATURE_PUSHA
	popaw
#else
	popw	%di
	popw	%si
	popw	%dx
	popw	%cx
	popw	%bx
	popw	%ax
#endif
	.balign	2
.Lnoinit:
	int	$0x21
	ret

	.balign	2

	.global	__libi86_conio_int21
__libi86_conio_int21:
	jmpw	*.Ldispatch

	.balign	2

	.section .rodata

.Lcon:
	.asciz	"CON"

	.data

.Ldispatch:
	.hword	.Linit
