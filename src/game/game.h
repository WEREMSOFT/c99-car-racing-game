#ifndef __GAME_H__
#define __GAME_H__

#include "screens/level.h"

typedef struct game_context_t {
    level_t level;
} game_context_t;

game_context_t game_init();
void game_fini(game_context_t context);
void game_update(game_context_t* game_context);

#endif

#ifdef __GAME_H_IMPLEMENTATION__
#undef __GAME_H_IMPLEMENTATION__

#include "game.h"
#include <raylib.h>

void game_update(game_context_t* context){
    context->level.update(&context->level);
}

game_context_t game_init(){
    game_context_t return_value = {0};
    return_value.level = level_init();
    
    return return_value;
}

void game_fini(game_context_t context){
    level_fini(context.level);
}

#endif