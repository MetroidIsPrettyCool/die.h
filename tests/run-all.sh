#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

./test1a.sh
./test1b.sh
./test1c.sh
./test1d.sh
./test1e.sh

./test2.sh

./test3.sh

./test4a.sh
./test4b.sh

./test5.sh

./test6.sh

./test7abc.sh
./test7def.sh

rm -R -f TESTING.TXT die_renamed.h test1a-clang test1a-gcc test1a-slimcc test1b-clang test1b-gcc test1b-slimcc test1d-clang test1d-gcc test1d-slimcc test1d.c test1e-clang test1e-gcc test1e-slimcc test3-clang test3-gcc test4a-clang test4a-gcc test4b-gcc test5-clang test5-gcc test6-clang test6-gcc test6-slimcc test7a-clang test7a-gcc test7a-slimcc test7b-clang test7b-gcc test7b-slimcc test7c-clang test7c-gcc test7c-slimcc test7d-clang test7d-gcc test7e-clang test7e-gcc test7f-clang test7f-gcc

printf '\nall tests passed!\n'
