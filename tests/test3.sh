#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v clang
command -v gcc

gcc -O2 -std=c23 -ffreestanding -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test3-gcc test3.c
./test3-gcc
clang -O2 -std=c23 -ffreestanding -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test3-clang test3.c
./test3-clang
