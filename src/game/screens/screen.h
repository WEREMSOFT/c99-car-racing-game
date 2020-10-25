#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <raylib.h>

typedef enum screen_state_e {
    SCREEN_STATE_PLAYING,
    SCREEN_STATE_DEAD,
    SCREEN_STATE_PAUSE
} screen_state_e;

typedef struct screen_t {
    Camera3D camera;
    screen_state_e state;
} screen_t;

#endif