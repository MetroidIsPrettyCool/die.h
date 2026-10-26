#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v clang
command -v gcc
command -v slimcc

gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -DDIE_NO_PROTECT_KEYWORDS -lm -o test1b-gcc test1.c
./test1b-gcc
clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -DDIE_NO_PROTECT_KEYWORDS -lm -o test1b-clang test1.c
./test1b-clang
slimcc -O2 -std=c23 -Wall -DDIE_NO_PROTECT_KEYWORDS -lm -o test1b-slimcc test1.c
./test1b-slimcc
