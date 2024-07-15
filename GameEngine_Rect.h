//Grupp 80
//Adrian Andersson Martvall adan2936
//Ida Laaksonen idla8418
#ifndef Rect_h
#define Rect_h

#include <SDL2/SDL_rect.h>
#include <memory>
#include "GameEngine_Component.h"

namespace Engine {
//Komponent som innehåller en SDL_rect för att representera position på entitys
//samt information om krockar med andra rektanglar
    class Rect : public Component<Rect> {

    public:

        static void New(int x, int y, int width_height, Entity ent);

        const std::unique_ptr<SDL_Rect> &getRect() const;

        bool &isCollided();

        void setCollided(bool collided);

        Entity getCollidedWith() const;

        void setCollidedWith(const Entity &collidedWith);

    private:
        Rect(int x, int y, int width_height, Entity ent);
        std::unique_ptr<SDL_Rect> rect;
        bool collided = false;
        Entity collided_with;
    };
}
#endif