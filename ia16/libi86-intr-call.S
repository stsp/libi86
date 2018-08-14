/*
 * Copyright (c) 2018 TK Chia
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

/*
 * This file will be compiled with intr_no spanning values from 0000 to 0377
 * (numerically, 0x00 to 0xff).
 */

#APP
#define P_(x, y)	P2_ (x, y)
#define P2_(x, y)	x ## y

	.code16
	.att_syntax prefix

	.text
	.global	P_ (__libi86_intr_call_, intr_no)
P_ (__libi86_intr_call_, intr_no):
#if intr_no == 3
	int3
	ret
#else
	int	$(intr_no)
# ifdef __MSDOS__
#   if intr_no == 0x25 || intr_no == 0x26
	jc	0f
	popfw
	clc
	ret
0:
	popfw
	stc
#   endif
# endif
	ret
#endif
