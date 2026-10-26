#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v clang
command -v gcc
command -v sed
command -v slimcc

sed -e 's/DIE/SOME_PFX/g' ../die.h > die_renamed.h
sed -e 's/"\.\.\/die\.h"/"die_renamed.h"/' -e 's/DIE/SOME_PFX/g' test1.c > test1d.c
! grep -q -F 'DIE' test1d.c || exit 1
gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -DSOME_PFX_NO_PROTECT_KEYWORDS -lm -o test1d-gcc test1d.c
./test1d-gcc
clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -DSOME_PFX_NO_PROTECT_KEYWORDS -lm -o test1d-clang test1d.c
./test1d-clang
slimcc -O2 -std=c23 -Wall -DSOME_PFX_NO_PROTECT_KEYWORDS -lm -o test1d-slimcc test1.c
./test1d-slimcc
