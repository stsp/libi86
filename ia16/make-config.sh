#!/bin/sh
#
# Copyright (c) 2018 TK Chia
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; see the file COPYING3.LIB.  If not see
# <http://www.gnu.org/licenses/>.

# Given libi86.a, automatically generate a header file which defines macros
# for routines implemented in the current libi86.

set -e

if [ $# = 0 ]; then
  echo "usage: `basename "$0"` libi86.a" >&2
  exit 1
fi

cat <<FIN
/* Automatically generated by `basename "$0"` on `TZ=UTC0 date +%Y%m%d`. */

#ifndef _LIBI86_LIBI86_CONFIG_H_
#define _LIBI86_LIBI86_CONFIG_H_

#include <libi86/internal/acconfig.h>

FIN

${NM-'ia16-elf-nm'} -P "$@" | \
  ${AWK-'awk'} '!/:$/ && !/^_[_ABCDEFGHIJKLMNOPQRSTUVWXYZ]/ && / [TDBW] / {
		  if ($2 == "T" || $2 == "D" || $2 == "B" || $2 == "W")
		    {
		      print "/* " $1 " is implemented. */"
		      print "#define _LIBI86_HAVE_" toupper($1) " 1"
		      print ""
		    }
		}'

cat <<FIN
#endif
FIN
