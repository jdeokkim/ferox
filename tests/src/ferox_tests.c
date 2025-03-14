/*
    Copyright (c) 2021-2025 Jaedeok Kim <jdeokkim@protonmail.com>

    Permission is hereby granted, free of charge, to any person obtaining a 
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation 
    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
    and/or sell copies of the Software, and to permit persons to whom the 
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included 
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.
*/

/* Includes ===============================================================> */

#include "ferox.h"
#include "greatest.h"

/* 'Prelude' Functions ====================================================> */

GREATEST_MAIN_DEFS();

/* Public Function Prototypes =============================================> */

SUITE_EXTERN(broad_phase);
SUITE_EXTERN(collision);
SUITE_EXTERN(geometry);
SUITE_EXTERN(rigid_body);
SUITE_EXTERN(utils);
SUITE_EXTERN(world);

/* Public Functions =======================================================> */

int main(int argc, char *argv[]) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(broad_phase);
    RUN_SUITE(collision);
    RUN_SUITE(geometry);
    RUN_SUITE(rigid_body);
    RUN_SUITE(utils);
    RUN_SUITE(world);

    GREATEST_MAIN_END();

    return 0;
}
