/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/* Copyright (c) 2026 Joseph Burke <joseph.burke.04@gmail.com> */

#define DIE_NO_STD_HEADERS
#include "die.h"

/* expected to warn "you may have forgotten to include fenv.h before die.h" */

int main() {
    return 0;
}
