#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v clang
command -v gcc
command -v grep
command -v slimcc

grep -q -F 'you may have forgotten to include fenv.h before die.h' < <(! gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test2-gcc test2.c 2>&1 || exit 1)
grep -q -F 'you may have forgotten to include fenv.h before die.h' < <(! clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test2-clang test2.c 2>&1 || exit 1)
grep -q -F 'you may have forgotten to include fenv.h before die.h' < <(! slimcc -O2 -std=c23 -Wall -Werror -o test2-slimcc test2.c 2>&1 || exit 1)
