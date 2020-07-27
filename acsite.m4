dnl Copyright (c) 2020 TK Chia
dnl
dnl This file is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU Lesser General Public License as published by
dnl the Free Software Foundation; either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public License
dnl along with this program; see the file COPYING3.LIB.  If not see
dnl <http://www.gnu.org/licenses/>.

dnl Set default options to build & install for MS-DOS host.  -- tkchia 20200727
AC_DEFUN([_LIBI86_DEFAULT_OPTIONS],[
m4_divert_push([DEFAULTS])dnl
set x --host=ia16-elf --disable-elks-libc \
  --exec-prefix='${prefix}'/ia16-elf [$]{1+"[$][@]"}
shift
m4_divert_pop([DEFAULTS])])
