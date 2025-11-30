#ifndef GAME_H
#define GAME_H

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "TileMap.h"

using std::cout;
using std::endl;
using std::string;

enum Rooms{
    ENTRANCE = 0,
    ROOM1A = 1,
    ROOM1B = 2
};

enum SpawnTiles{
    LEFT = 238,
    RIGHT = 239
};

class Game{
    public:
        Game(int window_width, int window_height);
        void fadeIn();
        void begin();
        int init();
        virtual ~Game();
    private:
        SDL_Event event;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        Mix_Music* music = nullptr;
        bool sdl_ready = false;
        bool game_is_running = true;
        short int WINDOW_WIDTH = 0;
        short int WINDOW_HEIGHT = 0;                
        const int SCREEN_FPS = 60;
        const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
        bool testing_camera = false;
        std::vector<std::unique_ptr<TileMap>> maps;
        short unsigned int current_room = 0;
        unsigned short int fader_alpha = 0;
        SDL_Rect fader;
};

#endif // GAME_H
