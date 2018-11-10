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

#define _LIBI86_COMPILING_
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include "conio.h"

int
vcscanf (const char *fmt, va_list ap)
{
  char buf[UCHAR_MAX + 2], *p;

  buf[0] = (char) UCHAR_MAX;
  p = cgets (buf);
  if (! p)
    return EOF;

  return vsscanf (p, fmt, ap);
}