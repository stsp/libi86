#!/bin/sh
# Copyright (c) 2018--2021 TK Chia
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

# Script to do automated testing under GitLab CI (https://docs.gitlab.com/ee/
# ci/), invoked by .gitlab-ci.yml .

set -e -v
mkdir build-$$ install-$$
inst_prefix="`pwd`"/install-$$
cd build-$$
# Some CI platforms, e.g. Travis CI, set $CC to `gcc'.  This interferes with
# the `configure' script's detection of the C compiler, which should really be
# either `ia16-elf-gcc' or `ack-cc'. (!)
unset CC
../configure --prefix="$inst_prefix" ${1+"$@"} || \
  (cat config.log */config.log && exit 1)
make
make check TESTSUITEFLAGS="$TESTSUITEFLAGS" || \
  (cat tests/testsuite.log && exit 1)
make install
