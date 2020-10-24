#ifndef __LEVEL_H__
#define __LEVEL_H__
#include <raylib.h>

enum level_state_e {
    LEVEL_STATE_PLAYING,
    LEVEL_STATE_DEAD,
    LEVEL_STATE_PAUSE
};

typedef struct level_t {
    unsigned int state;
    Camera3D camera;
    Model terrain;
    Vector3 terrain_position;
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

level_t level_init(void){
    level_t return_value = {0};

    return_value.camera.fovy = 45.0f;
    return_value.camera.position = (Vector3){0.0f, 10.0f, 50.0f};
    return_value.camera.target = (Vector3){0.0f, 0.0f, 20.0f};
    return_value.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    return_value.camera.type = CAMERA_PERSPECTIVE;
    
    return_value.car_position.z = 37.f;
    return_value.terrain_position.y = -.5f;

    Image img = GenImageChecked(40, 40, 1, 1, GREEN, BLUE);
    
    return_value.terrain = LoadModelFromMesh(GenMeshCube(400.f, 0.2f, 400.f));

    return_value.terrain.materials[0].maps[MAP_DIFFUSE].texture = LoadTextureFromImage(img);

    UnloadImage(img);

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

        BeginMode3D(level->camera);
        {
            DrawModel(level->terrain, level->terrain_position, 1.f, WHITE);
            DrawCube(level->car_position, 1, 1, 1, RED);
            DrawCubeWires(level->car_position, 1, 1, 1, BLUE);
        }
        EndMode3D();

        if (IsKeyDown(KEY_KP_ADD))
            level->camera.fovy += 1.0f;
        if (IsKeyDown(KEY_KP_SUBTRACT))
            level->camera.fovy -= 1.0f;

        if (IsKeyPressed(KEY_LEFT))
            level->car_position.x -= 1.0f;
        if (IsKeyPressed(KEY_RIGHT))
            level->car_position.x += 1.0f;
        if (IsKeyPressed(KEY_UP))
            level->car_position.z -= 1.0f;
        if (IsKeyPressed(KEY_DOWN))
            level->car_position.z += 1.0f;
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