#!/bin/sh
#
# Copyright (c) 2021 TK Chia
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#   * Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#   * Neither the name of the developer(s) nor the names of its contributors
#     may be used to endorse or promote products derived from this software
#     without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
# IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Work-around wrapper for the ACK front-end, to invoke its C preprocessor in
# a way that GNU autoconf expects.  There are two issues at hand here:
#
#   * ACK does not recognize the `-E' option used by most compilers (e.g.
#     GCC); the way to invoke the C preprocessor is to use a `-c.i' option.
#   * In addition, the output file name(s) for `-c.i' cannot be controlled
#     with an `-o' option.
#
#       -- tkchia 20211105

set -e
id=$$
for src in ${1+"$@"}; do
  echo "#include \"$src\"" > src-$id.c
  ${CC-'ack-cc'} -c.i src-$id.c || \
    (rm -f src-$id.[ci] && exit 1)
  cat src-$id.i
  rm src-$id.[ci]
done
