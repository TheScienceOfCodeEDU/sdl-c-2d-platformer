#ifndef UNITY_BUILD
#pragma once
 #include "p2d_memory.h"
 #include "p2d_sprites.h"
 #include "p2d_globals.h"
 #include "p2d_structs.h"
 #ifdef __MINGW32__
  #include <SDL.h>
  #include <SDL_image.h>
 #else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
 #endif
#endif

function character *
character_CreatePlayer(arena *Arena, SDL_Renderer *Renderer)
{
    character_anim *Animations = LoadCharacterAnimations(Arena, Renderer);
    character *Character = (character*) ReserveMemory(Arena, sizeof(character));
    Character->X = 0;
    Character->Y = 200;
    Character->W = 15;
    Character->H = 22;
    Character->Animation = Animations;

    return Character;
}

function anim_sprites *
character_GetCurrentAnimationSet(character *Character)
{
    anim_sprites *CurrentSprites = Character->Animation->AnimationsPerState;
    for (int i = 0; i < Character->Animation->State; ++i)
    {
        ++CurrentSprites;
    }
    return CurrentSprites;
}

function SDL_Rect *
character_GetSpriteRect(anim_sprites *Sprites, int Frame) 
{
    SDL_Rect *CurrentRect = Sprites->SpritesRects;
    for (int i = 0; i < Frame; ++i) 
    {
        CurrentRect++;
    }
    return CurrentRect;
}
