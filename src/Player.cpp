#include "Player.h"

void Player::update(const double delta_time){
    x_pos += velocity.x * delta_time;
    pos_rect.x = (int) x_pos;
    if(velocity.x != 0 ) running = true;
    else running = false;
}

void Player::applyGravity(const double delta_time){
    velocity.y += gravity * delta_time;
    y_pos += velocity.y * delta_time;
    pos_rect.y = (int) y_pos;

    // terminal velocity
    if(velocity.y >= 3000){
        velocity.y = 3000;
    }

    // prevents double jumps
    if (velocity.y > 60 || velocity.y < 30) grounded = false;

    // this value is used for scrolling the tiles
    velocity_y = velocity.y;
}


void Player::handleEvents(SDL_Event& event){
    if(event.type == SDL_KEYDOWN && event.key.repeat == 0){
        switch(event.key.keysym.sym){
            case SDLK_SPACE:
                if(grounded){
                    velocity.y = jump_limit;
                    grounded = false;
                    Mix_PlayChannel( -1, jump_sound, 0 );
                }
            break;
            case SDLK_LEFT: velocity.x -= speed; break;
            case SDLK_RIGHT: velocity.x += speed; break;
        }
    }

    else if(event.type == SDL_KEYUP && event.key.repeat == 0){
        switch(event.key.keysym.sym){
            case SDLK_LEFT: velocity.x += speed; break;
            case SDLK_RIGHT: velocity.x -= speed; break;
        }
    }
}

// formula source: https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
bool Player::collidesWith(const SDL_Rect& target){
    int topA = y_pos;
    int leftA = x_pos;
    int rightA = x_pos + width;
    int bottomA = y_pos + height;

    int topB = target.y;
    int leftB = target.x;
    int rightB = target.x + target.w;
    int bottomB = target.y + target.h;

    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;

    return true;
}

// credits to: Chris Courses: https://youtu.be/rTVoyWu8r6g?si=9CDgk_pgw9y0S5Ru&t=5394 
// this section is a nightmare to deboug when you have no references
void Player::handleHorizontalCollision(const TileMap& map){
    for(unsigned int i = 0; i < map.getTilesAmount(); ++i){
        
        const bool current_tile_solid = map.getTileData(i).is_solid;
        if(current_tile_solid == false) continue;

        const SDL_Rect& current_tile = map.getTilePosRect(i);

        if(collidesWith(current_tile) && pos_rect.x < current_tile.x){
            if(velocity.x > 0){
                x_pos = current_tile.x - width;
                pos_rect.x = x_pos;
            }
        }

        if(collidesWith(current_tile) && current_tile.x < pos_rect.x){
            if(velocity.x < 0){
                x_pos = current_tile.x + current_tile.w;
                pos_rect.x = x_pos;
            }
        }
    }
}


void Player::handleVerticalCollision(const TileMap& map){
    for(unsigned int i = 0; i < map.getTilesAmount(); ++i){
        
        const bool current_tile_solid = map.getTileData(i).is_solid;
        if(current_tile_solid == false) continue;
        
        const SDL_Rect& current_tile = map.getTilePosRect(i);
        
        if(collidesWith(current_tile)){
            if(velocity.y > 0 && y_pos < current_tile.y){
                y_pos = current_tile.y - height;
                pos_rect.y = y_pos;
                velocity.y = 0;
                grounded = true;
            }
        }

        if(collidesWith(current_tile)){
            if(velocity.y < 0 && current_tile.y < y_pos){
                velocity.y = 0;
                y_pos = current_tile.y + current_tile.h;
                
            }
        }
    }
}

// animation is explained in https://lazyfoo.net/tutorials/SDL 
//I modified it for delta time
void Player::animate(const double delta){
    frame_counter += frame_speed * delta;
    if(frame_counter >= max_frames) frame_counter = 0;
    frame = frame_counter;
    updateSrcRect(frame * src_rect.w, spritesheet_row * src_rect.h);
    checkAnimationState();

}

void Player::updateSrcRect(const int x, const int y){
    src_rect.x = x;
    src_rect.y = y;
}

void Player::handleAnimationEvents(SDL_Event& event){
   if(event.type == SDL_KEYDOWN && event.key.repeat == 0){
        switch(event.key.keysym.sym){
            case SDLK_SPACE:
                if(grounded) state = States::JUMPING; 
            break;

            case SDLK_LEFT:
                flipped = true;  
                state = States::MOVING_LEFT; 
            break;

            case SDLK_RIGHT:
                flipped = false;
                state = States::MOVING_RIGHT;
            break;
        }
    }

    else if(event.type == SDL_KEYUP && event.key.repeat == 0){
        switch(event.key.keysym.sym){
            case SDLK_SPACE:
                state = States::FALLING; 
            break;
            case SDLK_LEFT:
                state = States::IDLE; 
            break;

            case SDLK_RIGHT: 
                state = States::IDLE; 
            break;
        }
    }
}

void Player::checkAnimationState(){
    switch(state){
        case States::IDLE:
            if(grounded && !running) spritesheet_row = 0;
        break;
        
        case States::MOVING_RIGHT: 
            if(grounded) spritesheet_row = 1;
        break;
        
        case States::MOVING_LEFT:
            if(grounded) spritesheet_row = 1;
        break;
        
        case States::JUMPING:
           if(!grounded) spritesheet_row = 7;
           if(grounded) state = States::GROUNDED;
        break;

        case States::FALLING:
           if(grounded) state = States::GROUNDED;
        break;

        case States::GROUNDED:
            if(velocity.x > 0) state = States::MOVING_RIGHT;
            else if(velocity.x < 0) state = States::MOVING_LEFT;
            else state = States::IDLE;
        break;
    
    }
    // fixes animation double key press glitch
    if(flipped and velocity.x > 0) flipped = false;
    else if(!flipped and velocity.x < 0) flipped = true;
}


// these are used for the camera movement, moving the player this way
// prevents colision detection glitches
void Player::moveForward(const double delta){
    x_pos += speed * delta;
}

void Player::moveBackward(const double delta){
    x_pos -= speed * delta;
}

void Player::moveUp(const double delta){
    y_pos -= (int) abs(velocity.y * delta);
}

void Player::moveDown(const double delta){
    y_pos += (int) abs(velocity.y * delta);
}

const int  Player::getSpeed(){
    return speed;
}

const int Player::getGravity(){
    return gravity;
}

float Player::getVelocityY(){
    return velocity_y;
}

void Player::setSoundChunk(string file){
    jump_sound = Mix_LoadWAV(file.c_str());
    if(jump_sound == nullptr) cout << "failed to load: " << file << endl;
    else cout << file << " has been loaded " << endl;
}

void Player::showinfo(){
    cout << "Player Data: [" 
    << " X: " << (int) x_pos
    << " Y: " << (int) y_pos
    << "] " 
    << "is grounded: " << (grounded ? " yes " : " no ")
    << " velocity Y: " << (int) velocity.y  
    << endl;
}