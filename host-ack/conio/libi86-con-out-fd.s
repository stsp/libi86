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
 * is an output file descriptor which is open to the console device `CON'.
 */

	.define	___libi86_con_out_fd_init
___libi86_con_out_fd_init:
	push	(_errno)
	mov	bx, 1			! check if fd 1 is console output
	push	bx
	mov	ax, 0x4400
	int	0x21
	jc	.opencon
	notb	dl
	testb	dl, 0x82
	jnz	.opencon
.done:	pop	bx
	pop	(_errno)		! if so, we are done; patch
					! ourselves with a `ret' instruction
	cseg movb (___libi86_con_out_fd_init), 0xc3
	ret
.opencon:				! otherwise, open an output fd on `CON'
	mov	ax, ___libi86_con_name	! fd no. 1 above doubles as O_WRONLY (:
	push	ax
	call	_open
	pop	bx
	cmp	ax, -1			! if this fails, just use fd 1 :-\
	jz	.done			! otherwise record the new fd to use
	mov	(___libi86_con_out_fd), ax
	jmp	.done

	.sect	.data

	.define	___libi86_con_out_fd
___libi86_con_out_fd:
	.data2	1
