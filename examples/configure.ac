dnl Copyright (c) 2020 TK Chia
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

set -x
m4_include([../aclocal.m4])
_LIBI86_ACK_FIX

AC_PREREQ(2.69)
AC_INIT([libi86],m4_esyscmd_s([
  if git diff --quiet HEAD; then
    TZ=UTC0 git log -n1 --oneline --date=short-local --format='%ad' | \
	    sed 's/-//g'
  else
    TZ=UTC0 date +%Y%m%d
  fi]),,[libi86])
AC_CONFIG_SRCDIR(drivei86.c)
AC_CONFIG_AUX_DIR(..)

if test "${enable_shared}" = "yes" ; then
  echo "Shared libraries not supported for cross compiling, ignored"
fi

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_ARG_PROGRAM

AC_PROG_CC
AC_CHECK_TOOL(AS, as, false)
AC_CHECK_TOOL(LD, ld, false)

host_makefile_frag_path=
host_makefile_frag=
AC_SUBST(host_makefile_frag_path)
AC_SUBST(host_makefile_frag)
AC_SUBST(libi86_hostdir)
AC_SUBST(ac_compiler_gnu)
_LIBI86_SET_HOST_DIR

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
