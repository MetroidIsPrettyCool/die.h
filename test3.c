/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/* Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com> */

#if __STDC_HOSTED__ != 0
    #error this is a freestanding test!
#endif

#define DIE_NO_STD_HEADERS
#define DIE_NO_PROTECT_KEYWORDS
#include "die.h"

#ifdef DIE_HOW_DEFAULT
    #error DIE_HOW_DEFAULT should not be defined!
#endif
#ifdef DIE_OUT_DEFAULT
    #error DIE_OUT_DEFAULT should not be defined!
#endif
#ifndef DIE_NO_UNUSED_SUPPRESSION
    #error DIE_NO_UNUSED_SUPPRESSION should be defined!
#endif
#if defined(__STDC_VERSION_STDDEF_H__) || defined(unreachable) \
    || defined(NULL) || defined(offsetof)
    #error stddef.h should not have been included!
#endif

#define EXPAND_AND_STRINGIZE(X) EXPAND_AND_STRINGIZE1(X)
#define EXPAND_AND_STRINGIZE1(X) #X

#define DIE_HOW_DEFAULT my_term_fn
#define DIE_OUT_DEFAULT my_log_fn

[[noreturn]]
void my_term_fn([[maybe_unused]] int code);

void my_log_fn(char const* p);

int main(void) {
    {
        #line 45 "test3.c"
        char const s[] = EXPAND_AND_STRINGIZE(DIE());
        char const expected[] = "(my_log_fn(\"test3.c\"\":\"\"45\""
                                " \"\" \"\" ),my_term_fn(1),0)";
        if (sizeof s != sizeof expected) return 1;
        for (unsigned i = 0; i < sizeof expected; ++i) {
            if (s[i] != expected[i]) return 1;
        }
    }

    {
        #line 56 "test3.c"
        char const s[] = EXPAND_AND_STRINGIZE(DIE(DIE_code,137));
        char const expected[] = "(my_log_fn(\"test3.c\"\":\"\"56\""
                                " \"\" \"\" ),my_term_fn(137),0)";
        if (sizeof s != sizeof expected) return 1;
        for (unsigned i = 0; i < sizeof expected; ++i) {
            if (s[i] != expected[i]) return 1;
        }
    }

    {
        #line 67 "test3.c"
        char const s[] = EXPAND_AND_STRINGIZE(DIE(DIE_out,(foo,1,2,3)));
        char const expected[] = "(foo(\"test3.c\"\":\"\"67\""
                                " \"\" \"\" ,1,2,3),my_term_fn(1),0)";
        if (sizeof s != sizeof expected) return 1;
        for (unsigned i = 0; i < sizeof expected; ++i) {
            if (s[i] != expected[i]) return 1;
        }
    }

    {
        char const s[] = EXPAND_AND_STRINGIZE(DIE_CODE_DEFAULT);
        char const expected[] = "1";
        if (sizeof s != sizeof expected) return 1;
        for (unsigned i = 0; i < sizeof expected; ++i) {
            if (s[i] != expected[i]) return 1;
        }
    }

    return 0;
}

void my_log_fn(char const* p) {
    [[maybe_unused]] volatile char const* some_mmio = p;
}

[[noreturn]]
void my_term_fn([[maybe_unused]] int code) {
    while (true);
}
