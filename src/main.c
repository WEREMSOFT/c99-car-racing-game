#include <stdio.h>
#include <raylib.h>

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
    emscripten_set_main_loop_arg(game_update, &context, 0, 1);
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