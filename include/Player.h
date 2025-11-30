#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Entity.h"
#include "TileMap.h"

using std::cout;
using std::endl;
using std::string;


class Player : public Entity{
    public:
        Player(int x, int y, int w, int h) : Entity(x, y, w, h){}
        Player(int x, int y, int w, int h, string file, SDL_Renderer*& renderer) : Entity(x, y, w, h, file, renderer){}
        void update(const double delta_time);
        void applyGravity(const double delta_time);
        void handleEvents(SDL_Event& event);
        bool collidesWith(const SDL_Rect& target);
        void handleHorizontalCollision(const TileMap& map);
        void handleVerticalCollision(const TileMap& map);
        void animate(const double delta);
        void updateSrcRect(const int x, const int y);
        void handleAnimationEvents(SDL_Event& event);
        void checkAnimationState();
        void moveForward(const double delta);
        void moveBackward(const double delta);
        void moveUp(const double delta);
        void moveDown(const double delta);
        const int getSpeed();
        const int getGravity();
        float getVelocityY();
        void setSoundChunk(string file);
        void showinfo();

    protected:
        // logic and physics
        struct{
            int x = 0;
            float y = 0;
        }velocity;

        const int speed = 580;
        const float gravity = 2600;
        const int jump_limit = -900;
        
        bool running = false;
        bool grounded = false;
        
        // this member is used to scroll the tiles in camera.cpp
        int velocity_y = 0;

        // animation
        unsigned short state = States::IDLE;
        enum States {
            IDLE,
            GROUNDED,
            JUMPING,
            MOVING_RIGHT,
            MOVING_LEFT,
            FALLING
        };
        int frame = 0;
        float frame_counter = 0;
        const float frame_speed = 10;
        const int max_frames = 6;
        int spritesheet_row = 0;
        
    };

#endif // PLAYER_H
