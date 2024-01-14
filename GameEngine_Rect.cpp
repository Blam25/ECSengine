//Grupp 80
//Adrian Andersson Martvall adan2936
//Ida Laaksonen idla8418
#include "GameEngine_Rect.h"

namespace Engine {
//Komponent som innehåller en SDL_rect för att representera position på entitys
//samt information om krockar med andra rektanglar

    void Rect::New(int x, int y, int width_height, Entity ent) {
        getComps().push_back(std::unique_ptr<Rect>(new Rect(x, y, width_height, ent)));
    }

    Rect::Rect(int x, int y, int width_height, Entity ent) : Component(ent) {
        rect = std::make_unique<SDL_Rect>();
        rect->x = x;
        rect->y = y;
        rect->h = width_height;
        rect->w = width_height;
    }

    const std::unique_ptr<SDL_Rect> &Rect::getRect() const {
        return rect;
    }

    bool &Rect::isCollided() {
        return collided;
    }

    void Rect::setCollided(bool collided) {
        Rect::collided = collided;
    }

    Entity Rect::getCollidedWith() const {
        return collided_with;
    }

    void Rect::setCollidedWith(const Entity &collidedWith) {
        collided_with = collidedWith;
    }
}
