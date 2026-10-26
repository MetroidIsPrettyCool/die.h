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

gcc -O2 -std=c23 -Wall -Wextra -Wstrict-aliasing -Wno-parentheses -Werror -o test6-gcc test6.c
clang -O2 -std=c23 -Wall -Wextra -Wstrict-aliasing -Wno-logical-op-parentheses -Werror -o test6-clang test6.c
slimcc -O2 -std=c23 -Wall -o test6-slimcc test6.c

diff -q <(echo '+ 0x7B') <(./test6-gcc 123 || exit 1)
diff -q <(echo '+ 0x7B') <(./test6-clang 123 || exit 1)
diff -q <(echo '+ 0x7B') <(./test6-slimcc 123 || exit 1)

diff -q <(echo '- 0x7B') <(./test6-gcc -123 || exit 1)
diff -q <(echo '- 0x7B') <(./test6-clang -123 || exit 1)
diff -q <(echo '- 0x7B') <(./test6-slimcc -123 || exit 1)

diff -q <(echo '  0x0') <(./test6-gcc 0 || exit 1)
diff -q <(echo '  0x0') <(./test6-clang 0 || exit 1)
diff -q <(echo '  0x0') <(./test6-slimcc 0 || exit 1)

diff -q <(echo 'test6.c:21: argc: less than 2') <(! ./test6-gcc 2>&1 || exit 1)
diff -q <(echo 'test6.c:21: argc: less than 2') <(! ./test6-clang 2>&1 || exit 1)
diff -q <(echo 'test6.c:21: argc: less than 2') <(! ./test6-slimcc 2>&1 || exit 1)

diff -q <(echo 'test6.c:29: strtoimax: "g" is not an intmax_t') <(! ./test6-gcc g 2>&1 || exit 1)
diff -q <(echo 'test6.c:29: strtoimax: "g" is not an intmax_t') <(! ./test6-clang g 2>&1 || exit 1)
diff -q <(echo 'test6.c:29: strtoimax: "g" is not an intmax_t') <(! ./test6-slimcc g 2>&1 || exit 1)

diff -q <(echo 'test6.c:29: strtoimax: "1g" is not an intmax_t') <(! ./test6-gcc 1g 2>&1 || exit 1)
diff -q <(echo 'test6.c:29: strtoimax: "1g" is not an intmax_t') <(! ./test6-clang 1g 2>&1 || exit 1)
diff -q <(echo 'test6.c:29: strtoimax: "1g" is not an intmax_t') <(! ./test6-slimcc 1g 2>&1 || exit 1)

diff -q <(echo 'test6.c:29: strtoimax: "g1" is not an intmax_t') <(! ./test6-gcc g1 2>&1 || exit 1)
diff -q <(echo 'test6.c:29: strtoimax: "g1" is not an intmax_t') <(! ./test6-clang g1 2>&1 || exit 1)
diff -q <(echo 'test6.c:29: strtoimax: "g1" is not an intmax_t') <(! ./test6-slimcc g1 2>&1 || exit 1)
