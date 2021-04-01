/*
 * Copyright (c) 2019--2020 TK Chia
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
 * This implements an internal routine to set errno to an error code output
 * by MS-DOS, and then return the same value to the caller.
 *
 * It makes no assumptions about the actual form that errno takes (e.g.  it
 * might be a macro which calls a function), but it assumes that MS-DOS
 * error codes can be directly treated as errno values.
 *
 * This routine is used by __libi86_ret_set_errno, which takes the state of
 * the carry flag into account.
 */

#define _LIBI86_COMPILING_
#include <errno.h>

#ifdef __MSDOS__
/* Do some basic sanity checking of errno values at compile time.  */
# if defined ENOENT && ENOENT - 0 != 2
#   error "weird ENOENT value for MS-DOS target"
# endif
# if defined EBADF && EBADF - 0 != 6
#   error "weird EBADF value for MS-DOS target"
# endif
# if defined ENOMEM && ENOMEM - 0 != 8
#   error "weird ENOMEM value for MS-DOS target"
# endif

__attribute__ ((regparmcall)) unsigned
__libi86_ret_really_set_errno (unsigned err)
{
  errno = (int) err;
  return err;
}
#endif
