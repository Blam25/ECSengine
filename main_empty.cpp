//
//  c++_empty
//  
#include <iostream>
#include <vector>
#include "memory"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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


#define FPS 60

void removeEntity(Entity ent) {
    Image::Remove(ent);
    Rect::Remove(ent);
}

Entity createPlayer(SDL_Renderer *renderer, int x, int y) {
    //auto ent = Entity::New();
    auto ent = Entity();
    Position::New(0, 0, ent);
    // Image asd = new Image();
    Image::New("images/bg.jpg", renderer, ent);
    Rect::New(x, y, 100, ent);
    Keyboard_Listener::New(ent, [ent](const Keyboard_Event& keyb_event) {
        //cout << "nice";
        for (auto &rect: Rect::comps) {
            //auto rect = Rect::Get(ent);
            // if (!rect.has_value()) return;
            if (rect->ent == ent) {
                switch (keyb_event.key) {
                    case SDLK_RIGHT:
                        removeEntity(rect->ent);
                        rect->rect->x += 5;
                        break;
                    case SDLK_LEFT:
                        rect->rect->x -= 5;
                        break;
                    case SDLK_UP:
                        rect->rect->y -= 5;
                        break;
                    case SDLK_DOWN:
                        rect->rect->y += 5;
                        break;
                    default:
                        break;
                }
            }
        }
    });

    return ent;
}

Entity createNPC(SDL_Renderer *renderer, int x, int y) {
    //auto ent = Entity::New();
    auto ent = Entity();
    Position::New(0, 0, ent);
    Image::New("images/bg.jpg", renderer, ent);
    Rect::New(x, y, 100, ent);

    return ent;
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
                other_rect->collided = true;
            }
        }
        //cout << std::boolalpha << rect->collided;
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

    SDL_Window *window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    auto player = createPlayer(renderer, 400, 400);
    createNPC(renderer, 200, 200);
    createNPC(renderer, 300, 100);

    set<Keyboard_Event> keyboard_events = set<Keyboard_Event>();
    bool running = true;
    while (running) {
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
        render(renderer);
        //move();

        auto rect_player = Rect::Get(player);
        if (rect_player.has_value()) {
       //     cout << std::boolalpha << rect_player.get()->collided;
        }

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

