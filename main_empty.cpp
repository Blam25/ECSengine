//
//  c++_empty
//  
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
#include "NPC.h"


#define FPS 60

vector<Entity> to_be_removed;

SDL_Renderer * renderer;

void removeEntity(Entity ent) {
    Image::Remove(ent);
    Rect::Remove(ent);
    Speed::Remove(ent);
    Missile::Remove(ent);
    NPC::Remove(ent);
}

Entity createMissile(string imagePath, int x, int y, int xspeed, int yspeed) {
    auto ent = Entity();
    Image::New(imagePath, renderer, ent);
    Rect::New(x, y, 10, ent);
    Missile::New(ent);
    Speed::New(xspeed, yspeed, ent);
    return ent;
}

int missileTimer = 0;

Entity createPlayer(SDL_Renderer *renderer, int x, int y) {
    //auto ent = Entity::New();
    auto ent = Entity();
    Position::New(0, 0, ent);
    // Image asd = new Image();
    Image::New("images/rymdskepp1-01.jpg", renderer, ent);
    Rect::New(x, y, 100, ent);
   // Speed::New(1,1, ent);
    Keyboard_Listener::New(ent, [ent](const Keyboard_Event& keyb_event) {
        //cout << "nice";
        bool create = false;
        Entity added;
        for (auto &rect: Rect::comps) {
            //auto rect = Rect::Get(ent);
            // if (!rect.has_value()) return;
            if (rect->ent == ent) {
                switch (keyb_event.key) {
                    case SDLK_d:
                        //to_be_removed.push_back(rect->ent);
                        rect->rect->x += 5;
                        break;
                    case SDLK_a:
                        rect->rect->x -= 5;
                        break;
                    case SDLK_w:
                        rect->rect->y -= 5;
                        break;
                    case SDLK_s:
                        rect->rect->y += 5;
                        break;
                    case SDLK_SPACE:
                        added = rect->ent;
                        create = true;
                    default:
                        break;
                }
            }
        }
        if (missileTimer > 20 && create) {
            auto rect = Rect::Get(added);
            createMissile("images/missile1.jpg", rect->get()->rect->x+rect->get()->rect->w/2, rect->get()->rect->y, 0, -15);
            missileTimer = 0;
        }


    });

    return ent;
}

Entity createNPC(SDL_Renderer *renderer, int x, int y) {
    //auto ent = Entity::New();
    auto ent = Entity();
    Position::New(0, 0, ent);
    Image::New("images/fiende1-01.jpg", renderer, ent);
    Rect::New(x, y, 50, ent);
    Speed::New(0, 3, ent);
    NPC::New(ent);

    return ent;
}

Entity createNPC2(SDL_Renderer *renderer, int x, int y) {
    //auto ent = Entity::New();
    auto ent = Entity();
    Position::New(0, 0, ent);
    Image::New("images/fiende2-01.jpg", renderer, ent);
    Rect::New(x, y, 70, ent);
    Speed::New(0, 5, ent);
    NPC::New(ent);

    return ent;
}



int spawnTimer = 0;

void NPC_movement2() {
    if (spawnTimer > 30) {
        createNPC(renderer, rand() % 1000, 0);
        spawnTimer = 0;
    }
/*
    for (auto &npc: NPC::comps) {
        auto rect = Rect::Get(npc->ent);
        auto speed = Speed::Get(npc->ent);
        if (rect.has_value() && speed.has_value()) {
            speed.get()->xspeed = 2;
            speed.get()->yspeed = 2;
        }
    }*/
}




int spawnTimer2 = 0;
int turn = 0;
void NPC_movement3() {

        if (spawnTimer2 > 40) {
        createNPC2(renderer, rand() % 1000, 0);
        spawnTimer2 = 0;
    }
/*
    if (spawnTimer2 > 20) {
    Entity ent = createNPC2(renderer, rand() % 1000, 0);
        for (auto &npc: NPC::comps) 
        auto rect = Rect::Get(npc->ent);
        Speed::Get(npc->ent);
            if(turn == 0){
            speed.get()->xspeed = -2;
            turn = 1;
        }
            if(turn == 1){
            speed.get()->xspeed = 2;
            turn = 0;
        }
        }
}
        spawnTimer2 = 0;
    }
    */
    
}

void render(SDL_Renderer *renderer) {
    SDL_RenderClear(renderer);
    for (auto &image: Image::comps) {
        auto rect = Rect::Get(image->ent);
        if (rect.has_value()) {
            SDL_RenderCopy(renderer, image->texture, nullptr, rect.get()->rect.get());
        }
    }
    SDL_RenderPresent(renderer);
}

void move() {
    for (auto &rect: Rect::comps) {
        rect->rect->x++;
    }
}

void move_with_speed() {
    for ( auto &speed : Speed::comps) {
        auto rect = Rect::Get(speed->ent);
        if (rect.has_value()) {
            rect->get()->rect->x += speed->xspeed;
            rect->get()->rect->y += speed->yspeed;
        }
    }
}

//Checks if any corner of the first square is inside the other square
bool is_colliding(const unique_ptr<SDL_Rect> &rect1, const unique_ptr<SDL_Rect> &rect2) {
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

void check_collisions() {
    for (auto &rect: Rect::comps) {
//        rect->rect_down->x = rect->rect->x;
//        rect->rect_down->y = rect->rect->y;
//        rect->rect_down->w = rect->rect->w;
//        rect->rect_down->h = -5;
//        rect->rect_up->x = rect->rect->x;
//        rect->rect_up->y = rect->rect->y - rect->rect->h;
//        rect->rect_up->w = rect->rect->w;
//        rect->rect_up->h = 5;
        for (auto &other_rect: Rect::comps) {
            if (&rect == &other_rect) continue;
            if (is_colliding(rect->rect, other_rect->rect)) {
                rect->collided = true;
                rect->collided_with = other_rect->ent;
                other_rect->collided = true;
                other_rect->collided_with = rect->ent;
            }
        }
        //cout << std::boolalpha << rect->collided;
    }
}

int killCounter;
//SDL_Surface* textgrej1 = SDL_

void missile_hits() {
    for (auto &rect: Rect::comps) {
        if (Keyboard_Listener::Get(rect->ent).has_value()) continue;
        auto missile = Missile::Get(rect->collided_with);
        if (missile.has_value()) {
            to_be_removed.push_back(rect->ent);
            killCounter++;
            cout << killCounter << endl;
        }
    }
}

void player_hit() {
    for (auto& player : Keyboard_Listener::listeners) {
        auto rect = Rect::Get(player->ent);
        if (rect.has_value()) {
            if (rect.get()->collided && !Missile::Get(rect.get()->collided_with).has_value()) {
                to_be_removed.push_back(player->ent);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    std::string s1 = "Hejsan";
    std::cout << s1 << std::endl;

    const int tick_interval = 1000 / FPS;
    Uint32 next_tick;
    int delay;

    SDL_Init(SDL_INIT_EVERYTHING);

    if (TTF_Init() < 0) {
        std::cout << "Error SDL_ttf Initialization : " << SDL_GetError();
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    auto player = createPlayer(renderer, 400, 400);
    createNPC(renderer, 200, 200);
    createNPC(renderer, 300, 100);

    set<Keyboard_Event> keyboard_events = set<Keyboard_Event>();
    bool running = true;
    while (running) {
        missileTimer ++;
        spawnTimer ++;
        spawnTimer2 ++;
        for (auto &rect: Rect::comps) { rect->collided = false; }
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
        NPC_movement2();
        NPC_movement3();
        player_hit();


        render(renderer);
        //move();

        auto rect_player = Rect::Get(player);
        if (rect_player.has_value()) {
       //     cout << std::boolalpha << rect_player.get()->collided;
        }


        for (auto& rect : Rect::comps) {
            if (rect->rect->x > 2020 || rect->rect-> x < -100 || rect->rect->y > 1180 || rect->rect->y < -100) {
                to_be_removed.push_back(rect->ent);
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
    for (auto &image: Image::comps) {

        SDL_DestroyTexture(image->texture);

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    SDL_Quit();
    return 0;
}

