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
_lltoa (long long value, char *buffer, int radix)
{
  unsigned long long u_value;

  if (radix != 10 || value >= 0)
    return _ulltoa ((unsigned long long) value, buffer, radix);

  buffer[0] = '-';
  /* Avoid C11 undefined behaviour at negation, when value == LLONG_MIN... */
  u_value = (unsigned long long) -(value + 1) + 1;
  _ulltoa (u_value, buffer + 1, 10);
  return buffer;
}

_LIBI86_WEAK_ALIAS (_lltoa) char *
lltoa (long long, char *, int);
