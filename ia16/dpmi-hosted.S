/*
 * Copyright (c) 2020 TK Chia
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

#include "libi86/internal/call-cvt.h"
#include "libi86/internal/acconfig.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	TEXT_ (dpmi_hosted.S.LIBI86)
#ifdef __MSDOS__
# ifndef _LIBI86_INTERNAL_HAVE___DPMI_HOSTED
	.weak	__DPMI_hosted
__DPMI_hosted:
# endif
	.global	__libi86_DPMI_hosted
__libi86_DPMI_hosted:
	/*
	 * We could try using int 0x2f, ax = 0x1686, but the _DOS Protected
	 * Mode Interface (DPMI)_ version 1.0 warns,
	 *
	 *	"This function should not be used to determine if a DPMI host
	 *	 is present. A client should make sure that DPMI services are
	 *	 available before calling this function; otherwise, the
	 *	 results returned by the function may not be valid."
	 *
	 * So instead, we do some tests with the segment registers to see if
	 * they behave like in real mode (or V86 mode) or like in protected
	 * mode.
	 */
	subw	$0x30,	%sp
	movw	%sp,	%bx
	movw	$1,	%ax
	movb	%ah,	(%bx)
	movb	%al,	0x10(%bx)
	movb	$2,	0x20(%bx)
	/*
	 * Frob the lowest bit of the segment value.
	 *   * In real or V86 mode, the resulting value will refer to the
	 *     segment one paragraph higher or lower than our data segment.
	 *   * In protected mode, this will refer to the same segment, with
	 *     only a different requested privilege level (RPL).
	 */
	movw	%ds,	%cx
	xorb	%al,	%cl
	movw	%cx,	%ds
	/* Return 1 if running under DPMI, -1 otherwise. */
	cmpb	%al,	0x10(%bx)
	jz	.pm
	negw	%ax
.pm:
	movw	%ss,	%cx
	movw	%cx,	%ds
	addw	$0x30,	%sp
	RET_ (0)
#else
# warning "unknown target OS; __DPMI_hosted () not implemented"
#endif
