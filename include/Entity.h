#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#define RED 255, 0, 0
#define GREEN 0,255, 0
#define BLUE 0, 0, 255
#define PURPLE 75,0,130

using std::cout;
using std::endl;
using std::string;

struct Vector2{
    float x, y;
};

class Entity{
    public:
        Entity(int x, int y, int w, int h);
        Entity(int x, int y, int w, int h, string file, SDL_Renderer*& renderer);
        void draw(SDL_Renderer*& renderer);
        bool isOnScreen(const int window_width, const int window_height);
        void setSourceRect(int x, int y, int w, int h);
        void setSrcRectTextureSize();
        void loadTexture(string path, SDL_Renderer*& renderer);
        Vector2 getPos() const;
        const int getWidth();
        const int getHeight();
        SDL_Rect getPosRect();
        void setPosX(int x);
        void setPosY(int y);
        void setSpriteRectW(const int w);
        void setSpriteRectH(const int h);
        void registerBackground();
        virtual ~Entity();
        bool testing_bounds = false;
    protected:
        string id;
        SDL_Rect src_rect;
        SDL_Rect pos_rect;
        SDL_Rect sprite_rect;
        SDL_Texture* m_texture = nullptr;
        float x_pos = 0;
        float y_pos = 0;
        int width = 0;
        int height = 0;
        int texture_width = 0;
        int texture_height = 0;
        bool flipped = false;
        bool is_background = false;
        Mix_Chunk* jump_sound = nullptr;
    };

#endif // ENTITY_H
