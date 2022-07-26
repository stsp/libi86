#
/*
 * Copyright (c) 2022 TK Chia
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
	.define	__getsysvars
__getsysvars:
	xor	bx, bx
	mov	es, bx
	movb	ah, 0x52
	int	0x21
	xchg	bx, ax
	mov	bx, sp
	mov	bx, 2(bx)
	mov	(bx), ax
	mov	2(bx), es
	xchg	bx, ax
	ret
#endif
