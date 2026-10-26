#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v [
command -v clang
command -v gcc

gcc -O2 -std=gnu23 -D_DEFAULT_SOURCE -Wall -Wextra -Wstrict-aliasing -Werror -o test5-gcc test5.c
clang -O2 -std=gnu23 -D_DEFAULT_SOURCE -Wall -Wextra -Wstrict-aliasing -Werror -o test5-clang test5.c

set +e
./test5-gcc
TEST5_GCC_RET="$?"
./test5-clang
TEST5_CLANG_RET="$?"
set -e
[ "${TEST5_GCC_RET}" -eq 137 ]
[ "${TEST5_CLANG_RET}" -eq 137 ]
