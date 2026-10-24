#if __STDC_HOSTED__ != 0
    #error this is a freestanding test!
#endif

#define DIE_NO_STD_HEADERS
#define DIE_NO_PROTECT_KEYWORDS
#include "die.h"

#define EXPAND_AND_STRINGIZE(X) EXPAND_AND_STRINGIZE1(X)
#define EXPAND_AND_STRINGIZE1(X) #X

#define DIE_HOW_DEFAULT my_term_fn
#define DIE_OUT_DEFAULT my_log_fn

[[noreturn]]
void my_term_fn([[maybe_unused]] int code);

void my_log_fn(char const* p);

int main(void) {
    #line 22 "test3.c"
    char const* const s = EXPAND_AND_STRINGIZE(DIE());
    char const expected[] = "(my_log_fn(\"test3.c\"\":\"\"22\" \"\" \"\" ),my_term_fn(1),0)";
    for (unsigned i = 0; i < sizeof expected; ++i) {
        if (s[i] != expected[i]) return 1;
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
