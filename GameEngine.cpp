
/*
 * Denna spelmotor är baserad på en så kallad entity-component-system (ECS) spelarkitektur
 * där varje entity endast representeras av en siffra (i detta fall dold i en Entity klass)
 * och spellogiken istället definieras på typer av komponenter som har koppling till en entity,
 * istället för att koppla all logik direkt till en basklass (såsom Sprite klass exemplet i uppgiften).
 * Idén med arkitekturen är att det är väldigt lätt att skapa nya typer av objekt som ser helt olika ut,
 * och sedan definiera "system" på dessa (funktioner i detta fall) utan att låsas i en krånglig klasshierarki.
 *
 * Vi använder oss fortfarande av oop principer såsom inkapsling och arv, men istället applicerat på själva
 * komponenterna - klassen Component är basklass för samtliga komponenter och i den går det enklast att se
 * hur komponenterna fungerar.
 */

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
//#include "Constants.h"

#include "GameEngine_Entity.h"
//#include "Component.h"
#include "GameEngine_Image.h"
#include "GameEngine_Rect.h"
#include "GameEngine_KeybEvent.h"
#include "GameEngine_KeybListener.h"
#include "GameEngine_Speed.h"
//#include "GameApp_Missile.h"
#include "GameEngine_NPC.h"
//#include "GameApp_PowerUp.h"

using namespace std;

namespace Engine {

#define FPS 60

    vector<void (*)(Entity ent)> removeEntitysFromSystem;

    void addRemoveSystem(void (*function)(Entity ent)) {
        removeEntitysFromSystem.push_back(function);
    }

    //Tar bort samtliga Komponenter tillhörande en specifik entity
    void removeEntity(Entity ent) {
        Image::Remove(ent);
        Rect::Remove(ent);
        Speed::Remove(ent);
        //Missile::Remove(ent);
        NPC::Remove(ent);
        //PowerUp::Remove(ent);
        for (auto function : removeEntitysFromSystem) {
            function(ent);
        }
    }

//Renderar samtliga tillagda bilder i programmet om de även har en tillhörande rektangel
    void render(SDL_Renderer *renderer) {
        SDL_RenderClear(renderer);
        for (auto &image: Image::getComps()) {
            auto rect = Rect::Get(image->getEnt());
            if (rect.has_value()) {
                SDL_RenderCopy(renderer, image->getTexture(), nullptr, rect.get()->getRect().get());
            }
        }
        SDL_RenderPresent(renderer);
    }

//Undersöker kollision mellan två rektanglar
    bool is_colliding(const unique_ptr<SDL_Rect> &rect1, const unique_ptr<SDL_Rect> &rect2) {
        return SDL_HasIntersection(rect1.get(), rect2.get());
    }

//Undersöker kollisioner och sparar information om dem
    void check_collisions() {
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
        }
    }

    vector<void (*)()> tickSystems; //Körs i spelloopen
    vector<int *> timers; //Tickar uppåt i spelloopen
    vector<Entity> to_be_removed; //Samling av enheter som ska tas bort vid slutet av loopen

//Lägg till ett externt system för tickande körning
    void addSystem(void (*func)()) {
        tickSystems.push_back(func);
    }

//Ta bort ett externt system
    void removeSystem(void (*func)()) {
        int indexToBeRemoved = -1;
        for (int i = 0; i != tickSystems.size(); i++) {
            if (tickSystems[i] == func) {
                indexToBeRemoved = i;
            }
        }
        if (indexToBeRemoved > -1) tickSystems.erase(tickSystems.begin() + indexToBeRemoved);
    }

//Rensa alla externa system
    void clearSystems() {
        tickSystems.clear();
    }

    void scheduleRemoveEntity(Entity ent) {
        to_be_removed.push_back(ent);
    }

//Lägg till en timer för att ticka uppåt
    void addTimer(int *timer) {
        timers.push_back(timer);
    }

//Ta bort en specifik timer från att ticka uppåt
    void removeTimer(int *timer) {
        int indexToBeRemoved = -1;
        for (int i = 0; i != timers.size(); i++) {
            if (timers[i] == timer) {
                indexToBeRemoved = i;
            }
        }
        if (indexToBeRemoved > -1) timers.erase(timers.begin() + indexToBeRemoved);
    }

    SDL_Renderer* renderer;
    SDL_Window *window;

//Initiera de variabler som krävs för att SDL ska fungera. Måste köras innan någon image komponent skapas
    int initGame() {
        SDL_Init(SDL_INIT_EVERYTHING);
        if (TTF_Init() < 0) {
            std::cout << "Error SDL_ttf Initialization : " << SDL_GetError();
            return EXIT_FAILURE;
        }
        window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, 0);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        Image::setRenderer(renderer);
        return 0;
    }

    int launchGame(int argc, char *argv[]) {

        //Initiera variabler för att hålla tickintervallet till 60 fps
        const int tick_interval = 1000 / FPS;
        Uint32 next_tick;
        int delay;

        //Set med knappar som är neddtryckta just nu
        set<Keyboard_Event> keyboard_events = set<Keyboard_Event>();

        bool running = true;

        //Spelloop
        while (running) {

            //Räkna ut när nästa tick ska ske
            next_tick = SDL_GetTicks() + tick_interval;

            //Räkna upp alla timers från spelet en gång
            for (auto timer: timers) {
                ++(*timer);
            }

            //Rensa alla kollisioner från förra ticken
            for (auto &rect: Rect::getComps()) {
                rect->isCollided() = false;
            }

            //Gå igenom alla events från SDL som vi är intresserade av
            SDL_Event e;
            if (SDL_PollEvent(&e)) {
                switch (e.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                    case SDL_KEYDOWN: //Lägg till ett nytt keyboard event i listan över sådana
                        keyboard_events.insert(Keyboard_Event(static_cast<SDL_KeyCode>(e.key.keysym.sym)));
                        break;
                    case SDL_KEYUP: //Ta bort det keyboard event som matchar nyckelkoden från listan
                        for (set<Keyboard_Event>::iterator i = keyboard_events.begin();
                             i != keyboard_events.end(); i++) {
                            if ((*i).key == e.key.keysym.sym) {
                                keyboard_events.erase(i);
                            }
                        }
                        break;
                }
            }

            //Gå igenom alla tangentbords-events och skicka de till alla tangentbords-listeners
            for (auto keyb_event: keyboard_events) {
                for (auto &listener: Keyboard_Listener::listeners) {
                    listener->handler(keyb_event);
                }
            }

            //Exekvera det inbyggda kollisionssystemet
            check_collisions();

            //exekvera alla tick-system som läggs till av spelet
            for (auto function: tickSystems) {
                function();
            }

            //Exekvera det inbyggda renderingssystemet
            render(renderer);

            //lägg till alla entitys utanför skärmen för borttagning
            for (auto &rect: Rect::getComps()) {
                if (rect->getRect()->x > 2020 || rect->getRect()->x < -100 || rect->getRect()->y > 1180 ||
                    rect->getRect()->y < -100) {
                    to_be_removed.push_back(rect->getEnt());
                }
            }

            //ta bort alla enheter som är tillagda för borttagning och därefter rensa den listan
            for (auto remove: to_be_removed) {
                removeEntity(remove);
            }
            to_be_removed.clear();

            //vänta så att nästa tick kommer enligt 60 fps
            delay = next_tick - SDL_GetTicks();
            if (delay > 0) {
                SDL_Delay(delay);
            }
        }



        // Städa innan programmet avslutas
        for (auto &image: Image::getComps()) {

            SDL_DestroyTexture(image->getTexture());

        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);


        SDL_Quit();
        return 0;
    }
}

#endif