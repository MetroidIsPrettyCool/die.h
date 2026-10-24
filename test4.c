#include <stdio.h>
#include <stdlib.h>

#include <assert.h>

#define DIE_YES_ALIASES
#include "die.h"

int main(int argc, char* argv[]) {
    #line 11 "test4.c"
    assert(argc == 2);
    FILE* f;
    TRYIO(f = fopen(argv[1], "r")) || DIE(,fopen);
    int c;
    if (FAILIO(c = fgetc(f))) DIE(,fgetc);
    TRYIO(fclose(f)) || DIE(,fclose);
}
