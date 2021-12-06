/*
 * Decide which newer C standard features to expose to the user program. 
 *
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

#ifndef _LIBI86_LIBI86_INTERNAL_FEATURES_H_
#define _LIBI86_LIBI86_INTERNAL_FEATURES_H_

/* This logic is largely cribbed from Newlib's <sys/features.h>. */
#if defined _GNU_SOURCE || defined _ISOC11_SOURCE \
    || __STDC_VERSION - 0 >= 201112L || __cplusplus - 0 >= 201103L
# define _LIBI86_ISO_C_VISIBLE	2011
#elif defined _ISOC99_SOURCE || _POSIX_C_SOURCE - 0 >= 200112L \
      || _XOPEN_SOURCE - 0 >= 600 || __STDC_VERSION__ - 0 >= 199901L \
      || defined __cplusplus
# define _LIBI86_ISO_C_VISIBLE	1999
#else
# define _LIBI86_ISO_C_VISIBLE	1990
#endif

#endif
