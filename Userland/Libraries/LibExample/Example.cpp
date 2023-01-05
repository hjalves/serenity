/*
 * Copyright (c) 2020, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Example.h"
#include <AK/Format.h>

extern "C" {

void hello_world()
{
    dbgln("Hello world from Python!");
}
}
