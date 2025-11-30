#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include "Player.h"
#include "TileMap.h"

using std::cout;
using std::endl;
using std::string;

class Camera {
    public:
        SDL_Rect camera_rect;
        void follow(Player& player, TileMap& map, int window_width, int window_height, double delta);
};   

#endif // CAMERA_H
