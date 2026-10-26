#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v clang
command -v diff
command -v enosys
command -v gcc
command -v sed
command -v slimcc

CLOSE_SYS_NO="$(enosys -l | sed -nE 's/^[[:space:]]+([[:digit:]]+)[[:space:]]+close$/\1/p')"

gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -static -o test4a-gcc test4.c
./test4a-gcc test4.c
diff -q <(echo 'test4.c:20: fopen: No such file or directory') <(! ./test4a-gcc 'file-that-does-not-exist.foobar' 2>&1 || exit 1)
diff -q <(echo 'test4.c:22: fgetc: Success') <(! ./test4a-gcc '/dev/null' 2>&1 || exit 1)
diff -q <(echo 'test4.c:23: fclose: Function not implemented') <(! enosys -s "${CLOSE_SYS_NO}" ./test4a-gcc test4.c 2>&1 || exit 1)

clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -static -o test4a-clang test4.c
./test4a-clang test4.c
diff -q <(echo 'test4.c:20: fopen: No such file or directory') <(! ./test4a-clang 'file-that-does-not-exist.foobar' 2>&1 || exit 1)
diff -q <(echo 'test4.c:22: fgetc: Success') <(! ./test4a-clang '/dev/null' 2>&1 || exit 1)
diff -q <(echo 'test4.c:23: fclose: Function not implemented') <(! enosys -s "${CLOSE_SYS_NO}" ./test4a-clang test4.c 2>&1 || exit 1)
