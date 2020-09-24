dnl Copyright (c) 2020 TK Chia
dnl
dnl This file is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU Lesser General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public License
dnl along with this program; see the file COPYING2.LIB.  If not see
dnl <http://www.gnu.org/licenses/>.

dnl Set default options to build & install for MS-DOS host.  -- tkchia 20200727
AC_DEFUN([_LIBI86_DEFAULT_OPTIONS],[
m4_divert_push([DEFAULTS])dnl
set x --host=ia16-elf --disable-elks-libc \
  --exec-prefix='${prefix}'/ia16-elf [$]{1+"[$][@]"}
shift
m4_divert_pop([DEFAULTS])])
./ia16/psp.S
./ia16/fmemcpy.S
./ia16/libi86-conio-init.c
./ia16/dpmi-hosted.S
./ia16/getch.c
./ia16/libi86-setvideomode-nonsvga.S
./ia16/sound.S
./ia16/dos-freemem.S
./ia16/graph.h
./ia16/delay.c
./ia16/osmajor-osminor.S
./ia16/dos-setfileattr.S
./ia16/libi86-intr-do.S
./ia16/dpmi.h
./ia16/getdrive.S
./ia16/libi86-ret-really-set-errno.c
./ia16/libi86-int86-do.S
./ia16/libi86-ungetch-buf.c
./ia16/dpmisegmenttodescriptor.S
./ia16/enable.S
./ia16/ungetch.S
./ia16/bios.h
./ia16/inpw.S
./ia16/dos-getdrive.S
./ia16/bios-keybrd.c
./ia16/dos.h
./ia16/libi86-fmemmove-backward.S
./ia16/outpw.S
./ia16/int86.c
./ia16/cscanf.c
./ia16/vcscanf.c
./ia16/bios-equiplist.S
./ia16/vcprintf.c
./ia16/libi86/internal/struc.h
./ia16/libi86/internal/arch.h
./ia16/libi86/internal/cdefs.h
./ia16/libi86/internal/conio.h
./ia16/libi86/internal/portio.h
./ia16/libi86/stdlib.h
./ia16/libi86/string.h
./ia16/dos-getfileattr.S
./ia16/libi86-int86x-do.S
./ia16/putch.c
./ia16/inp.S
./ia16/fmemmove.c
./ia16/outp.S
./ia16/intr.c
./ia16/disable.S
./ia16/libi86-setvideomode-svga.S
./ia16/nosound.S
./ia16/bios-joystick.S
./ia16/bios-memsize.S
./ia16/bios-disk.c
./ia16/libi86-ret-set-errno.S
./ia16/cprintf.c
./ia16/libi86-sound-by-divisor.S
./ia16/libi86-intr-dispatch.S
./ia16/bdos.S
./ia16/conio.h
./ia16/i86.h
./ia16/kbhit.c
./ia16/intdos.S
./ia16/libi86-intr-call.S
./ia16/Makefile.in
./ia16/libi86-setvideomode-default.S
./ia16/cgets.c
./ia16/configure.ac
./ia16/dpmigetdescriptor.S
./ia16/cputs.c
./ia16/intdosx.S
./ia16/dos-close.S
./ia16/bios-timeofday.c
./ia16/make-config.sh
./ia16/dos-allocmem.S
./ia16/setvideomode.c
./ia16/segread.S
./ia16/getche.c
./ia16/int86x.c
./autogen.sh
./config.guess
./config.sub
./examples/drivei86.c
./examples/joyi86.c
./examples/Makefile.in
./examples/configure.ac
./examples/dpmii86.c
./Makefile.in
./configure.ac
