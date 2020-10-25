#ifndef __SCREEN_UTILS_H__
#define __SCREEN_UTILS_H__

#include <raylib.h>

Camera3D camera_init(void);

#endif

#ifdef __SCREEN_UTILS_H_IMPLEMENTATION__
#undef __SCREEN_UTILS_H_IMPLEMENTATION__

Camera3D camera_init(void) {
    Camera3D return_value = {0};

    return_value.fovy = 45.0f;
    return_value.position = (Vector3){0.0f, 10.0f, 53.0f};
    return_value.target = (Vector3){0.0f, 0.0f, 20.0f};
    return_value.up = (Vector3){0.0f, 1.0f, 0.0f};
    return_value.type = CAMERA_PERSPECTIVE;

    return return_value;
}

#endif