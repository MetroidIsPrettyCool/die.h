/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/* Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com> */

#include <stdio.h>

#include "die.h"

#define EXPAND_AND_STRINGIZE(X) EXPAND_AND_STRINGIZE1(X)
#define EXPAND_AND_STRINGIZE1(X) #X

#undef DIE_WHO_DEFAULT
#define DIE_WHO_DEFAULT "warg!"

#undef DIE_CODE_DEFAULT
#define DIE_CODE_DEFAULT 137L

#undef DIE_M_USE_ERRNO
#define DIE_M_USE_ERRNO false

int main() {
    #include "die_defaults.inc"

    #line 29 "test7"
    puts(EXPAND_AND_STRINGIZE(DIE_WHO_DEFAULT));
    puts(EXPAND_AND_STRINGIZE(DIE_WHAT_DEFAULT));
    puts(EXPAND_AND_STRINGIZE(DIE_HOW_DEFAULT));
    puts(EXPAND_AND_STRINGIZE(DIE_CODE_DEFAULT));
    puts(EXPAND_AND_STRINGIZE(DIE_WHERE_DEFAULT));
    puts(EXPAND_AND_STRINGIZE(DIE_M_FEFLAGS));
    puts(EXPAND_AND_STRINGIZE(DIE_M_USE_ERRNO));
    puts(EXPAND_AND_STRINGIZE(DIE_M_USE_ERREXCEPT));

    return 0;
}
