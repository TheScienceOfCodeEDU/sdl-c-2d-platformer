#ifndef UNITY_BUILD
 #pragma once
 #ifdef __MINGW32__
  #include <SDL.h>
  #include <SDL_image.h>
 #else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
 #endif
 #include "p2d_memory.h"
 #include "p2d_globals.h"
 #include "p2d_structs.h"
#endif


inline function camera
camera_MakeCamera(character *Focus) 
{
    camera Camera = {};
    Camera.Focus = Focus;

    return Camera;
}

inline function void
camera_Update(camera *Camera)
{
    Camera->X = Camera->Focus->X - SCREEN_W_CAM + Camera->Focus->W_MID;
    Camera->Y = Camera->Focus->Y - SCREEN_H_CAM;    
}
