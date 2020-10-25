#ifndef __LEVEL_H__
#define __LEVEL_H__
#include <raylib.h>
#include <raymath.h>

#include "screen.h"

typedef struct level_t {
    screen_t screen;
    Model terrain;
    Model car_hero;
    Model car_enemy;
    Vector3 terrain_position;
    Vector3 car_target_position;
    Vector3 car_position;
    void (*update)(struct level_t* level);
} level_t;

level_t level_init(void);
void level_fini(level_t level);
void level_pass_to_state_playing(level_t* level);
void level_pass_to_state_dead(level_t* level);

#endif

#ifdef __LEVEL_H_IMPLEMENTATION__
#undef __LEVEL_H_IMPLEMENTATION__
#include <stdio.h>
#include <raylib.h>
#include "screen.h"
#include "screen_utils.h"

level_t level_init(void){
    level_t return_value = {0};
 
    return_value.screen.camera = camera_init();
    
    return_value.car_target_position = return_value.car_position = (Vector3){0, 0, 37.f};
    return_value.terrain_position.y = -.5f;

    Image img_terrain = GenImageChecked(40, 40, 1, 1, DARKGRAY, GRAY);
    Image img_car = GenImageChecked(2, 2, 1, 1, PETROL, GRAY);
    
    return_value.terrain = LoadModelFromMesh(GenMeshCube(400.f, 0.2f, 400.f));
    return_value.car_hero = LoadModelFromMesh(GenMeshCube(4.f, 1.f, 6.f));

    return_value.terrain.materials[0].maps[MAP_DIFFUSE].texture = LoadTextureFromImage(img_terrain);
    return_value.car_hero.materials[0].maps[MAP_DIFFUSE].texture = LoadTextureFromImage(img_car);

    UnloadImage(img_terrain);
    UnloadImage(img_car);

    level_pass_to_state_playing(&return_value);
    return return_value;
}

void level_fini(level_t level) {
    printf("unloading level data\n");
    UnloadTexture(level.terrain.materials[0].maps[MAP_DIFFUSE].texture);
    UnloadModel(level.terrain);
}

static void process_state_playing(level_t* level){

    level->terrain_position.z += 100.f * GetFrameTime();

    if(level->terrain_position.z > 20.f) 
        level->terrain_position.z -= 20.f;

    BeginDrawing();
    {
        ClearBackground(WHITE);
        DrawFPS(10, 10);

        BeginMode3D(level->screen.camera);
        {
            DrawModel(level->terrain, level->terrain_position, 1.f, WHITE);
            DrawModel(level->car_hero, level->car_position, 1.f, WHITE);
        }
        EndMode3D();

        if (IsKeyDown(KEY_KP_ADD))
            level->screen.camera.position.z += 0.1f;
        if (IsKeyDown(KEY_KP_SUBTRACT))
            level->screen.camera.position.z -= 0.1f;

        if (IsKeyDown(KEY_KP_8))
            level->screen.camera.position.y += 0.1f;
        if (IsKeyDown(KEY_KP_2))
            level->screen.camera.position.y -= 0.1f;

        if (IsKeyDown(KEY_KP_9))
            level->screen.camera.fovy += 1.f;
        if (IsKeyDown(KEY_KP_7))
            level->screen.camera.fovy -= 1.f;

        if (IsKeyDown(KEY_KP_5))
            level->screen.camera = camera_init();

        if (IsKeyPressed(KEY_LEFT))
            level->car_target_position.x -= 6.0f;
        if (IsKeyPressed(KEY_RIGHT))
            level->car_target_position.x += 6.0f;
        if (IsKeyPressed(KEY_UP))
            level->car_target_position.z -= 6.0f;
        if (IsKeyPressed(KEY_DOWN))
            level->car_target_position.z += 1.0f;

        level->car_position = Vector3Lerp(level->car_position, level->car_target_position, 0.3f);
    }
    EndDrawing();
}

static void process_state_dead(level_t* level){
    printf("level dead\n");
}

void level_pass_to_state_playing(level_t* level) {
    level->update = process_state_playing;
}

void level_pass_to_state_dead(level_t* level) {
    level->update = process_state_dead;
}
#endif