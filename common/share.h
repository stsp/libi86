/*
 * Copyright (c) 2021 TK Chia
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the developer(s) nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * MS-DOS 3+ file sharing modes as accepted by _dos_open (...) & al.  The
 * precise semantics of these sharing modes are described in Ralf Brown's
 * Interrupt List (under int 0x21, ah = 0x3d).
 *
 * NOTE: these sharing mode flags will only be honoured at runtime if
 * share.exe (or share.com) is loaded!
 */

#ifndef _LIBI86_SHARE_H_
#define _LIBI86_SHARE_H_

#define _SH_COMPAT	0x00		/* compatibility mode */
#define _SH_DENYRW	0x10		/* deny read/write access by others */
#define _SH_DENYWR	0x20		/* deny write access by others */
#define _SH_DENYRD	0x30		/* deny read access by others */
#define _SH_DENYNO	0x40		/* allow full access by others */

#ifndef __STRICT_ANSI__
# define SH_COMPAT	_SH_COMPAT
# define SH_DENYRW	_SH_DENYRW
# define SH_DENYWR	_SH_DENYWR
# define SH_DENYRD	_SH_DENYRD
# define SH_DENYNO	_SH_DENYNO
#endif  /* ! __STRICT_ANSI__ */

#endif
