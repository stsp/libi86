#
/* Copyright (c) 2022 TK Chia
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

#if defined L_spawnl
	.define	__spawnl
__spawnl:
#elif defined L_spawnle
	.define	__spawnle
__spawnle:
#elif defined L_spawnlp
	.define	__spawnlp
__spawnlp:
#elif defined L_spawnlpe
	.define	__spawnlpe
__spawnlpe:
#else
# error "which function are we assembling?"
#endif
	sub	sp, 8
	push	es
	push	si
	push	di
	push	ds
	pop	es
	mov	di, sp
	add	di, 6
	lea	si, 8+2(di)
	movsw
	movsw
	mov	ax, si
	stosw
#if defined L_spawnle || defined L_spawnlpe
.find_envp:
	lodsw
	test	ax, ax
	jnz	.find_envp
	movsw
#else
	xor	ax, ax
	stosw
#endif
	pop	di
	pop	si
	pop	es
#if defined L_spawnlp || defined L_spawnlpe
	call	__spawnvpe
#else
	call	__spawnve
#endif
	add	sp, 8
	ret
