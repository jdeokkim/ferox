#if defined(SOKOL_IMPL) && !defined(SOKOL_TIME_IMPL)
#define SOKOL_TIME_IMPL
#endif
#ifndef SOKOL_TIME_INCLUDED
/*
    sokol_time.h -- simple cross-platform time measurement
                 -- modified (stripped down) for `ferox`

    Project URL: https://github.com/floooh/sokol

    License: zlib/libpng license

    Copyright (c) 2018 Andre Weissflog (https://github.com/floooh)
    Copyright (c) 2024 Jaedeok Kim (https://github.com/jdeokkim)

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in a
        product, an acknowledgment in the product documentation would be
        appreciated but is not required.

        2. Altered source versions must be plainly marked as such, and must not
        be misrepresented as being the original software.

        3. This notice may not be removed or altered from any source
        distribution.
*/

#define SOKOL_TIME_INCLUDED (1)
#include <stdint.h>

#if defined(SOKOL_API_DECL) && !defined(SOKOL_TIME_API_DECL)
#define SOKOL_TIME_API_DECL SOKOL_API_DECL
#endif
#ifndef SOKOL_TIME_API_DECL
#if defined(_WIN32) && defined(SOKOL_DLL) && defined(SOKOL_TIME_IMPL)
#define SOKOL_TIME_API_DECL __declspec(dllexport)
#elif defined(_WIN32) && defined(SOKOL_DLL)
#define SOKOL_TIME_API_DECL __declspec(dllimport)
#else
#define SOKOL_TIME_API_DECL extern
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

SOKOL_TIME_API_DECL void stm_setup(void);
SOKOL_TIME_API_DECL uint64_t stm_now(void);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif // SOKOL_TIME_INCLUDED

/*-- IMPLEMENTATION ----------------------------------------------------------*/
#ifdef SOKOL_TIME_IMPL
#define SOKOL_TIME_IMPL_INCLUDED (1)
#include <string.h> /* memset */

#ifndef SOKOL_API_IMPL
    #define SOKOL_API_IMPL
#endif
#ifndef _SOKOL_PRIVATE
    #if defined(__GNUC__) || defined(__clang__)
        #define _SOKOL_PRIVATE __attribute__((unused)) static
    #else
        #define _SOKOL_PRIVATE static
    #endif
#endif

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
typedef struct {
    uint32_t initialized;
    LARGE_INTEGER freq;
    LARGE_INTEGER start;
} _stm_state_t;
#elif defined(__APPLE__) && defined(__MACH__)
#include <mach/mach_time.h>
typedef struct {
    uint32_t initialized;
    mach_timebase_info_data_t timebase;
    uint64_t start;
} _stm_state_t;
#elif defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
typedef struct {
    uint32_t initialized;
    double start;
} _stm_state_t;
#else /* anything else, this will need more care for non-Linux platforms */
#ifdef ESP8266
// On the ESP8266, clock_gettime ignores the first argument and CLOCK_MONOTONIC isn't defined
#define CLOCK_MONOTONIC 0
#endif
#include <time.h>
typedef struct {
    uint32_t initialized;
    uint64_t start;
} _stm_state_t;
#endif
static _stm_state_t _stm;

/* prevent 64-bit overflow when computing relative timestamp
    see https://gist.github.com/jspohr/3dc4f00033d79ec5bdaf67bc46c813e3
*/
#if defined(_WIN32) || (defined(__APPLE__) && defined(__MACH__))
_SOKOL_PRIVATE int64_t _stm_int64_muldiv(int64_t value, int64_t numer, int64_t denom) {
    int64_t q = value / denom;
    int64_t r = value % denom;
    return q * numer + r * numer / denom;
}
#endif

SOKOL_API_IMPL void stm_setup(void) {
    memset(&_stm, 0, sizeof(_stm));
    _stm.initialized = 0xABCDABCD;
    #if defined(_WIN32)
        QueryPerformanceFrequency(&_stm.freq);
        QueryPerformanceCounter(&_stm.start);
    #elif defined(__APPLE__) && defined(__MACH__)
        mach_timebase_info(&_stm.timebase);
        _stm.start = mach_absolute_time();
    #elif defined(__EMSCRIPTEN__)
        _stm.start = emscripten_get_now();
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        _stm.start = (uint64_t)ts.tv_sec*1000000000 + (uint64_t)ts.tv_nsec;
    #endif
}

SOKOL_API_IMPL uint64_t stm_now(void) {
    uint64_t now;
    #if defined(_WIN32)
        LARGE_INTEGER qpc_t;
        QueryPerformanceCounter(&qpc_t);
        now = (uint64_t) _stm_int64_muldiv(qpc_t.QuadPart - _stm.start.QuadPart, 1000000000, _stm.freq.QuadPart);
    #elif defined(__APPLE__) && defined(__MACH__)
        const uint64_t mach_now = mach_absolute_time() - _stm.start;
        now = (uint64_t) _stm_int64_muldiv((int64_t)mach_now, (int64_t)_stm.timebase.numer, (int64_t)_stm.timebase.denom);
    #elif defined(__EMSCRIPTEN__)
        double js_now = emscripten_get_now() - _stm.start;
        now = (uint64_t) (js_now * 1000000.0);
    #else
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        now = ((uint64_t)ts.tv_sec*1000000000 + (uint64_t)ts.tv_nsec) - _stm.start;
    #endif
    return now;
}
#endif /* SOKOL_TIME_IMPL */

