#include "Entity.h"

Entity::Entity(int x, int y, int w, int h){
    x_pos = x;
    y_pos = y;
    width = w;
    height = h;
    pos_rect.x = x_pos;
    pos_rect.y = y_pos;
    pos_rect.w = width;
    pos_rect.h = height;
    id = " primitive rect ";
}

Entity::Entity(int x, int y, int w, int h, string file, SDL_Renderer*& renderer){
    id = file;
    x_pos = x;
    y_pos = y;
    width = w;
    height = h;
    pos_rect.x = x_pos;
    pos_rect.y = y_pos;
    pos_rect.w = width;
    pos_rect.h = height;
    sprite_rect.x = x_pos;
    sprite_rect.y = y_pos;
    sprite_rect.w = width;
    sprite_rect.h = height;
    loadTexture(file, renderer);
    setSrcRectTextureSize();
}

void Entity::draw(SDL_Renderer*& renderer){
    // this makes sure to get at center as posible
    // of the boundary box
    if(is_background == false){
        sprite_rect.x = pos_rect.x - pos_rect.w / 4;
        sprite_rect.y = pos_rect.y - (sprite_rect.h - pos_rect.h);

        if(flipped){
            sprite_rect.x = pos_rect.x - sprite_rect.w / 3;
        }
    }
    // if texture is missing draw a rect, otherwise draw the texture
    if(m_texture == nullptr){
        SDL_SetRenderDrawColor(renderer, PURPLE,0);
        SDL_RenderFillRect(renderer, &pos_rect);    
    }else{
        if(testing_bounds){
            SDL_SetRenderDrawColor(renderer, RED,0);
            SDL_RenderDrawRect(renderer, &pos_rect);    
            SDL_SetRenderDrawColor(renderer, GREEN,0);
            SDL_RenderDrawRect(renderer, &sprite_rect);
        }
        if(flipped == false) SDL_RenderCopyEx(renderer, m_texture, &src_rect, &sprite_rect, 0, 0, SDL_FLIP_NONE);
        if(flipped) SDL_RenderCopyEx(renderer, m_texture, &src_rect, &sprite_rect, 0, 0, SDL_FLIP_HORIZONTAL);
    }
}

bool Entity::isOnScreen(const int window_width, const int window_height){
    if(pos_rect.x > window_width ) return false;
    else if(pos_rect.x + pos_rect.w < 0) return false;
    if(pos_rect.y > window_height) return false;
    else if(pos_rect.y + pos_rect.w < 0) return false;
    return true;
}

void Entity::setSourceRect(int x, int y, int w, int h){
    src_rect.x = x;
    src_rect.y = y;
    src_rect.w = texture_width / w;
    src_rect.h = texture_height / h;
}

void Entity::setSrcRectTextureSize(){
    SDL_QueryTexture(m_texture, NULL, NULL, &texture_width, &texture_height);
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = texture_width;
    src_rect.h = texture_height;
}

void Entity::loadTexture(string path, SDL_Renderer*& renderer){
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr){
        cout << "error, image " << path.c_str() << " was not loaded! SDL2_IMAGE_ERROR: " << IMG_GetError() << endl;
        m_texture = nullptr;
    } else {
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if(m_texture == nullptr) cout << "error, texture was not created! SDL2_ERROR: " << SDL_GetError() << endl; 
        else cout << "texture [ " << id << " ] created successfully √" << endl;
        SDL_FreeSurface(surface);
    }
}   

Vector2 Entity::getPos() const {
    return{x_pos, y_pos};
}

const int Entity::getWidth(){
    return width;
}

const int Entity::getHeight(){
    return height;
}

SDL_Rect Entity::getPosRect(){
    return pos_rect;
}

void Entity::setPosX(int x){
    x_pos = x;
}

void Entity::setPosY(int y){
    y_pos  = y;
}

void Entity::setSpriteRectW(const int w){
    sprite_rect.w += w;
}

void Entity::setSpriteRectH(const int h){
    sprite_rect.h += h;
}

void Entity::registerBackground(){
    is_background = true;
}

Entity::~Entity(){
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
    cout << "texture: [ " << id << " ] has been destroyed!" << endl;
    Mix_FreeChunk(jump_sound);
    jump_sound = nullptr;
    cout << "sound chunk has been destroyed" << endl;
}