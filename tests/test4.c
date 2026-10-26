/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/* Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com> */

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#define DIE_YES_ALIASES
#include "../die.h"

int main(int argc, char* argv[]) {
    #line 18 "test4.c"
    assert(argc == 2);
    FILE* f;
    TRYIO(f = fopen(argv[1], "r")) || DIE(,fopen);
    int c;
    if (FAILIO(c = fgetc(f))) DIE(,fgetc);
    TRYIO(fclose(f)) || DIE(,fclose);
}
