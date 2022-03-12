/*
 * Copyright (c) 2022 TK Chia
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

/*
 * Under gcc-ia16 with -mnewlib-autofloat-stdio, this module is linked in if
 * a program uses cscanf (...) or vcscanf (...) with floating-point
 * arguments.  This module will in turn trigger the linking in of floating-
 * -point vsscanf (...).
 */

#include "libi86/internal/call-cvt.h"

	.arch	i8086, jumps
	.code16
	.att_syntax prefix

	.global	__ia16_have_scanf_float.cscanf.v2
	.set	__ia16_have_scanf_float.cscanf.v2, 1
	.global	__ia16_have_scanf_float.vcscanf.v2
	.set	__ia16_have_scanf_float.vcscanf.v2, 1
	.global	__ia16_have_scanf_float.__libi86_bc_cscanf.v2
	.set	__ia16_have_scanf_float.__libi86_bc_cscanf.v2, 1
	.global	__ia16_have_scanf_float.__libi86_bc_vcscanf.v2
	.set	__ia16_have_scanf_float.__libi86_bc_vcscanf.v2, 1
	.section .autofloat_stdio.__ia16_have_scanf_float.vsscanf.v2, "a"
	.reloc	., R_386_NONE, __ia16_have_scanf_float.vsscanf.v2
