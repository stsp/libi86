#
/*
 * Copyright (c) 2018--2021 TK Chia
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the developer(s) nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "libi86/internal/sect.h"

/*
 * This module initializes the <conio.h> subsystem to make sure that there
 * is an input file descriptor which is open to the console device `CON'.
 *
 * This is (in a way) an improvement over the classical Turbo C and Open
 * Watcom C/C++'s <conio.h> code: they implement kbhit (), getch (), etc.,
 * using int $0x21 calls in a way that assume that standard input (fd 0)
 * points to `CON', whether or not it does.
 *
 * (Watcom in addition assumes that standard output, fd 1, points to `CON'. 
 * libi86 initializes the fd for console output in libi86-con-out-fd.c .)
 */

	.define	___libi86_con_in_fd_init
___libi86_con_in_fd_init:
	push	(_errno)
	xor	bx, bx			! check if fd 0 is console input
	push	bx
	mov	ax, 0x4400
	int	0x21
	jc	.opencon
	notb	dl
	testb	dl, 0x81
	jnz	.opencon
.done:	pop	bx
	pop	(_errno)		! if so, we are done; patch
					! ourselves with a `ret' instruction
	cseg movb (___libi86_con_in_fd_init), 0xc3
	ret
.opencon:				! otherwise, open an output fd on `CON'
	mov	ax, ___libi86_con_name	! fd no. 0 above doubles as O_RDONLY (:
	push	ax
	call	_open
	pop	bx
	cmp	ax, -1			! if this fails, just use fd 0 :-\
	jz	.done			! otherwise record the new fd to use
	mov	(___libi86_con_in_fd), ax
	xchg	bx, ax
	push	bx
	mov	ax, 0x4400		! store original device information
	int	0x21			! word so that we can restore the fd
	pop	bx			! to cooked mode
	xorb	dh, dh
	mov	(___libi86_con_in_info_word), dx
	testb	dl, 0x80		! if device, set to use raw binary
	jz	.done			! mode
	testb	dl, 0x20
	jnz	.done
	orb	dl, 0x20
	mov	ax, 0x4401
	int	0x21
					! register a function to be called
					! at exit which will restore cooked
					! mode
	mov	ax, ___libi86_con_in_fd_fini
	push	ax
	call	_atexit
	pop	ax
	jmp	.done

___libi86_con_in_fd_fini:
	mov	ax, 0x4401
	mov	bx, (___libi86_con_in_fd)
	mov	dx, (___libi86_con_in_info_word)
	int	0x21
	ret

	.sect	.bss

	.define	___libi86_con_in_fd
	.comm	___libi86_con_in_fd, 2
	.comm	___libi86_con_in_info_word, 2
