#!/usr/bin/env bash
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

grep -q -F 'you may have forgotten to include fenv.h before die.h [-Werror=cpp]' < <(! gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror test2.c 2>&1)
grep -q -F 'you may have forgotten to include fenv.h before die.h' < <(! clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror test2.c 2>&1)
grep -q -F 'you may have forgotten to include fenv.h before die.h' < <(! slimcc -O2 -std=c23 -Wall -Werror test2.c 2>&1)

gcc -O2 -std=c23 -ffreestanding -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test3-gcc test3.c
./test3-gcc
clang -O2 -std=c23 -ffreestanding -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test3-clang test3.c
./test3-clang

gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -static -o test4-gcc test4.c
./test4-gcc test4.c
diff -q <(echo 'test4.c:13: fopen: No such file or directory') <(! ./test4-gcc 'file-that-does-not-exist.foobar' 2>&1)
diff -q <(echo 'test4.c:15: fgetc: Success') <(! ./test4-gcc '/dev/null' 2>&1)
diff -q <(echo 'test4.c:16: fclose: Function not implemented') <(! enosys -s "${CLOSE_SYS_NO}" ./test4-gcc test4.c 2>&1)

clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -static -o test4-clang test4.c
./test4-clang test4.c
diff -q <(echo 'test4.c:13: fopen: No such file or directory') <(! ./test4-clang 'file-that-does-not-exist.foobar' 2>&1)
diff -q <(echo 'test4.c:15: fgetc: Success') <(! ./test4-clang '/dev/null' 2>&1)
diff -q <(echo 'test4.c:16: fclose: Function not implemented') <(! enosys -s "${CLOSE_SYS_NO}" ./test4-clang test4.c 2>&1)

grep -q -F -e '-Werror=unused-value' < <(! gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -DDIE_NO_UNUSED_SUPPRESSION -o test4b-gcc test4.c 2>&1)

rm -R -f TESTING.TXT ./test1-gcc ./test1-clang ./test1-slimcc ./test1b-gcc ./test1b-clang ./test1b-slimcc ./test3-gcc ./test3-clang ./test4-gcc ./test4-clang ./test4b-gcc

printf '\nall tests passed!\n'
