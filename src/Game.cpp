#include "Game.h"

#define MAP_LEFT_ENTRANCE -220

Game::Game(int window_width, int window_height){
    WINDOW_WIDTH = window_width;
    WINDOW_HEIGHT = window_height;

    if(init() < 0){
        cout << "ERROR...SDL2 could not initialized " << endl;
        sdl_ready = false;
        game_is_running = false;
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        Mix_FreeMusic(music);
        music = NULL;     
        window = nullptr;
        renderer = nullptr;
        IMG_Quit();
    }
    else{
        sdl_ready = true;
        game_is_running = true;
        cout << "SDL2 has successfully been initialized " << endl;
    }
}

void Game::begin(){
    if(sdl_ready){
//      --- LOADING ASSETS ---
        fader.x = 0;
        fader.y = 0;
        fader.w = WINDOW_WIDTH;
        fader.h = WINDOW_HEIGHT;
        Camera camera;
        Player player(0,WINDOW_HEIGHT / 2,80,120,"assets/warrior.png", renderer);
        player.setSpriteRectW(69);
        player.setSpriteRectH(44);
        player.setSourceRect(0,0,6,17);
        player.setSoundChunk("sound/jump.wav");
        Entity entrance_background(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, "assets/exterior.png", renderer);
        Entity interior_background(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, "assets/interior.png", renderer);
        entrance_background.registerBackground();
        interior_background.registerBackground();

        // AI helped me here, since I was having memory core dumps not sure why, untill AI explained unique pointers
        maps.push_back(std::make_unique<TileMap>("assets/twilight-tiles-mod.png", "data/entrance.csv", renderer));
        maps.push_back(std::make_unique<TileMap>("assets/twilight-tiles-mod.png", "data/room1A.csv", renderer));
        // top view MAP_LEFT_ENTRANCE + 180

        // initial map positions
        maps[Rooms::ENTRANCE]->setPosition(MAP_LEFT_ENTRANCE, -player.getPos().y);
        maps[Rooms::ROOM1A]->setPosition(MAP_LEFT_ENTRANCE, -player.getPos().y);

        //initial position
        current_room = Rooms::ENTRANCE;
        player.setPosX(maps[current_room]->getTilePosRectById(SpawnTiles::LEFT).x);


        // testing player bounds and camera make them true for a fun experience :D!
        player.testing_bounds = false;
        testing_camera = false;

        music = Mix_LoadMUS( "sound/durante - vergine tutto amore reberb version.ogg" );
        if( music == nullptr ){
            cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << endl;
            cout << "error at file Game.cpp line 65" << endl;
            sdl_ready = false;
            game_is_running = false;
        }

        // $$$ get the previus time before loop begins to apply delta time $$$
        Uint32 last_time = SDL_GetTicks();

//      %%% GAME LOOP %%%
        while(game_is_running){
            if( Mix_PlayingMusic() == 0 ){
                //Play the music
                Mix_PlayMusic( music, -1 );
            }
            // get the counter everytime loop repeats 
            Uint64 start = SDL_GetPerformanceCounter();

//          @@@ EVENTS @@@
            while(SDL_PollEvent(&event) != 0){
                player.handleAnimationEvents(event);
                player.handleEvents(event);
                if(event.type == SDL_QUIT) game_is_running = false;
                else if (event.type == SDL_KEYDOWN){
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            game_is_running = false;
                        break;
                    }
                }
            }

//          ### check rooms I'll fix this in a method later
            switch(current_room){
                case Rooms::ENTRANCE:
                    //fade out
                    if(player.getPos().x + player.getWidth() > WINDOW_WIDTH){
                        fader_alpha += 12;
                        if(fader_alpha > 255 ){
                            current_room = Rooms::ROOM1A;
                            player.setPosX(maps[current_room]->getTilePosRectById(SpawnTiles::LEFT).x);
                            fader_alpha = 255;
                        }

                    }
                    //fade out exit
                    else if(player.getPos().x + player.getWidth()  < 0){
                        fader_alpha += 12;
                        if(fader_alpha > 255){
                            game_is_running = false;
                        }   
                    }
                    else{
                        if(fader_alpha >= 0){
                            fader_alpha -= 12;
                            if(fader_alpha < 0) fader_alpha = 0;
                        }
                    }

                break;

                case Rooms::ROOM1A:
                    if(player.getPos().x + player.getWidth() < 0){
                        fader_alpha += 12;
                        if(fader_alpha > 255){
                            current_room = Rooms::ENTRANCE;
                            player.setPosX(maps[current_room]->getTilePosRectById(SpawnTiles::RIGHT).x);
                            fader_alpha = 255;
                        }

                    }

                    else if(player.getPos().x + player.getWidth() > WINDOW_WIDTH){
                        fader_alpha += 12;
                        if(fader_alpha > 255 ){
                            fader_alpha = 255;
                            game_is_running = false;
                        }
                    }

                    else{
                        if(fader_alpha >= 0){
                            fader_alpha -= 12;
                            if(fader_alpha < 0) fader_alpha = 0;
                            
                        }
                    }
                break;
            }

//          $$$ delta time claculation $$$
            Uint32 current_time = SDL_GetTicks();
            double delta_time = (current_time - last_time) / 1000.0f;
            last_time = current_time;
            
//          ### UPDATE ###
            player.update(delta_time);
            player.handleHorizontalCollision(*maps[current_room]);
            player.applyGravity(delta_time);
            player.handleVerticalCollision(*maps[current_room]);
            camera.follow(player, *maps[current_room], WINDOW_WIDTH, WINDOW_HEIGHT, delta_time);
            player.animate(delta_time);


//          ### RENDER ### 
            SDL_SetRenderDrawColor(renderer, 83,104,120,0);
            SDL_RenderClear(renderer);

            if(current_room == Rooms::ENTRANCE) entrance_background.draw(renderer);
            else interior_background.draw(renderer);
            if(testing_camera){
                SDL_SetRenderDrawColor(renderer, 0,0,255,0);
                SDL_RenderDrawRect(renderer, &camera.camera_rect);
            }
            player.draw(renderer);
            maps[current_room]->draw(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0,0,0,fader_alpha);
            SDL_RenderFillRect(renderer, &fader);
            
            SDL_RenderPresent(renderer);

//          ### DELAY ### this delay is better explained by: pikuma
            // https://youtu.be/XfZ6WrV5Z7Y?si=va2iYYY8DQ8G5bDY&t=4797
            Uint64 end = SDL_GetPerformanceCounter();
            float frameTime = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
            if (frameTime < SCREEN_TICKS_PER_FRAME) {
                SDL_Delay((Uint32)(SCREEN_TICKS_PER_FRAME - frameTime));
            }
        }
    }
}




int Game::init(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "video init error: SDL2_ERROR: " << SDL_GetError() << endl;
        return -99;
    }
    else{
        if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1")) cout << "warning! linear texture filtering is not enabled" << endl; //Set texture filtering to linear

        window = SDL_CreateWindow("SDL2 Platformer OOP: By: Lu",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH, WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
        if (window == NULL){
            cout << "window creation error! SDL2_ERROR: " << SDL_GetError() << endl;
            return -98;
        }

        else{
            //SDL_RENDERER_PRESENTVSYNC
            //SDL_RENDERER_ACCELERATED
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL){
                cout << "renderer creation error! SDL2_ERROR" << SDL_GetError() << endl;
                return -97;
            }

            else {
                //Initialize PNG loading
                int img_flags = IMG_INIT_PNG;
                if(!IMG_Init(img_flags)){
                    cout << "SDL image not initialized! SDL2_ERROR: " << SDL_GetError() << " SDL2 IMAGE ERROR: " << IMG_GetError()  << endl;
                    return -96;
                }

               if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
                    cout << "SDL_mixer could not initialize! SDL_mixer Error: " <<  Mix_GetError() << endl;
                    return -95;
                }
            }
        }
    }
    return 1;
}

// FREEING THE MEMORY
Game::~Game(){
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    Mix_FreeMusic(music);
    music = NULL;      
    window = NULL;
    renderer = NULL;
    cout << "game has ended " << endl;
}
