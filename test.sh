#!/usr/bin/env bash
set -euxo pipefail

gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -lm -o test1-gcc test1.c && ./test1-gcc
clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -lm -o test1-clang test1.c && ./test1-clang
slimcc -O2 -std=c23 -Wall -lm -o test1-slimcc test1.c && ./test1-slimcc

grep -q -F 'you may have forgotten to include fenv.h before die.h [-Werror=cpp]' < <(! gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror test2.c 2>&1)
grep -q -F 'you may have forgotten to include fenv.h before die.h' < <(! clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -Werror test2.c 2>&1)
grep -q -F 'you may have forgotten to include fenv.h before die.h' < <(! slimcc -O2 -std=c23 -Wall -Werror test2.c 2>&1)

gcc -O2 -std=c23 -ffreestanding -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test3-gcc test3.c && ./test3-gcc
clang -O2 -std=c23 -ffreestanding -Wall -Wextra -pedantic -Wstrict-aliasing -Werror -o test3-clang test3.c && ./test3-clang

rm -R -f TESTING.TXT ./test1-gcc ./test1-clang ./test1-slimcc ./test3-gcc ./test3-clang
