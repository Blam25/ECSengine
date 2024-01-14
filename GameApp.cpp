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

//Includes tillhörande spelmotorn samt dess komponenter
#include "GameEngine.h"
#include "GameEngine_Image.h"
#include "GameEngine_Rect.h"
#include "GameEngine_KeybEvent.h"
#include "GameEngine_KeybListener.h"
#include "GameEngine_Speed.h"
#include "GameEngine_NPC.h"

//Includes av komponenter tillhörande spelet
#include "GameApp_Missile.h"
#include "GameApp_PowerUp.h"

//Skapa en enkel fiendekaraktär
Engine::Entity createNPC(int x, int y, int xspeed, int yspeed);

//Skapa en missil
inline Engine::Entity createMissile(int x, int y, int xspeed, int yspeed);

//Skapa en power-up
inline Engine::Entity createPowerUp(int x, int y, int xspeed, int yspeed);

//Skapa spelar-entityn med tillhörande listener för tangentbord-events
Engine::Entity createPlayer( int x, int y);

//Flytta alla enheter som har en fart-komponent
void move_with_speed();

//Ta bort alla enheter förutom spelaren som krockat med en missil
void missile_hits();

//Ta bort spelaren om den har krockat med en fiende
void player_hit();

//Ge spelaren en powerup om den krockar med en powerup-entity
void givePowerUp();

//Körs under spelnivån 1
void NPCLevelOne();

//Körs under spelnivån 2
void NPCLevelTwo();

//Körs under spelnivån 3
void NPCLevelThree();

//Körs under spelnivån 4
void NPCLevelFour();

//Körs under spelnivån 5
void NPCLevelFive();

//Startar level 1
void startLevelOne();

//Tar bort level 1 och startar level 2 samt spawnar en powerup
void startLevelTwo();

//Tar bort level 2 och startar level 3 samt spawnar en powerup
void startLevelThree();

//Tar bort level 3 och startar level 4 samt spawnar en powerup
void startLevelFour();

//Tar bort level 3 och startar level 5 samt spawnar en powerup
void startLevelFive();

//Funktion för borttag av entitys hos de egenskapade komponenterna
void removeEntitys(Engine::Entity ent);

//Referens till spelare-entityn för enkel referering (Entity-klassen innehåller endast en int, utan
//någon typ av arv, därav tillåter vi värdesemantik på den klassen)
Engine::Entity player;

//olika timers som ska skickas till spelmotorn
int levelTimer = 0;
int spawnTimer = 0;
int missileTimer = 0;

int main(int argc, char *argv[]) {

    //Initera nödvändiga SDL-variabler
    Engine::initGame();

    //Lägg till timers för tickning
    Engine::addTimer(&missileTimer);
    Engine::addTimer(&spawnTimer);
    Engine::addTimer(&levelTimer);

    //Lägg till system för tickning
    Engine::addSystem(move_with_speed);
    Engine::addSystem(missile_hits);
    Engine::addSystem(player_hit);
    Engine::addSystem(givePowerUp);
    Engine::addSystem(startLevelOne);
    Engine::addSystem(startLevelTwo);
    Engine::addSystem(startLevelThree);
    Engine::addSystem(startLevelFour);
    Engine::addSystem(startLevelFive);

    //Lägg till borttag av entitys för de egenskapade komponenterna
    Engine::addRemoveSystem(removeEntitys);

    //Skapa själva spelaren
    player = createPlayer(400, 400);

    //Starta spelloopen
    Engine::launchGame(argc, argv);

    return 0;
}

//Skapa en enkel fiendekaraktär
Engine::Entity createNPC(int x, int y, int xspeed, int yspeed) {
    Engine::Entity ent = Engine::Entity();
    Engine::Image::New("images/fiende1-01.jpg", ent);
    Engine::Rect::New(x, y, 50, ent);
    Engine::Speed::New(xspeed, yspeed, ent);
    Engine::NPC::New(ent);
    return ent;
}

//Skapa en missil
inline Engine::Entity createMissile(int x, int y, int xspeed, int yspeed) {
    Engine::Entity ent = Engine::Entity();
    Engine::Image::New("images/missile1.jpg", ent);
    Engine::Rect::New(x, y, 7, ent);
    Missile::New(ent);
    Engine::Speed::New(xspeed, yspeed, ent);
    return ent;
}

//Skapa en entity för att ge spelaren en "power-up"
inline Engine::Entity createPowerUp(int x, int y, int xspeed, int yspeed) {
    Engine::Entity ent = Engine::Entity();
    Engine::Image::New("images/missile1.jpg", ent);
    Engine::Rect::New(x, y, 12, ent);
    PowerUp::New(ent);
    Engine::Speed::New(xspeed, yspeed, ent);
    return ent;
}

//Skapa spelar-entityn med tillhörande listener för tangentbord-events
Engine::Entity createPlayer( int x, int y) {
    Engine::Entity ent = Engine::Entity();
    Engine::Image::New("images/rymdskepp1-01.jpg", ent);
    Engine::Rect::New(x, y, 75, ent);
    Engine::Keyboard_Listener::New(ent, [ent](const Engine::Keyboard_Event& keyb_event) {
        auto rect = Engine::Rect::Get(ent);
        if (!Engine::Rect::Get(ent).has_value()) return;
        switch (keyb_event.key) {
            case SDLK_RIGHT:
                //to_be_removed.push_back(rect->ent);
                rect.get()->getRect()->x += 5;
                break;
            case SDLK_LEFT:
                rect.get()->getRect()->x -= 5;
                break;
            case SDLK_UP:
                rect.get()->getRect()->y -= 5;
                break;
            case SDLK_DOWN:
                rect.get()->getRect()->y += 5;
                break;
            case SDLK_SPACE:
                if (missileTimer > 25 && PowerUp::Get(ent).has_value()) {
                    createMissile(rect.get()->getRect()->x+rect.get()->getRect()->w/2, rect.get()->getRect()->y, 0, -15);
                    createMissile(rect.get()->getRect()->x+rect.get()->getRect()->w/2 - 20, rect.get()->getRect()->y, -3, -15);
                    createMissile(rect.get()->getRect()->x+rect.get()->getRect()->w/2 + 20, rect.get()->getRect()->y, 3, -15);
                    missileTimer = 0;
                }
                else if (missileTimer > 25) {
                    createMissile(rect.get()->getRect()->x+rect.get()->getRect()->w/2, rect.get()->getRect()->y, 0, -15);
                    missileTimer = 0;
                }
                break;
            default:
                break;
        }
    });
    return ent;
}

//Flytta alla enheter som har en fart-komponent
void move_with_speed() {
    for ( auto &speed : Engine::Speed::getComps()) {
        auto rect = Engine::Rect::Get(speed->getEnt());
        if (rect.has_value()) {
            rect.get()->getRect()->x += speed->getXspeed();
            rect.get()->getRect()->y += speed->getYspeed();
        }
    }
}

//Ta bort alla enheter förutom spelaren som krockat med en missil
void missile_hits() {
    for (auto &rect: Engine::Rect::getComps()) {
//        if (Keyboard_Listener::Get(rect->getEnt()).has_value()) continue;
        if (rect->getEnt() == player) continue;
        if (PowerUp::Get(rect->getEnt()).has_value()) continue;
        auto missile = Missile::Get(rect->getCollidedWith());
        if (missile.has_value()) {
            Engine::scheduleRemoveEntity(rect->getEnt());
        }
    }
}

//Ta bort spelaren om den har krockat med en fiende
void player_hit() {
        auto rect = Engine::Rect::Get(player);
        if (rect.has_value()) {
            auto other_rect = rect.get()->getCollidedWith();
            if (rect.get()->isCollided() && !Missile::Get(other_rect).has_value() && !PowerUp::Get(other_rect).has_value()) {
                Engine::scheduleRemoveEntity(player);
            }
        }
}

//Ge spelaren en powerup om den krockar med en powerup-entity
void givePowerUp() {
    auto rect = Engine::Rect::Get(player);
    if (rect.has_value() && PowerUp::Get(rect.get()->getCollidedWith()).has_value()) {
        PowerUp::New(player);
        Engine::scheduleRemoveEntity(rect.get()->getCollidedWith());
    }
}

//Körs under spelnivån 1
void NPCLevelOne() {
    if (spawnTimer > 20) {
        createNPC(rand() % 800, 0, (rand() % 6)-3,3);
        spawnTimer = 0;
    }
}

//Körs under spelnivån 2
void NPCLevelTwo() {
    if (spawnTimer > 17) {
        createNPC(rand() % 800, 0,(rand() % 8)-4,4);
        spawnTimer = 0;
    }
}

//Körs under spelnivån 3
void NPCLevelThree() {
    if (spawnTimer > 14) {
        createNPC(rand() % 800, 0,(rand() % 10)-5,5);
        spawnTimer = 0;
    }
}

//Körs under spelnivån 4
void NPCLevelFour() {
    if (spawnTimer > 11) {
        createNPC(rand() % 800, 0,(rand() % 10)-5,6);
        spawnTimer = 0;
    }
}

//Körs under spelnivån 5
void NPCLevelFive() {
    if (spawnTimer > 7) {
        createNPC(rand() % 800, 0,(rand() % 10)-5,8);
        spawnTimer = 0;
    }
}


//Startar level 1
void startLevelOne() {
    Engine::addSystem(NPCLevelOne);
    Engine::removeSystem(startLevelOne);
}

//Tar bort level 1 och startar level 2 samt spawnar en powerup
void startLevelTwo() {
    if (levelTimer > 1000) {
        Engine::removeSystem(NPCLevelOne);
        createPowerUp(400, 0, 0, 2);
        Engine::addSystem(NPCLevelTwo);
        Engine::removeSystem(startLevelTwo);
    }
}

//Tar bort level 2 och startar level 3 samt spawnar en powerup
void startLevelThree() {
    if (levelTimer > 2000) {
        Engine::removeSystem(NPCLevelTwo);
        createPowerUp(400, 0, 0, 2);
        Engine::addSystem(NPCLevelThree);
        Engine::removeSystem(startLevelThree);
    }
}

//Tar bort level 3 och startar level 4 samt spawnar en powerup
void startLevelFour() {
    if (levelTimer > 3000) {
        Engine::removeSystem(NPCLevelThree);
        createPowerUp(400, 0, 0, 2);
        Engine::addSystem(NPCLevelFour);
        Engine::removeSystem(startLevelFour);
    }
}

//Tar bort level 3 och startar level 5 samt spawnar en powerup
void startLevelFive() {
    if (levelTimer > 4000) {
        Engine::removeSystem(NPCLevelFour);
        createPowerUp(400, 0, 0, 2);
        Engine::addSystem(NPCLevelFive);
        Engine::removeSystem(startLevelFive);
    }
}

//Funktion för borttag av entitys hos de egenskapade komponenterna
void removeEntitys(Engine::Entity ent) {
    Missile::Remove(ent);
    PowerUp::Remove(ent);
}