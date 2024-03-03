#ifndef UNITY_BUILD
 #pragma once
 #ifdef _WIN64
  #include <SDL.h>
  #include <SDL_image.h>
 #else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h> 
 #endif
 #include "p2d_globals.h"
#endif


const uint8 E_CHARACTER_STATES_SIZE = 2;
enum character_state {
    E_CHARACTER_STATE_IDLE,
    E_CHARACTER_STATE_WALKING
};

struct {
    SDL_Rect *SpritesRects;
    int Count;
} typedef animation_sprites;


struct {
    character_state State;
    int Frame;
    int NextUpdate;

    SDL_Texture *SpritesTexture;
    animation_sprites *AnimationsPerState;
} typedef character_animations;

struct {
    int X, Y;
    int W, H;
    bool Left, Right;

    character_animations *Animations;
} typedef  character;

