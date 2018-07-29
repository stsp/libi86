dnl Copyright (c) 2018 TK Chia
dnl
dnl This file is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; see the file COPYING3.  If not see
dnl <http://www.gnu.org/licenses/>.

dnl
dnl
dnl Derived from libgloss/cr16/configure.in in Newlib.
AC_PREREQ(2.69)
AC_INIT
AC_CONFIG_SRCDIR(bios-memsize.S)

if test "${enable_shared}" = "yes" ; then
  echo "Shared libraries not supported for cross compiling, ignored"
fi

if test "$srcdir" = "." ; then
  if test "${with_target_subdir}" != "." ; then
    libi86_topdir="${srcdir}/${with_multisrctop}../.."
  else
    libi86_topdir="${srcdir}/${with_multisrctop}.."
  fi
else
  libi86_topdir="${srcdir}/.."
fi
AC_CONFIG_AUX_DIR($libi86_topdir)

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_ARG_PROGRAM

AC_PROG_INSTALL

AC_PROG_CC
AC_CHECK_TOOL(AS, as, :)
AC_CHECK_TOOL(AR, ar, :)
AC_CHECK_TOOL(LD, ld, :)
AC_PROG_RANLIB

host_makefile_frag_path=
host_makefile_frag=
AC_SUBST(host_makefile_frag_path)
AC_SUBST(host_makefile_frag)

AC_CONFIG_FILES([Makefile],
[. ${libi86_topdir}/config-ml.in],
srcdir="${srcdir}"
target="${target}"
with_multisubdir="${with_multisubdir}"
ac_configure_args="--enable-multilib ${ac_configure_args}"
CONFIG_SHELL="${CONFIG_SHELL-/bin/sh}"
libi86_topdir="${libi86_topdir}"
CC="${CC}"
LDFLAGS="${LDFLAGS}"
)
AC_OUTPUT
