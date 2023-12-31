
//#include <vector>
//#include "memory"
//#include "Entity.h"
//#include "Constants.h"
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>
//#include <SDL2/SDL_ttf.h>
//#include <boost/optional.hpp>


class Rect {

public:
    inline static vector<unique_ptr<Rect>> comps;
    Entity ent;
    unique_ptr<SDL_Rect> rect;
    unique_ptr<SDL_Rect> rect_up;
    unique_ptr<SDL_Rect> rect_right;
    unique_ptr<SDL_Rect> rect_left;
    unique_ptr<SDL_Rect> rect_down;
    bool collided = false;

    Rect(int x, int y, int width_height, Entity ent) : ent(ent) {
        rect = make_unique<SDL_Rect>();
        rect->x = x;
        rect->y = y;
        rect->h = width_height;
        rect->w = width_height;
    }

    static void New(int x, int y, int width_height, Entity ent) {
        comps.push_back(make_unique<Rect>(x, y, width_height, ent));
    }

    static boost::optional<unique_ptr<Rect>&> Get(Entity ent) {
        for (auto &rect : comps) {
            if (ent == rect->ent) {
                return rect;
            }
        }
        return boost::none;
    }

};
