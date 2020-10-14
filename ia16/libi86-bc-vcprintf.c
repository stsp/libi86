/*
 * Copyright (c) 2020 TK Chia
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

#define _BORLANDC_SOURCE
#define _LIBI86_COMPILING_
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "conio.h"
#include "libi86/internal/graph.h"
#include "libi86/internal/acconfig.h"

int
vcprintf (const char *fmt, va_list ap)
{
  va_list ap_2;
  int res;

  /*
   * Find out how many characters we will be writing, so that we can allocate
   * a large enough buffer.
   */
  va_copy (ap_2, ap);
  res = snprintf (NULL, (size_t) 0, fmt, ap_2);
  va_end (ap_2);

  if (res < 0)
    return res;

  /*
   * Do an snprintf (...) into a temporary buffer, then write everything out
   * from there.
   */
  {
    size_t count = (size_t) res + 1;
    char buf[count];

    res = snprintf (buf, count, fmt, ap);

    if (res > 0)
      __libi86_vid_bc_outmem_do (buf, (size_t) res);
  }

  return res;
}
