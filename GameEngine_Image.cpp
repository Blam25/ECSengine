//Grupp 80
//Adrian Andersson Martvall adan2936
//Ida Laaksonen idla8418
#include "Constants.h"
#include "GameEngine_Image.h"

namespace Engine {

    Image::Image(std::string image_path, SDL_Renderer *_renderer, Entity ent) : Component(ent) {
        SDL_Surface *surface = IMG_Load((constants::gResPath + image_path).c_str());
        texture = SDL_CreateTextureFromSurface(_renderer, surface);
    }

    SDL_Texture *Image::getTexture() const {
        return texture;
    }

    void Image::setTexture(SDL_Texture *_texture) {
        this->texture = _texture;
    }
    SDL_Renderer* Image::renderer = 0;
}