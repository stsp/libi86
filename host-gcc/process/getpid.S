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

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/acconfig.h"

#ifndef __IA16_FEATURE_DOSX
	.arch	i8086, jumps
#else
	.arch	i286, jumps
#endif
	.code16
	.att_syntax prefix

/*
 * Get the current process identifier.  For MS-DOS, use the Program Segment
 * Prefix (PSP) segment as the PID, unless the system supports an actual
 * `getpid' syscall, e.g. European MS-DOS 4.0.
 *
 * Special case for the CauseWay DOS extender: CauseWay is able to create
 * "subprocesses" each of which has its own protected mode "PSP".  If we are
 * running under both European MS-DOS and CauseWay, combine the pid from
 * MS-DOS with the current CauseWay "PSP" to form a 32-bit pid.  Do not do
 * this though if the CauseWay PSP is not the child of any parent PSP.
 */

#define epsp_parent	0x100

	TEXT_ (dos_getpid.S.LIBI86)

/* If in DOS extender mode, force linking in libi86-init-causeway.S. */
#ifdef __IA16_FEATURE_DOSX
	.reloc	., R_386_NONE, __libi86_have_init_causeway
#endif

	.global	__libi86_getpid
__libi86_getpid:
#ifndef _LIBI86_INTERNAL_HAVE__GETPID
	.global	_getpid
_getpid:
#endif
#ifndef _LIBI86_INTERNAL_HAVE_GETPID
	.global	getpid
getpid:
#endif
	movb	$0x87,	%ah
	stc
	int	$0x21
	jnc	.eur_dos
	movw	_psp,	%ax
#ifndef __IA16_FEATURE_DOSX
.eur_dos:
#endif
.norm_dos:
	xorw	%dx,	%dx
	RET_ (0)
#ifdef __IA16_FEATURE_DOSX
.eur_dos:
	movw	_psp,	%dx
	pushw	%ds
	movw	%dx,	%ds
	cmpw	epsp_parent, %dx
	popw	%ds
	jz	.norm_dos
	shrw	$2,	%dx		/* try to ensure pid is positive... */
	RET_ (0)
#endif
