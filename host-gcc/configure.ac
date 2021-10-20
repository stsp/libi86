dnl Copyright (c) 2018--2021 TK Chia
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions are
dnl met:
dnl
dnl   * Redistributions of source code must retain the above copyright
dnl     notice, this list of conditions and the following disclaimer.
dnl   * Redistributions in binary form must reproduce the above copyright
dnl     notice, this list of conditions and the following disclaimer in the
dnl     documentation and/or other materials provided with the distribution.
dnl   * Neither the name of the developer(s) nor the names of its
dnl     contributors may be used to endorse or promote products derived from
dnl     this software without specific prior written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
dnl IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
dnl TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
dnl PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
dnl HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
dnl TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
dnl PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
dnl LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
dnl NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
dnl SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

dnl Derived from libgloss/cr16/configure.in in Newlib.
set -x
AC_PREREQ(2.69)
AC_INIT([libi86],m4_esyscmd_s([
  if git diff --quiet HEAD; then
    TZ=UTC0 git log -n1 --oneline --date=short-local --format='%ad' | \
	    sed 's/-//g'
  else
    TZ=UTC0 date +%Y%m%d
  fi]),,[libi86])
AC_CONFIG_SRCDIR(bios/bios-memsize.S)
AC_CONFIG_SRCDIR(make-config.sh)

m4_include([../aclocal.m4])
_LIBI86_ACK_FIX

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
AC_CHECK_TOOL(AS, as, false)
AC_CHECK_TOOL(AR, ar, false)
AC_CHECK_TOOL(LD, ld, false)
AC_PROG_RANLIB
dnl make-config.sh uses nm and awk.
AC_CHECK_TOOL(NM, nm, false)
AC_PROG_AWK

AC_CHECK_TYPE([long long int],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE_LONG_LONG_INT],[1])])
dnl See <libi86/port-io.h>.
AC_MSG_CHECKING([if $CC understands the "N" asm constraint])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]],
				   [[unsigned x;
				     __asm ("" : "=r" (x) : "N" (1));]])],
		  [AC_DEFINE([_LIBI86_INTERNAL_HAVE_ASM_N_CONSTRAINT],[1])
		   AC_MSG_RESULT(yes)],
		  [AC_MSG_RESULT(no)])
dnl ...
AC_CHECK_FUNC([__DPMI_hosted],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE___DPMI_HOSTED],[1])])
AC_CHECK_HEADER([sys/syslimits.h],
		[AC_DEFINE([_LIBI86_INTERNAL_HAVE_SYS_SYSLIMITS_H],[1])])
if test yes = "$ac_cv_header_sys_syslimits_h"; then
  AC_CHECK_DECL([_PATH_MAX],
		[AC_DEFINE([_LIBI86_INTERNAL_HAVE__PATH_MAX],[1])],,
		[#include <sys/syslimits.h>])
fi
AC_CHECK_DECL([PATH_MAX],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE_PATH_MAX],[1])],,
	      [#include <limits.h>])
AC_CHECK_DECL([O_TEXT],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE_O_TEXT],[1])],,
	      [#include <fcntl.h>])
AC_CHECK_DECL([ENAMETOOLONG],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE_ENAMETOOLONG],[1])])

host_makefile_frag_path=
host_makefile_frag=
AC_SUBST(host_makefile_frag_path)
AC_SUBST(host_makefile_frag)

AC_CONFIG_FILES([Makefile],
[. ${libi86_topdir}/config-ml.in],
srcdir="${srcdir}"
host="${host}"
target="${target}"
with_multisubdir="${with_multisubdir}"
ac_configure_args="--enable-multilib ${ac_configure_args}"
CONFIG_SHELL="${CONFIG_SHELL-/bin/sh}"
libi86_topdir="${libi86_topdir}"
CC="${CC}"
LDFLAGS="${LDFLAGS}"
)
AC_DEFINE_UNQUOTED([_LIBI86_VERSION],[${PACKAGE_VERSION}L])
AC_CONFIG_HEADERS([libi86/internal/acconfig.h])
AC_OUTPUT
