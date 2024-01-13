#ifndef Rect
#include <SDL2/SDL_rect.h>
#include <memory>
namespace Engine {
//Komponent som innehåller en SDL_rect för att representera position på entitys
//samt information om krockar med andra rektanglar
    class Rect : public Component<Rect> {

    public:

        static void New(int x, int y, int width_height, Entity ent);

        inline const std::unique_ptr<SDL_Rect> &getRect() const;

        inline void setRect(const std::unique_ptr<SDL_Rect> &rect);

        inline bool &isCollided();

        inline void setCollided(bool collided);

        inline Entity getCollidedWith() const;

        inline void setCollidedWith(const Entity &collidedWith);
//    ~Rect() override {
//        SDL_dest
//    }
    private:
        inline Rect(int x, int y, int width_height, Entity ent);

        std::unique_ptr<SDL_Rect> rect;
        bool collided = false;
        Entity collided_with;

    };

    inline void Rect::New(int x, int y, int width_height, Entity ent) {
        getComps().push_back(std::unique_ptr<Rect>(new Rect(x, y, width_height, ent)));
    }

    inline Rect::Rect(int x, int y, int width_height, Entity ent) : Component(ent) {
        rect = std::make_unique<SDL_Rect>();
        rect->x = x;
        rect->y = y;
        rect->h = width_height;
        rect->w = width_height;
    }

    inline const std::unique_ptr<SDL_Rect> &Rect::getRect() const {
        return rect;
    }

    inline bool &Rect::isCollided() {
        return collided;
    }

    inline void Rect::setCollided(bool collided) {
        Rect::collided = collided;
    }

    inline Entity Rect::getCollidedWith() const {
        return collided_with;
    }

    inline void Rect::setCollidedWith(const Entity &collidedWith) {
        collided_with = collidedWith;
    }
}
#endif