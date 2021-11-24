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

#ifndef _LIBI86_LIBI86_INTERNAL_DOS_DBCS_H_
#define _LIBI86_LIBI86_INTERNAL_DOS_DBCS_H_

#ifndef _LIBI86_COMPILING_
# error \
  "<libi86/internal/dos-dbcs.h> should only be used when compiling libi86!"
#endif

#include <stdbool.h>
#include "libi86/internal/acconfig.h"
#include "libi86/internal/cdefs.h"

_LIBI86_BEGIN_EXTERN_C

#ifdef __FAR
typedef const unsigned short __attribute__ ((__aligned__ (1)))
	__far *_dos_dbcs_lead_table_t;
#else
typedef __libi86_fpcv_t _dos_dbcs_lead_table_t;
#endif
#define _null_dbcs_lt	((_dos_dbcs_lead_table_t) 0)

extern bool __libi86_msdos_dbcs_lead_p (char, _dos_dbcs_lead_table_t);
/*
 * _dos_get_dbcs_lead_table () is from Watcom's internal library interface.
 * When building libi86, this function might be defined in the underlying
 * C library, if the C library is newlib-ia16.
 *
 * _dos_free_dbcs_lead_table (.) might also be defined in the C library.
 */
extern _dos_dbcs_lead_table_t __libi86_msdos_get_dbcs_lead_table (void);
extern _dos_dbcs_lead_table_t _dos_get_dbcs_lead_table (void);
extern void __libi86_msdos_free_dbcs_lead_table (_dos_dbcs_lead_table_t);
#if (defined _LIBI86_INTERNAL_HAVE__DOS_GET_DBCS_LEAD_TABLE \
     && ! defined _LIBI86_INTERNAL_HAVE__DOS_FREE_DBCS_LEAD_TABLE) \
    || (! defined _LIBI86_INTERNAL_HAVE__DOS_GET_DBCS_LEAD_TABLE \
	&& ! defined __IA16_FEATURE_PROTECTED_MODE)
_LIBI86_ALT_INLINE void
_dos_free_dbcs_lead_table (_dos_dbcs_lead_table_t __dbcs)
{
}
#else
extern void _dos_free_dbcs_lead_table (_dos_dbcs_lead_table_t);
#endif

_LIBI86_END_EXTERN_C

#endif
