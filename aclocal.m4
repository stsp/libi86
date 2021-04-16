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

dnl Set default options to build & install for MS-DOS host.  -- tkchia 20200727
AC_DEFUN([_LIBI86_DEFAULT_OPTIONS],dnl
[m4_divert_push([DEFAULTS])dnl
set x --host=ia16-elf --disable-elks-libc \
  --exec-prefix='${prefix}'/ia16-elf [$]{1+"[$][@]"}
shift
m4_divert_pop([DEFAULTS])])

dnl Patch autoconf so that it works with the Amsterdam Compiler Kit's
dnl non-standard default output file names.  (This is for future use ---
dnl support for ACK is still work in progress.)  -- tkchia 20210409
AC_DEFUN([_LIBI86_ACK_FIX],dnl
[m4_define([_LIBI86_SAVE_AC_COMPILER_EXEEXT_DEFAULT],dnl
m4_defn([_AC_COMPILER_EXEEXT_DEFAULT]))dnl
m4_define([_AC_COMPILER_EXEEXT_DEFAULT],dnl
[m4_bpatsubst(m4_defn([_LIBI86_SAVE_AC_COMPILER_EXEEXT_DEFAULT]),dnl
[a_out\.exe],dnl
[a_out.exe cpm.com e.out linux386.exe linux68k.exe linuxmips.exe ]dnl
[linuxppc.exe msdos86.exe osx386.exe osxppc.exe pc86.img qemuppc.img ]dnl
[raspberrypi.bin])])])

AC_DEFUN([_LIBI86_DO_SET_HOST_DIR],dnl
[if test $ac_compiler_gnu = yes; then
  libi86_hostdir=host-gcc
  $1([host-gcc])
else
  dnl If not using GCC, then assume ACK.
  libi86_hostdir=host-ack
  $1([host-ack])
fi
AC_SUBST(libi86_hostdir)])

dnl Determine which host-specific subdirectory to build the actual libi86
dnl code from.
AC_DEFUN([_LIBI86_SET_HOST_DIR],[_LIBI86_DO_SET_HOST_DIR([m4_if])])

dnl Like _LIBI86_SET_HOST_DIR, but also run `configure' in the host-specific
dnl subdirectory.  This should be used by the top-level `configure' script.
AC_DEFUN([_LIBI86_SET_AND_CONFIG_HOST_DIR],dnl
[_LIBI86_DO_SET_HOST_DIR([AC_CONFIG_SUBDIRS])])
