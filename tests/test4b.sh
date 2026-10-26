#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

command -v gcc
command -v grep

grep -q -F -e '-Werror=unused-value' < <(! gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -DDIE_NO_UNUSED_SUPPRESSION -o test4b-gcc test4.c 2>&1 || exit 1)
