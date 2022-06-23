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
m4_include([../aclocal.m4])
_LIBI86_GCC_ACK_FIX

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
AC_CONFIG_AUX_DIR(..)

if test "${enable_shared}" = "yes" ; then
  echo "Shared libraries not supported for cross compiling, ignored"
fi

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_ARG_PROGRAM

AC_PROG_INSTALL

AC_PROG_CC
AC_CHECK_TOOL(AR, aal, false)
dnl make-config.sh uses nm and awk.
AC_CHECK_TOOL(NM, anm, false)
AC_PROG_AWK

AC_CHECK_DECLS([__ACK],,dnl
[AC_MSG_WARN([supposedly using Amsterdam Compiler Kit,])
AC_MSG_WARN([but __ACK undefined])])
AC_CHECK_FUNC([_sys_seterrno],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE__SYS_SETERRNO],[1])],,
	      [long _sys_seterrno (unsigned);])
m4_include([../check-common.m4])

AC_CHECK_DECLS([__MSDOS__],[libi86_ackhost=msdos86],[libi86_ackhost=pc86])
AC_SUBST(libi86_ackhost)

host_makefile_frag_path=
host_makefile_frag=
AC_SUBST(host_makefile_frag_path)
AC_SUBST(host_makefile_frag)

AC_CONFIG_FILES([Makefile direct.h:../common/direct.h.in])
AC_DEFINE_UNQUOTED([_LIBI86_VERSION],[${PACKAGE_VERSION}L])
AC_CONFIG_HEADERS([libi86/internal/acconfig.h])
AC_OUTPUT
