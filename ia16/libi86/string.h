/*
 * Copyright (c) 2018 TK Chia
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

#ifndef _LIBI86_LIBI86_STRING_H_
#define _LIBI86_LIBI86_STRING_H_

#include <libi86/internal/cdefs.h>

_LIBI86_BEGIN_EXTERN_C

__libi86_fpv _fmemcpy (__libi86_fpv __dest, __libi86_fpcv __src,
		       __SIZE_TYPE__ __n);
__libi86_fpv _fmemmove (__libi86_fpv __dest, __libi86_fpcv __src,
			__SIZE_TYPE__ __n);

_LIBI86_END_EXTERN_C

#endif
