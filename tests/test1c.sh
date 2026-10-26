#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v clang
command -v gcc
command -v grep

grep -q -F 'die.h requires C23 or later (__STDC_VERSION__ >= 202311L)' < <(! gcc -O2 -std=c17 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -static -o test1c-gcc test1.c 2>&1 || exit 1)
grep -q -F 'die.h requires C23 or later (__STDC_VERSION__ >= 202311L)' < <(! clang -O2 -std=c17 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -static -o test1c-clang test1.c 2>&1 || exit 1)
