#ifndef __LEVEL_H__
#define __LEVEL_H__
#include <raylib.h>
#include <raymath.h>

#include "screen.h"

#define CAR_LATERAL_SPEED 6.f
#define TREE_RADIUS 15.f
#define CAR_LERP_SPEED 20.0f
#define CAR_Z_POSITION 37.f
#define TREE_COUNT 30
#define TERRAIN_SIDE_SIZE 400.f
#define CAR_INITIAL_SPEED 0.f
#define CAR_ACCELERATION 10.f
#define CAR_MAX_SPEED 150.f
#define CAR_ENEMY_COUNT 5
#define CAR_LENGTH 6.f

typedef enum models_e {
    MODELS_TREE,
    MODELS_CAR_HERO,
    MODELS_TERRAIN,
    MODELS_CAR_ENEMY,
    MODELS_COUNT,
} models_e;

typedef struct level_t {
    screen_t screen;
    Model models[MODELS_COUNT];
    Vector3 terrain_position;
    Vector3 car_target_position;
    Vector3 car_position;
    Shader shader;
    Shader shader_default;
    Light light_1;
    Vector3 tree_positions[TREE_COUNT];
    Vector3 car_enemy_position[CAR_ENEMY_COUNT];
    float car_speed;
    float tree_offset;
    int car_enemy_active_count;
    void (*update)(struct level_t* level);
} level_t;

level_t level_init(void);
void level_fini(level_t level);
void level_pass_to_state_playing(level_t* level);
void level_pass_to_state_dead(level_t* level);
void shader_init(level_t* level);

#endif

#ifdef __LEVEL_H_IMPLEMENTATION__
#undef __LEVEL_H_IMPLEMENTATION__
#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include "screen_utils.h"

level_t level_init(void){
    level_t return_value = {0};
 
    shader_init(&return_value);

    return_value.car_enemy_active_count = 1;

    return_value.screen.camera = camera_init();
    
    return_value.car_target_position = return_value.car_position = (Vector3){0, 0, CAR_Z_POSITION};
    return_value.terrain_position.y = -.5f;

    Image img_terrain = GenImageChecked(40, 40, 1, 1, DARKGRAY, GRAY);
    Image img_car = GenImageChecked(2, 2, 1, 1, PETROL, GRAY);
    Image img_car_enemy = GenImageChecked(2, 2, 1, 1, ORANGE, RED);
    Image img_tree = GenImageChecked(2, 2, 1, 1, GREEN, DARKGREEN);
    
    return_value.models[MODELS_TERRAIN] = LoadModelFromMesh(GenMeshCube(TERRAIN_SIDE_SIZE, 0.2f, TERRAIN_SIDE_SIZE));
    return_value.models[MODELS_CAR_HERO] = LoadModelFromMesh(GenMeshCube(4.f, 2.f, CAR_LENGTH));
    return_value.models[MODELS_CAR_ENEMY] = LoadModelFromMesh(GenMeshCube(4.f, 2.f, CAR_LENGTH));
    return_value.models[MODELS_TREE] = LoadModelFromMesh(GenMeshCylinder(1.f, 5.f, 4));

    return_value.models[MODELS_TERRAIN].materials[0].maps[MAP_DIFFUSE].texture = LoadTextureFromImage(img_terrain);
    return_value.models[MODELS_CAR_HERO].materials[0].maps[MAP_DIFFUSE].texture = LoadTextureFromImage(img_car);
    return_value.models[MODELS_CAR_ENEMY].materials[0].maps[MAP_DIFFUSE].texture = LoadTextureFromImage(img_car_enemy);
    return_value.models[MODELS_TREE].materials[0].maps[MAP_DIFFUSE].texture = LoadTextureFromImage(img_tree);

    return_value.models[MODELS_TERRAIN].materials[0].shader = return_value.shader;
    return_value.models[MODELS_CAR_HERO].materials[0].shader = return_value.shader;
    return_value.models[MODELS_CAR_ENEMY].materials[0].shader = return_value.shader;
    return_value.models[MODELS_TREE].materials[0].shader = return_value.shader;

    UnloadImage(img_terrain);
    UnloadImage(img_car);
    UnloadImage(img_tree);
    UnloadImage(img_car_enemy);

    for(int i = 0; i < TREE_COUNT; i++){
        return_value.tree_positions[i].z = i * 20;
        return_value.tree_positions[i].x = TREE_RADIUS;
    }

    for(int i = 0; i < CAR_ENEMY_COUNT; i++)
        return_value.car_enemy_position[i].z = TERRAIN_SIDE_SIZE / -2;

    level_pass_to_state_playing(&return_value);
    return return_value;
}

void level_fini(level_t level) {
    printf("unloading level data\n");
    for(int i = 0; i < MODELS_COUNT; i++){
        UnloadTexture(level.models[i].materials[0].maps[MAP_DIFFUSE].texture);
        UnloadModel(level.models[i]);
    }
}

static void process_state_playing(level_t* level){
    if(level->car_enemy_active_count < CAR_ENEMY_COUNT && level->car_enemy_position[level->car_enemy_active_count - 1].z - level->car_enemy_position[level->car_enemy_active_count].z > CAR_LENGTH * 4)
        level->car_enemy_active_count++;

    level->car_speed += CAR_ACCELERATION * GetFrameTime();
    level->car_speed = fmin(level->car_speed, CAR_MAX_SPEED);
    level->terrain_position.z += level->car_speed * 1.4 * GetFrameTime();

    for(int i = 0; i < level->car_enemy_active_count; i++){
        level->car_enemy_position[i].z += level->car_speed * GetFrameTime();

        if(level->car_enemy_position[i].z > CAR_Z_POSITION){
            level->car_enemy_position[i].z -= TERRAIN_SIDE_SIZE / 2;
            level->car_enemy_position[i].x = GetRandomValue(-1, 1) * CAR_LATERAL_SPEED;
        }
    }

    if(level->terrain_position.z > 20.f) 
        level->terrain_position.z -= 20.f;

    BeginDrawing();
    {
        ClearBackground(WHITE);
        DrawFPS(10, 10);

        BeginMode3D(level->screen.camera);
        {
            DrawModel(level->models[MODELS_TERRAIN], level->terrain_position, 1.f, WHITE);
            DrawModel(level->models[MODELS_CAR_HERO], level->car_position, 1.f, WHITE);
            for(int i = 0; i < level->car_enemy_active_count; i++)
                DrawModel(level->models[MODELS_CAR_ENEMY], level->car_enemy_position[i], 1.f, WHITE);

            for(int i = 0; i < TREE_COUNT; i++){
                level->tree_positions[i].z = i * 10 + level->terrain_position.z - 200;
                DrawModel(level->models[MODELS_TREE], level->tree_positions[i], 1.f, WHITE);
                level->tree_positions[i].x *= -1;
                DrawModel(level->models[MODELS_TREE], level->tree_positions[i], 1.f, WHITE);
            }

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

        if (IsKeyDown(KEY_KP_1))
            level->screen.camera.position.x += .1f;
        if (IsKeyDown(KEY_KP_3))
            level->screen.camera.position.x -= .1f;

        if (IsKeyDown(KEY_KP_5))
            level->screen.camera = camera_init();

        if (IsKeyPressed(KEY_LEFT))
            level->car_target_position.x -= CAR_LATERAL_SPEED;
        if (IsKeyPressed(KEY_RIGHT))
            level->car_target_position.x += CAR_LATERAL_SPEED;
        if (IsKeyPressed(KEY_UP))
            level->car_target_position.z -= CAR_LATERAL_SPEED;
        if (IsKeyPressed(KEY_DOWN))
            level->car_target_position.z += CAR_LATERAL_SPEED;

        level->car_target_position.x = fmin(CAR_LATERAL_SPEED, fmax(level->car_target_position.x, -CAR_LATERAL_SPEED));
        level->car_position = Vector3Lerp(level->car_position, level->car_target_position, CAR_LERP_SPEED * GetFrameTime());
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

void shader_init(level_t* level){
    level->shader = LoadShader(FormatText("./assets/shaders/glsl%i/base_lighting.vs", GLSL_VERSION),
                            FormatText("./assets/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    level->shader.locs[LOC_MATRIX_MODEL] = GetShaderLocation(level->shader, "matModel");
    level->shader.locs[LOC_VECTOR_VIEW] = GetShaderLocation(level->shader, "viewPos");

    int ambientLoc = GetShaderLocation(level->shader, "ambient");
    SetShaderValue(level->shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, UNIFORM_VEC4);

    level->light_1 = CreateLight(LIGHT_POINT, (Vector3){ -60.f, 100.f, 100.f}, Vector3Zero(), WHITE, level->shader);
    UpdateLightValues(level->shader, level->light_1);
}
#endif