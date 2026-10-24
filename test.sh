#!/usr/bin/env bash
set -euxo pipefail
gcc -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -lm -o test1-gcc test1.c && ./test1-gcc
printf '\n'
clang -O2 -std=c23 -Wall -Wextra -pedantic -Wstrict-aliasing -lm -o test1-clang test1.c && ./test1-clang
printf '\n'
slimcc -O2 -std=c23 -Wall -lm -o test1-slimcc test1.c && ./test1-slimcc
printf '\n'
