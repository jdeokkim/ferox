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

#include <float.h>

#include "ferox.h"
#include "greatest.h"

/* Private Function Prototypes ============================================> */

TEST utCircleVsCircle(void);
TEST utCircleVsPolygon(void);
TEST utPolygonVsPolygon(void);

/* Public Functions =======================================================> */

SUITE(collision) {
    RUN_TEST(utCircleVsCircle);
    RUN_TEST(utCircleVsPolygon);
    RUN_TEST(utPolygonVsPolygon);
}

/* Private Functions ======================================================> */

TEST utCircleVsCircle(void) {
    frShape *s1 = frCreateCircle(frStructZero(frMaterial), 1.0f);
    frShape *s2 = frCreateCircle(frStructZero(frMaterial), 1.15f);

    frBody *b1 = frCreateBodyFromShape(FR_BODY_KINEMATIC,
                                       frStructZero(frVector2),
                                       s1);

    frBody *b2 = frCreateBodyFromShape(FR_BODY_KINEMATIC,
                                       frStructZero(frVector2),
                                       s2);

    frCollision collision = { .count = 0 };

    {
        frSetBodyPosition(b1, (frVector2) { .x = -0.5f });
        frSetBodyPosition(b2, (frVector2) { .x = 1.65f });

        (void) frComputeCollision(b1, b2, &collision);

        ASSERT_EQ(1, collision.count);

        ASSERT_IN_RANGE(1.0f, collision.direction.x, FLT_EPSILON);
        ASSERT_IN_RANGE(0.0f, collision.direction.y, FLT_EPSILON);

        ASSERT_IN_RANGE(0.0f, collision.contacts[0].depth, FLT_EPSILON);

        ASSERT_IN_RANGE(0.5f, collision.contacts[0].point.x, FLT_EPSILON);
        ASSERT_IN_RANGE(0.0f, collision.contacts[0].point.y, FLT_EPSILON);
    }

    {
        frSetBodyPosition(b1, (frVector2) { .x = -0.5f });
        frSetBodyPosition(b2, (frVector2) { .x = 1.5f });

        (void) frComputeCollision(b1, b2, &collision);

        ASSERT_EQ(1, collision.count);

        ASSERT_IN_RANGE(1.0f, collision.direction.x, FLT_EPSILON);
        ASSERT_IN_RANGE(0.0f, collision.direction.y, FLT_EPSILON);

        ASSERT_IN_RANGE(0.15f, collision.contacts[0].depth, FLT_EPSILON);

        ASSERT_IN_RANGE(0.5f, collision.contacts[0].point.x, FLT_EPSILON);
        ASSERT_IN_RANGE(0.0f, collision.contacts[0].point.y, FLT_EPSILON);
    }

    {
        frSetBodyPosition(b1, (frVector2) { .x = 0.0f });
        frSetBodyPosition(b2, (frVector2) { .x = 0.0f });

        (void) frComputeCollision(b1, b2, &collision);

        ASSERT_EQ(1, collision.count);

        ASSERT_IN_RANGE(0.0f, collision.direction.x, FLT_EPSILON);
        ASSERT_IN_RANGE(1.0f, collision.direction.y, FLT_EPSILON);

        ASSERT_IN_RANGE(2.15f, collision.contacts[0].depth, FLT_EPSILON);

        ASSERT_IN_RANGE(0.0f, collision.contacts[0].point.x, FLT_EPSILON);
        ASSERT_IN_RANGE(1.0f, collision.contacts[0].point.y, FLT_EPSILON);
    }

    frReleaseShape(s1), frReleaseShape(s2);
    frReleaseBody(b1), frReleaseBody(b2);

    PASS();
}

TEST utCircleVsPolygon(void) {
    frShape *s1 = frCreateCircle(frStructZero(frMaterial), 1.0f);
    frShape *s2 = frCreateRectangle(frStructZero(frMaterial), 2.0f, 2.0f);

    frBody *b1 = frCreateBodyFromShape(FR_BODY_KINEMATIC,
                                       frStructZero(frVector2),
                                       s1);

    frBody *b2 = frCreateBodyFromShape(FR_BODY_KINEMATIC,
                                       frStructZero(frVector2),
                                       s2);

    frCollision collision = { .count = 0 };

    {
        frSetBodyPosition(b1, (frVector2) { .x = -1.5f });
        frSetBodyPosition(b2, (frVector2) { .x = 1.5f });

        (void) frComputeCollision(b1, b2, &collision);

        ASSERT_EQ(0, collision.count);
    }

    {
        frSetBodyPosition(b1, (frVector2) { .x = -1.0f });
        frSetBodyPosition(b2, (frVector2) { .x = 1.0f });

        (void) frComputeCollision(b1, b2, &collision);

        ASSERT_EQ(1, collision.count);

        ASSERT_IN_RANGE(1.0f, collision.direction.x, FLT_EPSILON);
        ASSERT_IN_RANGE(0.0f, collision.direction.y, FLT_EPSILON);

        ASSERT_IN_RANGE(0.0f, collision.contacts[0].depth, FLT_EPSILON);

        ASSERT_IN_RANGE(0.0f, collision.contacts[0].point.x, FLT_EPSILON);
        ASSERT_IN_RANGE(0.0f, collision.contacts[0].point.y, FLT_EPSILON);
    }

    {
        frSetBodyPosition(b1, (frVector2) { .x = -0.5f });
        frSetBodyPosition(b2, (frVector2) { .x = 0.5f });

        (void) frComputeCollision(b1, b2, &collision);

        ASSERT_EQ(1, collision.count);

        ASSERT_IN_RANGE(1.0f, collision.direction.x, FLT_EPSILON);
        ASSERT_IN_RANGE(0.0f, collision.direction.y, FLT_EPSILON);

        ASSERT_IN_RANGE(1.0f, collision.contacts[0].depth, FLT_EPSILON);

        ASSERT_IN_RANGE(0.5f, collision.contacts[0].point.x, FLT_EPSILON);
        ASSERT_IN_RANGE(0.0f, collision.contacts[0].point.y, FLT_EPSILON);
    }

    {
        float offsetX = 0.01f, offsetY = 0.01f;

        frSetBodyPosition(b1, frStructZero(frVector2));
        frSetBodyPosition(b2,
                          (frVector2) { .x = 1.0f + offsetX,
                                        .y = 1.0f + offsetY });

        (void) frComputeCollision(b1, b2, &collision);

        ASSERT_EQ(1, collision.count);

        frVector2 direction = frVector2Normalize(
            frVector2Subtract(frGetBodyPosition(b2), frGetBodyPosition(b1)));

        ASSERT_IN_RANGE(direction.x, collision.direction.x, FLT_EPSILON);
        ASSERT_IN_RANGE(direction.y, collision.direction.y, FLT_EPSILON);

        float depth = 1.0f - sqrtf((offsetX * offsetX) + (offsetY * offsetY));

        ASSERT_IN_RANGE(depth, collision.contacts[0].depth, FLT_EPSILON);
    }

    frReleaseShape(s1), frReleaseShape(s2);
    frReleaseBody(b1), frReleaseBody(b2);

    PASS();
}

TEST utPolygonVsPolygon(void) {
    /* TODO: ... */

    PASS();
}