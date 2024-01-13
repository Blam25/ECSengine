//
//  c++_empty
//
#ifndef GameEngine
#include <iostream>
#include <vector>
#include "memory"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <boost/optional/optional.hpp>
#include <set>
#include "Constants.h"
inline SDL_Renderer* renderer;
inline SDL_Window *window;
using namespace std;

#include "Entity.h"
#include "Component.h"
#include "Position.h"
#include "Image.h"
#include "Rect.h"
#include "Keyboard_Event.h"
#include "Keyboard_Listener.h"
#include "Speed.h"
#include "Missile.h"


#define FPS 60

inline vector<Entity> to_be_removed;



inline void removeEntity(Entity ent) {
    Image::Remove(ent);
    Rect::Remove(ent);
    Speed::Remove(ent);
    Missile::Remove(ent);
}

inline Entity createMissile(string imagePath, int x, int y, int xspeed, int yspeed) {
    auto ent = Entity();
    Image::New(imagePath, ent);
    Rect::New(x, y, 10, ent);
    Missile::New(ent);
    Speed::New(xspeed, yspeed, ent);
    return ent;
}

inline int missileTimer = 0;

inline Entity createPlayer( int x, int y) {
    //auto ent = Entity::New();
    auto ent = Entity();
    Position::New(0, 0, ent);
    // Image asd = new Image();
    Image::New("images/bg.jpg", ent);
    Rect::New(x, y, 100, ent);
   // Speed::New(1,1, ent);
    Keyboard_Listener::New(ent, [ent](const Keyboard_Event& keyb_event) {
        //cout << "nice";
        bool create = false;
        Entity added;
        for (auto &rect: Rect::getComps()) {
            //auto rect = Rect::Get(ent);
            // if (!rect.has_value()) return;
            if (rect->getEnt() == ent) {
                switch (keyb_event.key) {
                    case SDLK_RIGHT:
                        //to_be_removed.push_back(rect->ent);
                        rect->getRect()->x += 5;
                        break;
                    case SDLK_LEFT:
                        rect->getRect()->x -= 5;
                        break;
                    case SDLK_UP:
                        rect->getRect()->y -= 5;
                        break;
                    case SDLK_DOWN:
                        rect->getRect()->y += 5;
                        break;
                    case SDLK_SPACE:
                        added = rect->getEnt();
                        create = true;
                    default:
                        break;
                }
            }
        }
        if (missileTimer > 20 && create) {
            auto rect = Rect::Get(added);
            createMissile("images/bg.jpg", rect->get()->getRect()->x+rect->get()->getRect()->w/2, rect->get()->getRect()->y, 0, -15);
            missileTimer = 0;
        }


    });

    return ent;
}


inline Entity createNPC(int x, int y) {
    //auto ent = Entity::New();
    auto ent = Entity();
    Position::New(0, 0, ent);
    Image::New("images/bg.jpg", ent);
    Rect::New(x, y, 100, ent);

    return ent;
}

inline void render(SDL_Renderer *renderer) {
    SDL_RenderClear(renderer);
    for (auto &image: Image::getComps()) {
        auto rect = Rect::Get(image->getEnt());
        if (rect.has_value()) {
            SDL_RenderCopy(renderer, image->getTexture(), nullptr, rect.get()->getRect().get());
        }
    }
    SDL_RenderPresent(renderer);
}

inline void move() {
    for (auto &rect: Rect::getComps()) {
        rect->getRect()->x++;
    }
}

inline void move_with_speed() {
    for ( auto &speed : Speed::getComps()) {
        auto rect = Rect::Get(speed->getEnt());
        if (rect.has_value()) {
            rect->get()->getRect()->x += speed->getXspeed();
            rect->get()->getRect()->y += speed->getYspeed();
        }
    }
}

//Checks if any corner of the first square is inside the other square
inline bool is_colliding(const unique_ptr<SDL_Rect> &rect1, const unique_ptr<SDL_Rect> &rect2) {
    return (rect1->x > rect2->x &&
            rect1->x < rect2->x + rect2->w &&
            rect1->y < rect2->y &&
            rect1->y > rect2->y - rect2->h
           ) || (rect1->x + rect1->w > rect2->x &&
                 rect1->x + rect1->w < rect2->x + rect2->w &&
                 rect1->y < rect2->y &&
                 rect1->y > rect2->y - rect2->h
           ) || (rect1->x + rect1->w > rect2->x &&
                 rect1->x + rect1->w < rect2->x + rect2->w &&
                 rect1->y - rect1->h < rect2->y &&
                 rect1->y - rect1->h > rect2->y - rect2->h
           ) || (rect1->x > rect2->x &&
                 rect1->x < rect2->x + rect2->w &&
                 rect1->y - rect1->h < rect2->y &&
                 rect1->y - rect1->h > rect2->y - rect2->h
           );
}

inline void check_collisions() {
    for (auto &rect: Rect::getComps()) {
        for (auto &other_rect: Rect::getComps()) {
            if (&rect == &other_rect) continue;
            if (is_colliding(rect->getRect(), other_rect->getRect())) {
                rect->isCollided() = true;
                rect->setCollidedWith(other_rect->getEnt());
                other_rect->isCollided() = true;
                other_rect->setCollidedWith(rect->getEnt());
            }
        }
        //cout << std::boolalpha << rect->collided;
    }
}

inline void missile_hits() {
    for (auto &rect: Rect::getComps()) {
        if (Keyboard_Listener::Get(rect->getEnt()).has_value()) continue;
        auto missile = Missile::Get(rect->getCollidedWith());
        if (missile.has_value()) {
            to_be_removed.push_back(rect->getEnt());
        }
    }
}

inline vector<void (*)()> beginPlaySystems; //körs en gång när programmet startas
inline vector<void (*)()> tickSystems; //Körs i main loopen

//#include "GameApp.cpp"

inline int initGame() {
    SDL_Init(SDL_INIT_EVERYTHING);

    if (TTF_Init() < 0) {
        std::cout << "Error SDL_ttf Initialization : " << SDL_GetError();
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return 0;
}

inline int launchGame(int argc, char *argv[]) {

    const int tick_interval = 1000 / FPS;
    Uint32 next_tick;
    int delay;
    for (auto function : beginPlaySystems) {
        function();
    }

    set<Keyboard_Event> keyboard_events = set<Keyboard_Event>();
    bool running = true;
    while (running) {
        missileTimer ++;
        for (auto &rect: Rect::getComps()) { rect->isCollided() = false; }
        next_tick = SDL_GetTicks() + tick_interval;
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    keyboard_events.insert(Keyboard_Event(static_cast<SDL_KeyCode>(e.key.keysym.sym)));
                    break;
//                    switch(e.key.keysym.sym) {
//                        case SDLK_RIGHT
//                    }
                case SDL_KEYUP: {
                    for (set<Keyboard_Event>::iterator i = keyboard_events.begin();
                         i != keyboard_events.end(); i++) {    //for (auto keyb_event : keyboard_events) {
                        if ((*i).key == e.key.keysym.sym) {    //  if (keyb_event.key == e.key.keysym.sym) {
                            keyboard_events.erase(i);
                            // i--;
                        }
                    }
                    break;
                }
            }
        }

        for (auto keyb_event: keyboard_events) {
            for (auto &listener: Keyboard_Listener::listeners) {
                listener->handler(keyb_event);
            }
        }
        //keyboard_events.clear();
        check_collisions();
        move_with_speed();
        missile_hits();
        render(renderer);
        //move();

        for (auto function : tickSystems) {
            function();
        }

//        auto rect_player = Rect::Get(player);
//        if (rect_player.has_value()) {
//       //     cout << std::boolalpha << rect_player.get()->collided;
//        }


        for (auto& rect : Rect::getComps()) {
            if (rect->getRect()->x > 2020 || rect->getRect()-> x < -100 || rect->getRect()->y > 1180 || rect->getRect()->y < -100) {
                to_be_removed.push_back(rect->getEnt());
            }
        }

        for (auto remove : to_be_removed) {
            removeEntity(remove);
        }
        to_be_removed.clear();

        delay = next_tick - SDL_GetTicks();
        if (delay > 0) {
            SDL_Delay(delay);
        }
    }



    // Städa innan programmet avslutas!
    for (auto &image: Image::getComps()) {

        SDL_DestroyTexture(image->getTexture());

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    SDL_Quit();
    return 0;
}

#endif