#include <stdio.h>
#include <raylib.h>

#define PETROL CLITERAL(Color){ 0, 0x4A, 0x6D, 0xFF }
#define WIDTH 800
#define HEIGHT 600

#include "implementations.h"

#ifdef OS_WEB
#include <emscripten/emscripten.h>
#endif


int main(void)
{
#ifdef OS_Windows_NT
    printf("Windows dettected\n");
#elif defined OS_Linux
    printf("LINUS dettected\n");
#elif defined OS_Darwin
    printf("MacOS dettected\n");
#endif

    InitWindow(WIDTH, HEIGHT, "AWESOME RACING!!!");
    SetTargetFPS(60);

    game_context_t context = game_init();

#ifdef OS_WEB
    emscripten_set_main_loop_arg(update_frame, &context, 0, 1);
#else
    while (!WindowShouldClose())
    {
        game_update(&context);
    }
#endif

    game_fini(context);
    CloseWindow();

    return 0;
}