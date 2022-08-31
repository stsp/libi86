dnl Copyright (c) 2018--2022 TK Chia
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

dnl Perform checks for underlying C library facilities, where these checks
dnl are needed for all target platforms.

AC_CHECK_TYPE([long long int],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE_LONG_LONG_INT],[1])])
AC_CHECK_DECL([O_TEXT],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE_O_TEXT],[1])],,
	      [#include <fcntl.h>])
AC_CHECK_DECL([O_BINARY],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE_O_BINARY],[1])],,
	      [#include <fcntl.h>])
AC_CHECK_DECL([ENAMETOOLONG],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE_ENAMETOOLONG],[1])],,
	      [#include <errno.h>])
dnl For vsscanf (...), use AC_CHECK_DECL rather than AC_CHECK_FUNC.  We want
dnl autoconf to include the appropriate headers --- especially <stdio.h> ---
dnl so that the detection can happen properly.  In particular, if
dnl -mnewlib-autofloat-stdio is in effect, we want to arrange to link vsscanf
dnl (...) without the bulky floating-point support, lest it causes the text
dnl segment to overflow...  -- tkchia 20211207
AC_CHECK_DECL([vsscanf],
	      [AC_DEFINE([_LIBI86_INTERNAL_HAVE_VSSCANF],[1])])
AC_CHECK_FUNC([system],[AC_DEFINE([_LIBI86_INTERNAL_HAVE_SYSTEM],[1])])
AC_CHECK_FUNC([getcwd],[AC_DEFINE([_LIBI86_INTERNAL_HAVE_GETCWD],[1])])
AC_CHECK_FUNC([_getcwd],[AC_DEFINE([_LIBI86_INTERNAL_HAVE__GETCWD],[1])])
AC_CHECK_FUNC([_dos_get_dbcs_lead_table],
  [AC_DEFINE([_LIBI86_INTERNAL_HAVE__DOS_GET_DBCS_LEAD_TABLE],[1])])
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
AC_MSG_CHECKING([whether mkdir (const char *, mode_t) is declared])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/stat.h>]]
				   [[#include <sys/types.h>]],
				   [[mkdir ("foo", 0700);]])],
		  [AC_DEFINE([_LIBI86_INTERNAL_HAVE_MKDIR2],[1])
		   AC_MSG_RESULT(yes)],
		  [AC_MSG_RESULT(no)])
AC_MSG_CHECKING([whether mkdir (const char *) is declared])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/stat.h>]],
				   [[mkdir ("foo");]])],
		  [AC_DEFINE([_LIBI86_INTERNAL_HAVE_MKDIR1],[1])
		   AC_MSG_RESULT(yes)],
		  [AC_MSG_RESULT(no)])
dnl libi86 does not actually call libc's _mkdir, but we try to detect it so
dnl that we can avoid conflicting with libc, in case we too want to define
dnl an _mkdir symbol.
AC_CHECK_FUNC([_mkdir],[AC_DEFINE([_LIBI86_INTERNAL_HAVE__MKDIR],[1])])
dnl ...
AC_CHECK_FUNC([getpid],[AC_DEFINE([_LIBI86_INTERNAL_HAVE_GETPID],[1])])
AC_CHECK_FUNC([_getpid],[AC_DEFINE([_LIBI86_INTERNAL_HAVE__GETPID],[1])])
AC_CHECK_FUNC([rmdir],[AC_DEFINE([_LIBI86_INTERNAL_HAVE_RMDIR],[1])])
AC_CHECK_FUNC([_rmdir],[AC_DEFINE([_LIBI86_INTERNAL_HAVE__RMDIR],[1])])
AC_CHECK_FUNC([chdir],[AC_DEFINE([_LIBI86_INTERNAL_HAVE_CHDIR],[1])])
AC_CHECK_FUNC([_chdir],[AC_DEFINE([_LIBI86_INTERNAL_HAVE__CHDIR],[1])])
AC_CHECK_FUNC([_setmode],[AC_DEFINE([_LIBI86_INTERNAL_HAVE__SETMODE],[1])])
AC_CHECK_DECL([_psp],[AC_DEFINE([_LIBI86_INTERNAL_HAVE__PSP],[1])],,dnl
	      [#include <stdlib.h>])
AC_CHECK_DECL([_osmajor],[AC_DEFINE([_LIBI86_INTERNAL_HAVE__OSMAJOR],[1])],,dnl
	      [#include <stdlib.h>])
AC_CHECK_DECL([_osminor],[AC_DEFINE([_LIBI86_INTERNAL_HAVE__OSMINOR],[1])],,dnl
	      [#include <stdlib.h>])

dnl We want to define types __libi86_mode_t & __libi86_pid_t to be the same
dnl as the types mode_t & pid_t, but without actually bringing in all the
dnl other types in <sys/types.h>.  Play some games with function prototypes
dnl to try to detect the underlying types for mode_t & pid_t.
AC_DEFUN([_LIBI86_TYPE],
  [AC_CACHE_CHECK([underlying type of ]$1,[[libi86_cv_type_]$1],dnl
    [[libi86_cv_type_]$1=
     _LIBI86_TYPE_DO([$1],[$2],[[int]])
     _LIBI86_TYPE_DO([$1],[$2],[[unsigned]])
     _LIBI86_TYPE_DO([$1],[$2],[[signed char]])
     _LIBI86_TYPE_DO([$1],[$2],[[unsigned char]])
     _LIBI86_TYPE_DO([$1],[$2],[[char]])
     _LIBI86_TYPE_DO([$1],[$2],[[short]])
     _LIBI86_TYPE_DO([$1],[$2],[[unsigned short]])
     _LIBI86_TYPE_DO([$1],[$2],[[long]])
     _LIBI86_TYPE_DO([$1],[$2],[[unsigned long]])
     _LIBI86_TYPE_DO([$1],[$2],[[long long]])
     _LIBI86_TYPE_DO([$1],[$2],[[unsigned long long]])
     [if test -z "$libi86_cv_type_]$1["; then]
        AC_MSG_ERROR([[could not figure out ]$1[ type definition]])
     [fi]])])
AC_DEFUN([_LIBI86_TYPE_DO],
	 [[if test -z "$libi86_cv_type_]$1["; then]
	    AC_COMPILE_IFELSE([AC_LANG_PROGRAM(dnl
				 [$2
				  [void __libi86_foo(]$1[ __libi86_quux);]
				  [void __libi86_foo(]$3[ __libi86_quux);]
				  [void __libi86_bar(]$1[ *__libi86_quux);]
				  [void __libi86_bar(]$3[ *__libi86_quux);]
				  $1 [__libi86_baz(void);]
				  $3 [__libi86_baz(void);]
				  $1 [*__libi86_qux(void);]
				  $3 [*__libi86_qux(void);]],dnl
				 [])],
			      [[libi86_cv_type_]$1='$3'])
	  [fi]])
_LIBI86_TYPE([pid_t],[#include <sys/types.h>])
_LIBI86_TYPE([mode_t],[#include <sys/types.h>])

AC_SUBST(ac_cv_have_decl_vsscanf)
AC_SUBST(ac_cv_func_system)
AC_SUBST(ac_cv_func_getcwd)
AC_SUBST(ac_cv_func_getpid)
AC_SUBST(ac_cv_func_getppid)
AC_SUBST(ac_cv_func_rmdir)
AC_SUBST(ac_cv_func_chdir)
AC_SUBST(ac_cv_have_decl__psp)
AC_SUBST(ac_cv_have_decl__osmajor)
AC_SUBST(ac_cv_have_decl__osminor)
AC_SUBST(libi86_cv_type_pid_t)
AC_SUBST(libi86_cv_type_mode_t)
