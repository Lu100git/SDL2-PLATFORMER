#include "TileMap.h"

/*
    I coded this from scratch, horrible experince 1 / 10
    but I learned about CSV files, this could be cosnsider as my
    Matix tile map gen, no AI was used here, unly pure skill, pain
    and faith, lots and lots of faith
*/

TileMap::TileMap(string texture_path, string csv_path, SDL_Renderer*& renderer){
    map_id = texture_path;
    loadTexture(texture_path, renderer);
    SDL_QueryTexture(m_texture, NULL, NULL, &texture_width, NULL);
    rows = getRowsFromCsvFile(csv_path);
    columns = getColumnsFromCsvFile(csv_path);
    matrix = createMatrix(csv_path);
    //printMatrix();
    createTileMap();
}

void TileMap::draw(SDL_Renderer*& renderer, const int window_width, const int window_height){
    if(!testing_spawn_tiles){
        for(unsigned int i = 0; i < data.size(); ++i){
            if(isOnScreen(window_width, window_height, i) && data[i].id < 237){
                SDL_RenderCopyEx(renderer, m_texture, &data[i].src_rect, &data[i].pos_rect, 0, 0, SDL_FLIP_NONE);
            }
        }
    }
    else if(testing_spawn_tiles){
        for(unsigned int i = 0; i < data.size(); ++i){
            if(isOnScreen(window_width, window_height, i)){
                SDL_RenderCopyEx(renderer, m_texture, &data[i].src_rect, &data[i].pos_rect, 0, 0, SDL_FLIP_NONE);
            }
        }
    }

}

bool TileMap::isOnScreen(const int window_width, const int window_height, const int i){
    if(data[i].pos_rect.x > window_width) return false;
    else if(data[i].pos_rect.x + data[i].pos_rect.w < 0) return false;
    if(data[i].pos_rect.y > window_height) return false;
    else if(data[i].pos_rect.y + data[i].pos_rect.h < 0) return false;
    else return true;
}

// we do += insrtead of =, to prevent all the tiles
// going in the same coordinates, there might be better way,
// but ain't got time for it right now!
void TileMap::setPosition(const int x, const int y){
    for(unsigned int i = 0; i < data.size(); ++i){
        data[i].pos_rect.x += x;
        data[i].pos_rect.y += y;
    }
}

// Rused for collisin detection with the player
SDL_Rect TileMap::getTilePosRect(int i) const {
    return data[i].pos_rect;
}

SDL_Rect TileMap::getTilePosRectById(const int id) const {
    SDL_Rect tile;
    for(unsigned int i = 0; i < data.size(); ++i){
        if(data[i].id == id){
            tile.x = data[i].pos_rect.x;
            tile.y = data[i].pos_rect.y;
            tile.w = data[i].pos_rect.w;
            tile.h = data[i].pos_rect.h;
            break;
        }
    }

    return tile;
}

single_tile_data TileMap::getTileData(int i) const {
    const int id = data[i].id;
    const bool is_hazzard = data[i].is_hazzard;
    const bool is_solid = data[i].is_solid;
    return{id, is_hazzard, is_solid};
}

int TileMap::getTilesAmount() const{
    int size = data.size();
    return size;
}

const int TileMap::getTileSize(){
    return tile_size;
}

void TileMap::scrollLeft(const int amount){
    for(unsigned int i = 0; i < data.size(); ++i){
        data[i].pos_rect.x -= amount;
    }
}

void TileMap::scrollRight(const int amount){
    for(unsigned int i = 0; i < data.size(); ++i){
        data[i].pos_rect.x += amount;
    }
}

void TileMap::scrollUp(const int amount){
    for(unsigned int i = 0; i < data.size(); ++i){
        data[i].pos_rect.y -= amount;
    }
}

void TileMap::scrollDown(const int amount){
    for(unsigned int i = 0; i < data.size(); ++i){
        data[i].pos_rect.y += amount;
    }
}
 
// this method borrows the data from the 2D array created as a ponter of pointers
// that holds the id of each tile, according to the id of the tile,
// and position in the 2D array, each tile will be assigned with data,
// and then pushed in the data vector
void TileMap::createTileMap(){
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < columns; ++j){
            int id = matrix[i][j];

            if(id == 0){
                tile t;

                t.id = id;
                t.pos_rect.x = j * tile_size * scale;
                t.pos_rect.y = i * tile_size * scale;
                t.pos_rect.w = tile_size * scale;
                t.pos_rect.h = tile_size * scale;
                
                t.src_rect.x = 0;
                t.src_rect.y = 0;
                t.src_rect.w = tile_size;
                t.src_rect.h = tile_size;

                t.is_hazzard = false;
                t.is_solid = true;
                data.push_back(t);
            }

            else if(id > 0){
                bool hazzard = false;
                bool solid = false;

                // spikes NO DAMAGE LOGIC APPLIED YET
                if (id >= 225 && id <=227) {
                    hazzard = true;
                    solid = false;
                }

                // invisible tiles that control the scrolling of the map
                else if(id >= 237){
                    hazzard = false;
                    solid = false;
                }
                // if its not a hazzard it's a solid tile
                else{
                    hazzard = false;
                    solid = true;
                }
                
                // third flther, walkable pillars
                switch(id){
                    case 98:
                    case 99:
                    case 100:
                    case 114:
                    case 115:
                    case 116:
                    case 130:
                    case 131:
                    case 132:
                    case 146:
                    case 147:
                    case 148:
                    case 162:
                    case 163:
                    case 164:
                        hazzard = false;
                        solid = false;
                    break;
                }

                // MAGIC TILE FORMULA FOR SPRITE SHEET
                tile t;
                t.id = id;

                t.pos_rect.x = j * tile_size * scale;
                t.pos_rect.y = i * tile_size * scale;
                t.pos_rect.w = tile_size * scale;
                t.pos_rect.h = tile_size * scale;

                t.src_rect.x = id % (texture_width / tile_size) * tile_size;
                t.src_rect.y = id / (texture_width / tile_size) * tile_size;
                t.src_rect.w = tile_size;
                t.src_rect.h = tile_size;

                t.is_hazzard = hazzard;
                t.is_solid = solid;
                data.push_back(t);
            }
        }
    }
}

// ****************************************************************
// these methods bellow are the reason of my existential crisis,
// and why re-inventing the wheel is a bad idea,
// life is too short to be a single indie c++ game dev 
// ****************************************************************

// this method return the amount of columns in a csv file
int TileMap::getColumnsFromCsvFile(string csv_path){
    int columns = 0;
    std::ifstream csv_reader;
    csv_reader.open(csv_path);
    if(csv_reader.fail()){
        std::cerr << "ERROR!, Unable to open ther CSV file [getColumns function] " << std::endl;
        return -1;
    }

    string line;
    std::getline(csv_reader, line, '\n');

    for(unsigned int i = 0; i < line.length(); ++i){
        if (line[i] == ',') continue;
        else if (line[i] == '-') continue;
        columns++;
    }

    csv_reader.close();
    return columns;
}

// this method returns the amount of rows in a csv file
int TileMap::getRowsFromCsvFile(string csv_path){
    int rows = 0;
    std::ifstream csv_reader;
    csv_reader.open(csv_path);
    if(csv_reader.fail()){
        std::cerr << "ERROR!, Unable to open ther CSV file [getRows function]" << std::endl;
        return -1;
    }

    string line;
    while(csv_reader.peek() != EOF){
        std::getline(csv_reader, line, '\n');
        rows++;
    }

    csv_reader.close();
    return rows;
}

// this method initializes a 2D pointer of pointers
// ans separates the commas so each element in the array
// will be just a raw id, so it can be processed in the crerate tilemap method
// this concept is similar to tetris
int** TileMap::createMatrix(string csv_path){
    int** matrix = new int*[rows];
    for(int i = 0; i < rows; i++){
        matrix[i] = new int[columns];
        // this prevents glitches by initializing the matrix with -1's
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = -1; // Initialize each element to -1
        }
    }

    std::ifstream csv_reader;
    csv_reader.open(csv_path);
    string line;
    int i = 0;
    int j = 0;

    while(getline(csv_reader, line)){
        std::stringstream ss(line);
        string value;
        while(getline(ss, value, ',')){
            matrix[i][j] = atoi(value.c_str());
            j++;
        }
        i++;
        j = 0;
    }
    
    csv_reader.close();
    cout << "Matrix created successfully √ " << endl;
    return matrix;
}

void TileMap::printMatrix(){
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < columns; j++){
            std::cout << "[" << matrix[i][j] << "]" << " ";
        }
        std::cout << std::endl;
    }
}


void TileMap::loadTexture(string path, SDL_Renderer*& renderer){
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr){
        cout << "error, image " << path.c_str() << " was not loaded! SDL2_IMAGE_ERROR: " << IMG_GetError() << endl;
        m_texture = nullptr;
    } else {
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if(m_texture == nullptr) cout << "error, texture was not created! SDL2_ERROR: " << SDL_GetError() << endl; 
        else cout << "texture [ " << map_id << " ] created successfully √" << endl;
        SDL_FreeSurface(surface);
    }
}

TileMap::~TileMap(){
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i]; // Delete each row
    }
    delete[] matrix;
    cout << "matrix pointers** from: TileMap have been deleted" << endl;

    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
    cout << "[ " << map_id << " ] Texture has been destroyed!" << endl;
    
}
