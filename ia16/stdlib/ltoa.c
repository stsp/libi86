/*
 * Copyright (c) 2021 TK Chia
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

#define _LIBI86_COMPILING_
#include "libi86/stdlib.h"

char *
_ltoa (long value, char *buffer, int radix)
{
  /* Ugh. */
  if (radix == 10)
    return _lltoa ((long long) value, buffer, radix);
  else
    return _ulltoa ((unsigned long long) (unsigned long) value, buffer, radix);
}

_LIBI86_WEAK_ALIAS (_ltoa) char *
ltoa (long, char *, int);
