#!/bin/sh
# Copyright (c) 2018 TK Chia
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; ; see the file COPYING.  If not see
# <http://www.gnu.org/licenses/>.

# Script to postprocess the screen output of a DOS program as captured by
# capture.exe .  This script removes trailing empty lines and trailing spaces
# from non-empty lines.

exec sed -n -e 's/ *$//' \
	    -e 'H' \
	    -e '/./ { g; s/^.//; p; s/.//g; h; }'
