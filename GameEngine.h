//
// Created by Martvall on 13/01/2024.
//

#ifndef TEST1_COPY_GAMEENGINE_H
#define TEST1_COPY_GAMEENGINE_H


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

#include "Entity.h"
#include "Component.h"
#include "Image.h"
#include "Rect.h"
#include "Keyboard_Event.h"
#include "Keyboard_Listener.h"
#include "Speed.h"
#include "Missile.h"
#include "NPC.h"
#include "PowerUp.h"

namespace Engine {

#define FPS 60

//Tar bort samtliga Komponenter tillhörande en specifik entity
    inline void removeEntity(Entity ent) {
        Image::Remove(ent);
        Rect::Remove(ent);
        Speed::Remove(ent);
        Missile::Remove(ent);
        NPC::Remove(ent);
        PowerUp::Remove(ent);
    }

//Undersöker kollisioner och sparar information om dem
    inline void check_collisions();

//Lägg till ett externt system för tickande körning
    inline void addSystem(void (*func)());

//Ta bort ett externt system
    inline void removeSystem(void (*func)());

//Rensa alla externa system
    inline void clearSystems();

//Lägg till en timer för att ticka uppåt
    inline void addTimer(int *timer);

//Ta bort en specifik timer från att ticka uppåt
    inline void removeTimer(int *timer);

//Initiera de variabler som krävs för att SDL ska fungera. Måste köras innan någon image komponent skapas
    inline int initGame();

    inline int launchGame(int argc, char *argv[]);

}
#endif

#endif //TEST1_COPY_GAMEENGINE_H
