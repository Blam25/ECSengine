
#include <vector>
#include "memory"
#include "Entity.h"
#include "Constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <boost/optional.hpp>

using namespace std;

class Image {

public:
    inline static vector<unique_ptr<Image>> comps;
    Entity ent;
    SDL_Texture* texture;

    Image(string image_path, SDL_Renderer* renderer, Entity ent) : ent(ent) {
        SDL_Surface* surface = IMG_Load( (constants::gResPath + image_path).c_str() );
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        //comps.push_back(make_unique<Position>(*this));
        //positions2.push_back(this);

    }

    static void New(string image_path, SDL_Renderer* renderer, Entity ent) {
        comps.push_back(make_unique<Image>(image_path, renderer, ent));
    }

    static boost::optional<unique_ptr<Image>&> Get(Entity ent) {
        for (auto &image : comps) {
            if (ent == image->ent) {
                return image;
            }
        }
        return boost::none;
    }

};