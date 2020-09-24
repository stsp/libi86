/*
 * Macros to handle different IA-16 subarchitectures in assembly language code.
 *
 * Copyright (c) 2018--2020 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING2.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef _LIBI86_INTERNAL_ARCH_H_
#define _LIBI86_INTERNAL_ARCH_H_

#ifndef __ASSEMBLER__
# error "not using this file in assembly code!"
#endif

#ifdef __IA16_FEATURE_PUSH_IMM
	.arch	i186, jumps
/* Push a 16-bit constant.  Clobber the register REG if necessary.  */
# define PUSH_IMM_VIA_(imm, reg)	pushw	$(imm)
#else
	.arch	i8086, jumps
# define PUSH_IMM_VIA_(imm, reg)	movw	$(imm), reg; \
					pushw	reg
#endif

#endif
