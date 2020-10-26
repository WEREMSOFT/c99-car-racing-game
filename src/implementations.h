#ifndef __IMPLEMENTATIONS_H__
#define __IMPLEMENTATIONS_H__

#if defined(OS_WEB)
#define GLSL_VERSION            100
#else   // PLATFORM_WEB
#define GLSL_VERSION            330
#endif

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "game/constants.h"
#define __LEVEL_H_IMPLEMENTATION__
#include "game/screens/level.h"
#define __MENU_H_IMPLEMENTATION__
#include "game/screens/menu.h"
#define __GAME_H_IMPLEMENTATION__
#include "game/game.h"
#define __SCREEN_UTILS_H_IMPLEMENTATION__
#include "game/screens/screen_utils.h"


#endif