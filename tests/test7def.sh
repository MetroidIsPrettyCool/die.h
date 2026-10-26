#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v clang
command -v diff
command -v gcc

gcc -O2 -std=c23 -ffreestanding -Wall -Wextra -Wstrict-aliasing -Wno-parentheses -Werror -o test7d-gcc test7a.c
clang -O2 -std=c23 -ffreestanding -Wall -Wextra -Wstrict-aliasing -Wno-logical-op-parentheses -Werror -o test7d-clang test7a.c
gcc -O2 -std=c23 -ffreestanding -Wall -Wextra -Wstrict-aliasing -Wno-parentheses -Werror -o test7e-gcc test7b.c
clang -O2 -std=c23 -ffreestanding -Wall -Wextra -Wstrict-aliasing -Wno-logical-op-parentheses -Werror -o test7e-clang test7b.c
gcc -O2 -std=c23 -ffreestanding -Wall -Wextra -Wstrict-aliasing -Wno-parentheses -Werror -o test7f-gcc test7c.c
clang -O2 -std=c23 -ffreestanding -Wall -Wextra -Wstrict-aliasing -Wno-logical-op-parentheses -Werror -o test7f-clang test7c.c

diff -q <(./test7d-gcc) <(./test7e-gcc)
diff -q <(./test7d-clang) <(./test7e-clang)

! diff -q <(./test7d-gcc) <(./test7f-gcc) || exit 1
! diff -q <(./test7d-clang) <(./test7f-clang) || exit 1
