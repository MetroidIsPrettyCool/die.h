#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com>

set -euxo pipefail

CLOSE_SYS_NO="$(enosys -l | sed -nE 's/^[[:space:]]+([[:digit:]]+)[[:space:]]+close$/\1/p')"

gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -lm -o test1-gcc test1.c
./test1-gcc
clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -lm -o test1-clang test1.c
./test1-clang
slimcc -O2 -std=c23 -Wall -lm -o test1-slimcc test1.c
./test1-slimcc

gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -DDIE_NO_PROTECT_KEYWORDS -lm -o test1b-gcc test1.c
./test1b-gcc
clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -DDIE_NO_PROTECT_KEYWORDS -lm -o test1b-clang test1.c
./test1b-clang
slimcc -O2 -std=c23 -Wall -DDIE_NO_PROTECT_KEYWORDS -lm -o test1b-slimcc test1.c
./test1b-slimcc

grep -q -F 'die.h requires C23 or later (__STDC_VERSION__ >= 202311L)' < <(! gcc -O2 -std=c17 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -static -o test1c-gcc test1.c 2>&1 || exit 1)
grep -q -F 'die.h requires C23 or later (__STDC_VERSION__ >= 202311L)' < <(! clang -O2 -std=c17 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -static -o test1c-clang test1.c 2>&1 || exit 1)

sed 's/DIE/SOME_PFX/g' die.h > die_renamed.h
sed 's/"die\.h"/"die_renamed.h"/
     s/DIE/SOME_PFX/g' test1.c > test1d.c
! grep -q -F 'DIE' test1d.c || exit 1
gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -DSOME_PFX_NO_PROTECT_KEYWORDS -lm -o test1d-gcc test1d.c
./test1d-gcc
clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -DSOME_PFX_NO_PROTECT_KEYWORDS -lm -o test1d-clang test1d.c
./test1d-clang
slimcc -O2 -std=c23 -Wall -DSOME_PFX_NO_PROTECT_KEYWORDS -lm -o test1d-slimcc test1.c
./test1d-slimcc

gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -lm -o test1e-gcc test1d.c
./test1e-gcc
clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -lm -o test1e-clang test1d.c
./test1e-clang
slimcc -O2 -std=c23 -Wall -lm -o test1e-slimcc test1.c
./test1e-slimcc

grep -q -F 'you may have forgotten to include fenv.h before die.h' < <(! gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test2-gcc test2.c 2>&1 || exit 1)
grep -q -F 'you may have forgotten to include fenv.h before die.h' < <(! clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test2-clang test2.c 2>&1 || exit 1)
grep -q -F 'you may have forgotten to include fenv.h before die.h' < <(! slimcc -O2 -std=c23 -Wall -Werror -o test2-slimcc test2.c 2>&1 || exit 1)

gcc -O2 -std=c23 -ffreestanding -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test3-gcc test3.c
./test3-gcc
clang -O2 -std=c23 -ffreestanding -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test3-clang test3.c
./test3-clang

gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -static -o test4-gcc test4.c
./test4-gcc test4.c
diff -q <(echo 'test4.c:20: fopen: No such file or directory') <(! ./test4-gcc 'file-that-does-not-exist.foobar' 2>&1 || exit 1)
diff -q <(echo 'test4.c:22: fgetc: Success') <(! ./test4-gcc '/dev/null' 2>&1 || exit 1)
diff -q <(echo 'test4.c:23: fclose: Function not implemented') <(! enosys -s "${CLOSE_SYS_NO}" ./test4-gcc test4.c 2>&1 || exit 1)

clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -static -o test4-clang test4.c
./test4-clang test4.c
diff -q <(echo 'test4.c:20: fopen: No such file or directory') <(! ./test4-clang 'file-that-does-not-exist.foobar' 2>&1 || exit 1)
diff -q <(echo 'test4.c:22: fgetc: Success') <(! ./test4-clang '/dev/null' 2>&1 || exit 1)
diff -q <(echo 'test4.c:23: fclose: Function not implemented') <(! enosys -s "${CLOSE_SYS_NO}" ./test4-clang test4.c 2>&1 || exit 1)

grep -q -F -e '-Werror=unused-value' < <(! gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -DDIE_NO_UNUSED_SUPPRESSION -o test4b-gcc test4.c 2>&1 || exit 1)

gcc -O2 -std=gnu23 -D_DEFAULT_SOURCE -Wall -Wextra -Wstrict-aliasing -Werror -o test5-gcc test5.c
clang -O2 -std=gnu23 -D_DEFAULT_SOURCE -Wall -Wextra -Wstrict-aliasing -Werror -o test5-clang test5.c

set +e
./test5-gcc
TEST5_GCC_RET="$?"
./test5-clang
TEST5_CLANG_RET="$?"
set -e
[ "${TEST5_GCC_RET}" -eq 137 ]
[ "${TEST5_CLANG_RET}" -eq 137 ]

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

rm -R -f TESTING.TXT die_renamed.h test1-clang test1-gcc test1-slimcc test1b-clang test1b-gcc test1b-slimcc test1d-clang test1d-gcc test1d-slimcc test1d.c test1e-clang test1e-gcc test1e-slimcc test3-clang test3-gcc test4-clang test4-gcc test4b-gcc test5-clang test5-gcc test6-clang test6-gcc test6-slimcc test7a-clang test7a-gcc test7a-slimcc test7b-clang test7b-gcc test7b-slimcc test7c-clang test7c-gcc test7c-slimcc test7d-clang test7d-gcc test7e-clang test7e-gcc test7f-clang test7f-gcc

printf '\nall tests passed!\n'
