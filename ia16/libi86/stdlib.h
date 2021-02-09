/*
 * Copyright (c) 2019--2021 TK Chia
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

#ifndef _LIBI86_LIBI86_STDLIB_H_
#define _LIBI86_LIBI86_STDLIB_H_

#include <libi86/internal/cdefs.h>
#include <libi86/internal/acconfig.h>

_LIBI86_BEGIN_EXTERN_C

/*
 * Try to defer to the underlying C runtime's idea of the maximum length of
 * file name paths.
 *
 * If _PATH_MAX is defined in <sys/syslimits.h> (as detected by the
 * `configure' script), or PATH_MAX is defined in <limits.h>, then use that.
 *
 * Otherwise, follow Open Watcom in defining a maximum length of 144. 
 * (FIXME: why 144?).
 */
#if defined _LIBI86_INTERNAL_HAVE__PATH_MAX
/*
 * Try to coax <sys/syslimits.h> into giving us _only_ _PATH_MAX.  If
 * <sys/syslimits.h> does not honour __need__PATH_MAX, then too bad.  :-|
 */
# define __need__PATH_MAX
# ifdef PATH_MAX
#   include <sys/syslimits.h>
# else
#   include <sys/syslimits.h>
#   ifdef PATH_MAX
#     warning "<libi86/stdlib.h> pollutes user namespace with PATH_MAX"
#   endif
# endif
# undef __need_PATH_MAX
# define _MAX_PATH	_PATH_MAX
#elif defined _LIBI86_INTERNAL_HAVE_PATH_MAX
# include <limits.h>
# warning "<libi86/stdlib.h> pollutes user namespace with <limits.h> stuff"
# define _MAX_PATH	PATH_MAX
#else
# define _MAX_PATH	144
#endif
#define _MAX_DRIVE	3
#define _MAX_DIR	_MAX_PATH
#define _MAX_FNAME	9
#define _MAX_EXT	5

extern unsigned _psp;
extern unsigned char _osmajor, _osminor;

extern int _makepath (char __path[_MAX_PATH], const char *__drive,
		      const char *__dir, const char *__fname,
		      const char *__ext);
extern void _splitpath (const char *__path, char __drive[_MAX_DRIVE],
			char __dir[_MAX_DIR], char __fname[_MAX_FNAME],
			char __ext[_MAX_EXT]);

_LIBI86_END_EXTERN_C

#endif
