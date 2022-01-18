/*
    Copyright (c) 2021-2022 jdeokkim

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "ferox.h"
#include "raylib.h"

#define TARGET_FPS 60

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

#define SCREEN_CENTER ((Vector2) { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f })

#define SCREEN_WIDTH_IN_METERS  (frNumberPixelsToMeters(SCREEN_WIDTH))
#define SCREEN_HEIGHT_IN_METERS (frNumberPixelsToMeters(SCREEN_HEIGHT))

#define WORLD_RECTANGLE ((Rectangle) { \
    .width = SCREEN_WIDTH_IN_METERS,   \
    .height = SCREEN_HEIGHT_IN_METERS  \
})

#define SEMO_MATERIAL   ((frMaterial) { 2.0f, 0.0f, 0.75f, 0.75f })
#define BULLET_MATERIAL ((frMaterial) { 2.0f, 0.0f, 0.5f, 0.5f })
#define ENEMY_MATERIAL  ((frMaterial) { 1.0f, 0.0f, 0.25f, 0.25f })

#define MAX_ENEMY_COUNT 48

static const int SEMO_DATA = 0, BULLET_DATA = 1, ENEMY_DATA = 2;

static void DrawCustomCursor(Vector2 position);
static void onCollisionPreSolve(frCollision *collision);

const float DELTA_TIME = (1.0f / TARGET_FPS) * 100.0f;

int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(TARGET_FPS);
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "c-krit/ferox | bullet.c");
    
    frWorld *world = frCreateWorld(FR_STRUCT_ZERO(Vector2), WORLD_RECTANGLE);
    
    frSetWorldCollisionHandler(
        world,
        (frCollisionHandler) { 
            .pre_solve = onCollisionPreSolve
        }
    );

    frVertices semo_vertices = {
        .data = {
            frVec2PixelsToMeters((Vector2) {  0.0f, -16.0f }),
            frVec2PixelsToMeters((Vector2) { -14.0f, 16.0f }),
            frVec2PixelsToMeters((Vector2) {  14.0f, 16.0f })
        },
        .count = 3
    };

    frVertices bullet_vertices = {
        .data = {
            frVec2PixelsToMeters((Vector2) {  0.0f, -6.0f }),
            frVec2PixelsToMeters((Vector2) { -3.0f,  6.0f }),
            frVec2PixelsToMeters((Vector2) {  3.0f,  6.0f })
        },
        .count = 3
    };
    
    frBody *semo = frCreateBodyFromShape(
        FR_BODY_KINEMATIC,
        FR_FLAG_NONE,
        frVec2PixelsToMeters(SCREEN_CENTER),
        frCreatePolygon(SEMO_MATERIAL, semo_vertices)
    );
    
    frSetBodyUserData(semo, (void *) &SEMO_DATA);
    
    frAddToWorld(world, semo);
    
    for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
        Vector2 position = FR_STRUCT_ZERO(Vector2);
        
        position.x = GetRandomValue(0, 1) 
            ? GetRandomValue(0, 0.45f * SCREEN_WIDTH)
            : GetRandomValue(0.55f * SCREEN_WIDTH, SCREEN_WIDTH);
        
        position.y = GetRandomValue(0, 1)
            ? GetRandomValue(0, 0.45f * SCREEN_HEIGHT)
            : GetRandomValue(0.55f * SCREEN_HEIGHT, SCREEN_HEIGHT);
        
        frBody *enemy = frCreateBodyFromShape(
            FR_BODY_DYNAMIC,
            FR_FLAG_NONE,
            frVec2PixelsToMeters(position),
            frCreateCircle(ENEMY_MATERIAL, GetRandomValue(1, 3))
        );
        
        frSetBodyUserData(enemy, (void *) &ENEMY_DATA);
        
        frAddToWorld(world, enemy);
    }
    
    HideCursor();

    while (!WindowShouldClose()) {
        Vector2 direction = frVec2Subtract(
            frVec2PixelsToMeters(GetMousePosition()), 
            frGetBodyPosition(semo)
        );
        
        frSetBodyRotation(semo, frVec2Angle((Vector2) { .y = -1 }, direction));
        frSetBodyVelocity(semo, frVec2ScalarMultiply(frVec2Normalize(direction), 0.006f));
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            frBody *bullet = frCreateBodyFromShape(
                FR_BODY_DYNAMIC,
                FR_FLAG_NONE,
                frGetWorldPoint(semo, semo_vertices.data[0]),
                frCreatePolygon(BULLET_MATERIAL, bullet_vertices)
            );
            
            frSetBodyRotation(bullet, frVec2Angle((Vector2) { .y = -1.0f }, direction));
            frSetBodyUserData(bullet, (void *) &BULLET_DATA);
            
            frApplyImpulse(bullet, frVec2ScalarMultiply(frVec2Normalize(direction), 0.006f));
            
            frAddToWorld(world, bullet);
        }

        for (int i = 0; i < frGetWorldBodyCount(world); i++) {
            frBody *body = frGetWorldBody(world, i);
            
            if (!frIsInWorldBounds(world, body)) {
                frRemoveFromWorld(world, body);
                frReleaseBody(body);
            }
        }

        frSimulateWorld(world, DELTA_TIME);

        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        DrawCustomCursor(GetMousePosition());
        
        for (int i = 0; i < frGetWorldBodyCount(world); i++) {
            frBody *body = frGetWorldBody(world, i);
            
            int *user_data = (int *) frGetBodyUserData(body);
            
            if (*user_data == SEMO_DATA) frDrawBody(body, DARKGRAY);
            else if (*user_data == BULLET_DATA) frDrawBody(body, RED);
            else frDrawBodyLines(body, 2, BLACK);
        }
        
        frDrawSpatialHash(frGetWorldSpatialHash(world));
        
        DrawFPS(8, 8);

        EndDrawing();
    }

    frReleaseWorld(world);
    
    CloseWindow();

    return 0;
}

static void DrawCustomCursor(Vector2 position) {
    DrawLineEx(
        frVec2Add(position, (Vector2) { .x = -8 }),
        frVec2Add(position, (Vector2) { .x = 8 }),
        2,
        RED
    );
    
    DrawLineEx(
        frVec2Add(position, (Vector2) { .y = -8 }),
        frVec2Add(position, (Vector2) { .y = 8 }),
        2,
        RED
    );
}

static void onCollisionPreSolve(frCollision *collision) {
    frBody *b1 = collision->_bodies[0];
    frBody *b2 = collision->_bodies[1];
            
    int *data1 = (int *) frGetBodyUserData(b1);
    int *data2 = (int *) frGetBodyUserData(b2);
    
    if ((*data1 == BULLET_DATA && *data2 == ENEMY_DATA)
       || (*data1 == ENEMY_DATA && *data2 == BULLET_DATA)) {
        collision->check = false;
        
        frSetBodyPosition(
            b1,
            (Vector2) { 
                -SCREEN_WIDTH_IN_METERS, 
                -SCREEN_HEIGHT_IN_METERS 
            }
        );
        
        frSetBodyPosition(
            b2,
            (Vector2) { 
                -SCREEN_WIDTH_IN_METERS, 
                -SCREEN_HEIGHT_IN_METERS 
            }
        );
    }
}