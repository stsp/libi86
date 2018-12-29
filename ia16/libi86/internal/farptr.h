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

#ifndef _LIBI86_I86_INTERNAL_FARPTR_H_
#define _LIBI86_I86_INTERNAL_FARPTR_H_

#define FP_SEG(__p)	((unsigned) \
			  ((unsigned long) (void __far *) (__p) >> 16))
#define FP_OFF(__p)	__builtin_ia16_FP_OFF (__p)
#define MK_FP(__s, __o)	((void __far *) \
			 ((unsigned long) (unsigned) (__s) << 16 | \
			  (unsigned) (__o)))
#define _FP_SEG(__p)	FP_SEG (__p)
#define _FP_OFF(__p)	FP_OFF (__p)
#define _MK_FP(__s, __o) MK_FP (__s, __o)

#endif
