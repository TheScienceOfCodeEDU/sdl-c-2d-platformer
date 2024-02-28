#ifndef UNITY_BUILD
 #pragma once
 #include "p2d_memory.h"
 #include "p2d_sdl_utils.h"
 #include "p2d_structs.h"
 #ifdef __MINGW32__
  #include <SDL.h>
  #include <SDL_image.h>
 #else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
 #endif
#endif

function character_anim *
LoadCharacterAnimations(arena *Arena, SDL_Renderer *Renderer)
{
    assert(E_ANIM_STATES_SIZE == 2);
    assert(E_ANIM_STATE_IDLE == 0);
    assert(E_ANIM_STATE_WALKING == 1);

    const int IDLE_COUNT = 1;
    SDL_Rect *IdleSpritesRects = (SDL_Rect *) ReserveMemory(Arena, sizeof(SDL_Rect) * IDLE_COUNT);
    SDL_Rect *CurrentIdleSprite = IdleSpritesRects;
    *IdleSpritesRects = { 9, 42, 15, 22};

    const int WALKING_COUNT = 4;
    SDL_Rect *WalkingSpritesRects = (SDL_Rect *) ReserveMemory(Arena, sizeof(SDL_Rect) * WALKING_COUNT);
    SDL_Rect *CurrentWalkingSprite = WalkingSpritesRects;
    *CurrentWalkingSprite = { 41, 42, 15, 22};
    *(++CurrentWalkingSprite) = {72, 42, 16, 22};
    *(++CurrentWalkingSprite) = {104, 41, 17, 22};
    *(++CurrentWalkingSprite) = {9, 42, 15, 22};

    
    anim_sprites *Sprites = (anim_sprites *) ReserveMemory(Arena, sizeof(anim_sprites) * E_ANIM_STATES_SIZE);
    anim_sprites *CurrentSprite = Sprites;
    CurrentSprite->SpritesRects = IdleSpritesRects;
    CurrentSprite->Count = IDLE_COUNT;

    ++CurrentSprite;
    CurrentSprite->SpritesRects = WalkingSpritesRects;
    CurrentSprite->Count = WALKING_COUNT;

    character_anim *Anim = (character_anim *) ReserveMemory(Arena, sizeof(character_anim));
    Anim->State = E_ANIM_STATE_IDLE;
    Anim->Frame = 0;
    Anim->NextUpdate = ANIMATION_NEXT_UPDATE;
    Anim->AnimationsPerState = Sprites;
    Anim->SpritesTexture = sdl_utils_loadTexture("res/characters.png", Renderer);

    return Anim;
}
