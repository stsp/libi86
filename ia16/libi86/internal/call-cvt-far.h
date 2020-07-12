/*
 * Macros to handle different function calling conventions.
 *
 * Copyright (c) 2020 TK Chia
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

/*
 * This header serves a similar purpose to "libi86/internal/call-cvt.h", but
 * it assumes that the function being defined is always a far function, no
 * matter what the memory model.
 */

#define FORCE_FAR__
#include "libi86/internal/call-cvt.h"
