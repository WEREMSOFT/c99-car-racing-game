#ifndef __MENU_H__
#define __MENU_H__

#include "screen.h"

typedef enum menu_states_e {
    STATE_INTRO,
    STATE_READY,
    STATE_OUTRO,
} menu_states_e;

typedef struct menu_t {
    screen_t screen;
} menu_t;

menu_t menu_init();
void menu_update();
void menu_fini();

#endif

#ifdef __MENU_H_IMPLEMENTATION__
#undef __MENU_H_IMPLEMENTATION__
menu_t menu_init(){
    menu_t return_value = {0};
    return return_value;
}
void menu_update(void){
    while(!(IsKeyPressed(KEY_ENTER) || WindowShouldClose())){
        BeginDrawing();
        {
            ClearBackground(WHITE);
            DrawText("Press Enter to Start, ESC to close", 100, 100, 14, BLACK);
        }EndDrawing();
    }
}
void menu_fini();
#endif