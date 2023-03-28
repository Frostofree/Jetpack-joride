# Jetpack Joyride

## Build Instructions

The Project is built using cmake. If you do not have cmake installed on your system -
> sudo apt-get -y install cmake

Once that is done, run the following commands in the Jetpack Joyride folder to start

```shell
cmake . ;
make ;
./app ;
```

After that, press Enter to start playing!

## Contents
- glfw, glad and glm built from source in `libraries`

- Standard shader class from learnopengl.com and stb_image.h in `include`

- CMake file to compile project (includes glfw, glad, glm and freetype)

- freetype2 was installed on system, linked to include path in /usr/....\

- Utilised multiple game assets from the original jetpack joyride, gameassets2D.com, etc.

## Features Implemented

- [x] Levels   
- [x] Movement
- [x] Zappers
- [x] Coins
- [x] HUD
- [x] Special Effects (GLSL SlowStep)





