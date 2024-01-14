

#include <SDL2/SDL_image.h>
#include "GameEngine_Component.h"
#include <string>

#ifndef Image_h
#define Image_h

namespace Engine {
//Komponent som innehåller en bild för att renderas i spelfönstret
    class Image : public Component<Image> {

    public:
        SDL_Texture *getTexture() const;

        void setTexture(SDL_Texture *texture);

        static void setRenderer(SDL_Renderer* _renderer) {
            renderer = _renderer;
        }

        static void New(std::string image_path, Entity ent) {
            getComps().push_back(std::unique_ptr<Image>(new Image(image_path, renderer, ent)));
        }

        ~Image() override {
            SDL_DestroyTexture(texture);
        }

    private:
        Image(std::string image_path, SDL_Renderer *renderer, Entity ent);
        static SDL_Renderer* renderer;
        SDL_Texture *texture{};
    };
}

#endif