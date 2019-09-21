/*
 * Copyright (c) 2018--2019 TK Chia
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

	.code16
	.att_syntax prefix

	.data
	.balign	2
	.global	__libi86_intr_dispatch
__libi86_intr_dispatch:
	.irpc	da, 0123
	.irpc	db, 01234567
	.irpc	dc, 01234567
	.hword	__libi86_intr_call_0\da\db\dc
	.endr
	.endr
	.endr
