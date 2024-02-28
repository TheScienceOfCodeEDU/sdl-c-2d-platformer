#ifndef UNITY_BUILD
 #include "p2d_globals.h"
 #ifdef __MINGW32__
  #include <SDL.h>
  #include <SDL_image.h>
 #else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
 #endif
#endif


struct anim_sprites {
    SDL_Rect *SpritesRects;
    int Count;
};


const uint8 E_ANIM_STATES_SIZE = 2;
enum character_anim_state {
    E_ANIM_STATE_IDLE,
    E_ANIM_STATE_WALKING
};

struct character_anim {
    character_anim_state State;
    int Frame;
    int NextUpdate;

    SDL_Texture *SpritesTexture;
    anim_sprites *AnimationsPerState;
};

struct character {
    int X, Y;
    int W, H;

    character_anim Animation;
};

