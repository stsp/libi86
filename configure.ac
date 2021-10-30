dnl Copyright (c) 2018--2020 TK Chia
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

dnl For the actual build, this script mostly just hands over to
dnl host-*/configure, which sets things up for an automated multilib build.
dnl It seems that setting up a project top-level directory for a multilib
dnl build is not supported.
dnl
dnl For tests, however, this script _does_ do the actual work of setting up
dnl the test suite runs, using the GNU Autotools' experimental Autotest
dnl feature.	-- tkchia 20210519

_LIBI86_ACK_FIX

AC_PREREQ(2.69)
AC_INIT([libi86],m4_esyscmd_s([
  if git diff --quiet HEAD 2>/dev/null; then
    TZ=UTC0 git log -n1 --oneline --date=short-local --format='%ad' | \
	    sed 's/-//g'
  else
    TZ=UTC0 date +%Y%m%d
  fi]),,[libi86])

_LIBI86_DEFAULT_OPTIONS

AC_DISABLE_OPTION_CHECKING
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
case "$host_alias" in
  ia16-*-msdosack)
    if test -z "$CC"; then
      AC_CHECK_PROG(CC, ack-cc, ack-cc -mmsdos86, ack -mmsdos86)
      export CC
    fi ;;
esac
AC_PROG_CC
AC_PROG_CXX
AC_PATH_PROG(DOSEMU, dosemu, /bin/false)
AC_TYPE_LONG_LONG_INT

AC_SUBST(ac_compiler_gnu)
AC_SUBST(ac_cv_type_long_long_int)
_LIBI86_SET_AND_CONFIG_HOST_DIR

AC_CONFIG_SUBDIRS([examples])
AC_CONFIG_TESTDIR([tests])
AC_CONFIG_FILES([Makefile tests/atlocal])
AC_OUTPUT
