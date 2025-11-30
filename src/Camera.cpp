#include "Camera.h"
#define UP 237
#define LEFT 238
#define RIGHT 239
#define SUN 254
#define BLACK_SUN 255

void Camera::follow(Player& player, TileMap& map, int window_width, int window_height, double delta){
    const SDL_Rect& player_pos_rect =  player.getPosRect();
    camera_rect.w = player_pos_rect.w * 5;
    camera_rect.h = player_pos_rect.h * 3;
    camera_rect.x = player_pos_rect.x - (camera_rect.w / 2) + (player_pos_rect.w / 2);
    camera_rect.y = player_pos_rect.y - (camera_rect.h / 2) + (player_pos_rect.h / 2);            
    
    //prevent out of bounds unless a special tiles is reached, and scroll the tiles
    // LEFT
    if(camera_rect.x < 0 ){
        if(map.getTilePosRectById(SUN).x < camera_rect.x){
            player.moveForward(delta);
            const int amount = player.getSpeed() * delta;
            map.scrollRight(amount);
        }
    }
    // RIGHT
    else if(camera_rect.x + camera_rect.w > window_width){
        if(map.getTilePosRectById(BLACK_SUN).x > camera_rect.x + camera_rect.w){
            player.moveBackward(delta);
            const int amount = player.getSpeed() * delta;
            map.scrollLeft(amount);
        }

    }
    // DOWN
    if(camera_rect.y  + camera_rect.h > window_height){
        if(camera_rect.y + camera_rect.h <  map.getTilePosRectById(BLACK_SUN).y){
            player.moveUp(delta);
            const int amount = abs(player.getVelocityY() * delta);
            map.scrollUp(amount);
        }

    }
    // UP
    else if(camera_rect.y < 0){
        if(camera_rect.y > map.getTilePosRectById(SUN).y){
            player.moveDown(delta);
            const int amount = abs(player.getVelocityY() * delta);
            map.scrollDown(amount);
        }
    }
}