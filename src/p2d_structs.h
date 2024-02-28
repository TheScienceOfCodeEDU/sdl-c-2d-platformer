#ifndef UNITY_BUILD
 #pragma once
 #ifdef __MINGW32__
  #include <SDL.h>
  #include <SDL_image.h>
 #else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
 #include "p2d_globals.h" 
 #endif
#endif


const uint8 E_CHARACTER_STATES_SIZE = 2;
enum character_state {
    E_CHARACTER_STATE_IDLE,
    E_CHARACTER_STATE_WALKING
};

struct animation_sprites {
    SDL_Rect *SpritesRects;
    int Count;
};


struct character_animations {
    character_state State;
    int Frame;
    int NextUpdate;

    SDL_Texture *SpritesTexture;
    animation_sprites *AnimationsPerState;
};

struct character {
    int X, Y;
    int W, H;
    bool Left, Right;

    character_animations *Animations;
};

