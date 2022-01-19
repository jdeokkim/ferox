﻿/*
    Copyright (c) 2021-2022 jdeokkim

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "ferox.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

/* | `utils` 모듈 상수... | */

const float TWO_PI = (2 * PI), INVERSE_TWO_PI = (1.0f / TWO_PI);

/* | `utils` 모듈 함수... | */

/* 각도 `angle` (단위: rad.)을 정규화하여, 구간 `[center - π, center + π]`에 포함되도록 한다. */
float frNormalizeAngle(float angle, float center) {
    return angle - (TWO_PI * floorf((angle + PI - center) * INVERSE_TWO_PI));
}

/* 부동 소수점 값 `f1`이 `f2`와 근접한 값인지 확인한다. */
bool frNumberApproxEquals(float f1, float f2) {
    return fabsf(f1 - f2) <= FR_NUMBER_MAX(f1, f2) * FLT_EPSILON;
}

/* 주어진 픽셀 단위 거리를 미터 단위 거리로 변환한다. */
float frNumberPixelsToMeters(float value) {
    return (FR_GLOBAL_PIXELS_PER_METER != 0.0f)
        ? (value / FR_GLOBAL_PIXELS_PER_METER)
        : 0.0f;
}

/* 주어진 미터 단위 거리를 픽셀 단위 거리로 변환한다. */
float frNumberMetersToPixels(float value) {
    return (value * FR_GLOBAL_PIXELS_PER_METER);
}

/* 주어진 픽셀 단위 `Rectangle` 구조체를 미터 단위 `Rectangle` 구조체로 변환한다. */
Rectangle frRecPixelsToMeters(Rectangle rec) {
    return (Rectangle) {
        .x = frNumberPixelsToMeters(rec.x),
        .y = frNumberPixelsToMeters(rec.y),
        .width = frNumberPixelsToMeters(rec.width),
        .height = frNumberPixelsToMeters(rec.height)
    };
}

/* 주어진 미터 단위 `Rectangle` 구조체를 픽셀 단위 `Rectangle` 구조체로 변환한다. */
Rectangle frRecMetersToPixels(Rectangle rec) {
    return (Rectangle) {
        .x = frNumberMetersToPixels(rec.x),
        .y = frNumberMetersToPixels(rec.y),
        .width = frNumberMetersToPixels(rec.width),
        .height = frNumberMetersToPixels(rec.height)
    };
}
