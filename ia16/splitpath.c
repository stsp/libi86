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
#include <stdbool.h>
#include <string.h>
#include "libi86/stdlib.h"

static inline bool
drive_letter_p (char c)
{
  switch (c)
    {
    case 'A' ... 'Z':
    case 'a' ... 'z':
      return true;
    default:
      return false;
    }
}

static inline bool
path_sep_p (char c)
{
  switch (c)
    {
    case '/':
    case '\\':
      return true;
    default:
      return false;
    }
}

static const char *
copy_component (const char *p, size_t pn, char *buf, size_t bn)
{
  if (buf)
    {
      if (pn < bn)
	bn = pn + 1;
      memcpy (buf, p, bn - 1);
      buf[bn - 1] = 0;
    }

  return p + pn;
}

static void
skip_component (char *buf)
{
  if (buf)
    buf[0] = 0;
}

void
_splitpath (const char *path, char drive[_MAX_DRIVE], char dir[_MAX_DIR],
	    char fname[_MAX_FNAME], char ext[_MAX_EXT])
{
  const char *p = path, *q, *dir_end = NULL;
  char c;

  /* If there is a drive letter, extract it. */
  if (drive_letter_p (p[0]) && p[1] == ':')
    p = copy_component (p, 2, drive, _MAX_DRIVE);
  else
    skip_component (drive);

  /* If there is a directory path component, extract it. */
  q = p;
  while ((c = *q++) != 0)
    {
      if (path_sep_p (c))
	dir_end = q;
    }
  if (dir_end)
    p = copy_component (p, dir_end - p, dir, _MAX_DIR);
  else
    skip_component (dir);

  /* Extract the file name & extension. */
  q = strrchr (p, '.');
  if (q)
    {
      p = copy_component (p, q - p, fname, _MAX_FNAME);
      copy_component (p, strlen (p), ext, _MAX_EXT);
    }
  else
    {
      copy_component (p, strlen (p), fname, _MAX_FNAME);
      skip_component (ext);
    }
}
