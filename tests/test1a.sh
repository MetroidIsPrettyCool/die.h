#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v clang
command -v gcc
command -v slimcc

gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -lm -o test1a-gcc test1.c
./test1a-gcc
clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -lm -o test1a-clang test1.c
./test1a-clang
slimcc -O2 -std=c23 -Wall -lm -o test1a-slimcc test1.c
./test1a-slimcc
