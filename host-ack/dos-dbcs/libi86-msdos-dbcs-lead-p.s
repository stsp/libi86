#
/*
 * Copyright (c) 2021 TK Chia
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

#include "libi86/internal/sect.h"

#ifdef __MSDOS__
	.define	___libi86_msdos_dbcs_lead_p
___libi86_msdos_dbcs_lead_p:
	mov	bx, sp
	mov	dx, 4(bx)
	mov	cx, 6(bx)
	movb	al, 2(bx)
	mov	bx, dx
	or	bx, cx
	jnz	.20
	xchg	dx, ax
	ret
.20:
	push	es
	push	si
	mov	si, dx
	mov	es, cx
	xchg	cx, ax
.21:
	eseg lodsw
	testb	al, al
	jz	.29
	cmpb	cl, al
	jb	.21
	cmpb	cl, ah
	ja	.21
	movb	al, 1
.29:
	cbw
	pop	si
	pop	es
	ret
#endif
