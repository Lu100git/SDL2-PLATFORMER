#ifndef TILEMAP_H
#define TILEMAP_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream> // for opening files
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL_image.h>

using std::cout;
using std::endl;
using std::string;

struct single_tile_data{
    const int id;
    const bool is_hazzard;
    const bool is_solid;
};

class TileMap{
    public:
        TileMap(string texture_path, string csv_path, SDL_Renderer*& renderer);
        void draw(SDL_Renderer*& renderer, const int window_width, const int window_height);
        bool isOnScreen(const int window_width, const int window_height, const int i);
        void setPosition(const int x, const int y);
        SDL_Rect getTilePosRect(int i) const;
        SDL_Rect getTilePosRectById(const int id) const;
        single_tile_data getTileData(int i) const;
        int getTilesAmount() const;
        const int getTileSize();
        void scrollLeft(const int amount);
        void scrollRight(const int amount);
        void scrollUp(const int amount);
        void scrollDown(const int amount);
        void createTileMap();
        int getColumnsFromCsvFile(string csv_path);
        int getRowsFromCsvFile(string csv_path);
        int **createMatrix(string csv_path);
        void printMatrix();
        void loadTexture(string path, SDL_Renderer*& renderer);
        virtual ~TileMap();
    private:
        string map_id;    
        SDL_Texture* m_texture;
        int **matrix;
        int rows = 0;
        int columns = 0;
        int texture_width = 0;
        const int tile_size = 16;
        const int scale = 4;
        struct tile{
            int id = -1;
            SDL_Rect pos_rect = {0,0, 0, 0};
            SDL_Rect src_rect = {0, 0, 0 ,0};
            bool is_hazzard = false;
            bool is_solid = false;
        };
        std::vector<tile> data;
        const bool testing_spawn_tiles = false;
};

#endif // TILEMAP_H
