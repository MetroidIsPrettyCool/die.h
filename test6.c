/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/* Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com> */

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <inttypes.h>
#include <stdckdint.h>

#define DIE_YES_ALIASES2
#include "die.h"

int main(int argc, char* argv[]) {
    #line 21 "test6.c"
    MUST_NOT(argc < 2) || DIE(,argc, ,"less than 2", ,DIE_ep);

    char* s = argv[1];

    intmax_t x;
    char* endptr = nullptr;
    TRYE(x = strtoimax(s, &endptr, 10)) && MUST(*endptr == '\0')
    #line 29 "test6.c"
    || DIE(,strtoimax, ,"\"%s\" is not an intmax_t", ,(DIE_epf,s));

    uintmax_t y;
    if (x >= 0) y = (uintmax_t)x;
    else (void)ckd_mul(&y, x, -1);

    printf(
        "%c 0x%"PRIXMAX"\n",
        (x > 0) ? '+' :
        (x < 0) ? '-' : ' ',
        y
    );
}
