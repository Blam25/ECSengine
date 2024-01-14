
/*
 * Denna spelmotor är baserad på en så kallad entity-component-system (ECS) spelarkitektur
 * där varje entity endast representeras av en siffra (i detta fall dold i en Entity klass)
 * och spellogiken istället definieras i form av olika system (i detta fall funktioner) på typer av komponenter
 * som har koppling till en entity, istället för att koppla all logik direkt till en basklass (såsom Sprite-klass
 * exemplet i uppgiften). Idén med arkitekturen är att det är väldigt lätt att skapa nya typer av objekt som ser
 * helt olika ut utan att låsas i en krånglig klasshierarki.
 *
 * Vi använder oss fortfarande av oop principer såsom inkapsling och arv, men istället applicerat på själva
 * komponenterna - klassen Component är basklass för samtliga komponenter och i den kan man se hur komponenterna
 * fungerar.
 */

#ifndef GameEngine
#define GameEngine

#include "GameEngine_Entity.h"

namespace Engine {

    //Tar bort alla komponenter tillhörande en specifik entity
    void removeEntity(Entity ent);

    //Lägg till system för borttagning av entitys hos egenskapade komponenter
    void addRemoveSystem(void (*function)(Entity ent));

    //Lägg till ett externt system för tickande körning
    void addSystem(void (*func)());

    //Ta bort ett externt system
    void removeSystem(void (*func)());

    //Rensa alla externa system
    void clearSystems();

    //Schemalägg en entity för borttagning
    void scheduleRemoveEntity(Entity ent);

    //Lägg till en timer för att ticka uppåt
    void addTimer(int *timer);

    //Ta bort en specifik timer från att ticka uppåt
    void removeTimer(int *timer);

    //Initiera de variabler som krävs för att SDL ska fungera. Måste köras innan någon image-komponent skapas
    int initGame();

    //Starta spelloopen
    int launchGame(int argc, char *argv[]);

}

#endif