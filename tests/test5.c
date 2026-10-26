/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/* Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com> */

#ifndef _DEFAULT_SOURCE
    #error Need _DEFAULT_SOURCE for this test!
#endif

#if !__has_include(<sys/syscall.h>)
    #error Need <sys/syscall.h> for this test!
#endif
#if !__has_include(<unistd.h>)
    #error Need <unistd.h> for this test!
#endif

#include <sys/syscall.h>
#include <unistd.h>

#include "../die.h"

int main() {
    #line 27 "test5.c"
    DIE(how,(syscall,SYS_exit), code,(long)137);
}
