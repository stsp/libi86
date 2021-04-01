/*
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

#define _LIBI86_COMPILING_
#include <stdio.h>
#include "conio.h"

#ifdef __MSDOS__
int
# ifdef _BORLANDC_SOURCE
getche (void)
# else
_getche (void)
# endif
{
  int ch = _getch ();
  if (ch != EOF)
    return putch (ch);
  return ch;
}

# ifndef _BORLANDC_SOURCE
_LIBI86_WEAK_ALIAS (_getche) int
getche (void);
# endif
#else
# warning "unknown host OS"
#endif