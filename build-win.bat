@echo off
mkdir "build/win" 2>NUL
gcc src/p2d.c -IC:\sdl2\include\SDL2 -LC:\sdl2\lib -g -ggdb -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -o build/win/game -gcodeview
