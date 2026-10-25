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

#include "die.h"

int main() {
    #line 19 "test5.c"
    DIE(how,(syscall,SYS_exit), code,(long)137);
}
