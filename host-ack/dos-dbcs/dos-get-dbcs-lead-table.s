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
#include "libi86/internal/acconfig.h"

#ifdef __MSDOS__
# ifndef _LIBI86_INTERNAL_HAVE__DOS_GET_DBCS_LEAD_TABLE
	.define	__dos_get_dbcs_lead_table
__dos_get_dbcs_lead_table:
# endif
	.define	___libi86_msdos_get_dbcs_lead_table
___libi86_msdos_get_dbcs_lead_table:
	call	___libi86_get_osmajor_osminor
	push	bp
	push	si
	push	di
	push	ds
	push	es
	cmpb	al, 3
	mov	ax, 0x6300
	mov	si, -1
	jb	.18
	int	0x21
.11:
	inc	si
	jz	.19
	dec	si
	testb	al, al
	jnz	.19
	cmpb	(si), al
	jz	.19
	xchg	si, ax
	mov	dx, ds
.12:
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	bp
	mov	bx, sp
	mov	bx, 2(bx)
	mov	(bx), ax
	mov	2(bx), dx
	ret
.18:
	stc
	int	0x21
	sbbb	al, al
	jmp	.11
.19:
	xor	ax, ax
	cwd
	jmp	.12
#endif
