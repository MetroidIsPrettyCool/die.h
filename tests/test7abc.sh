#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v clang
command -v diff
command -v gcc
command -v slimcc

gcc -O2 -std=c23 -Wall -Wextra -Wstrict-aliasing -Wno-parentheses -Werror -o test7a-gcc test7a.c
clang -O2 -std=c23 -Wall -Wextra -Wstrict-aliasing -Wno-logical-op-parentheses -Werror -o test7a-clang test7a.c
slimcc -O2 -std=c23 -Wall -o test7a-slimcc test7a.c
gcc -O2 -std=c23 -Wall -Wextra -Wstrict-aliasing -Wno-parentheses -Werror -o test7b-gcc test7b.c
clang -O2 -std=c23 -Wall -Wextra -Wstrict-aliasing -Wno-logical-op-parentheses -Werror -o test7b-clang test7b.c
slimcc -O2 -std=c23 -Wall -o test7b-slimcc test7b.c
gcc -O2 -std=c23 -Wall -Wextra -Wstrict-aliasing -Wno-parentheses -Werror -o test7c-gcc test7c.c
clang -O2 -std=c23 -Wall -Wextra -Wstrict-aliasing -Wno-logical-op-parentheses -Werror -o test7c-clang test7c.c
slimcc -O2 -std=c23 -Wall -o test7c-slimcc test7c.c

diff -q <(./test7a-gcc) <(./test7b-gcc)
diff -q <(./test7a-clang) <(./test7b-clang)
diff -q <(./test7a-slimcc) <(./test7b-slimcc)

! diff -q <(./test7a-gcc) <(./test7c-gcc) || exit 1
! diff -q <(./test7a-clang) <(./test7c-clang) || exit 1
! diff -q <(./test7a-slimcc) <(./test7c-slimcc) || exit 1
