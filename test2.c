#define DIE_NO_STD_HEADERS
#include "die.h"

/* expected to warn "you may have forgotten to include fenv.h before die.h" */

int main() {
    return 0;
}
