#ifndef Image


namespace Engine {
//Komponent som innehåller en bild för att renderas i spelfönstret
    class Image : public Component<Image> {

    public:
        inline SDL_Texture *getTexture() const;

        inline void setTexture(SDL_Texture *texture);

        static void New(string image_path, Entity ent) {
            getComps().push_back(std::unique_ptr<Image>(new Image(image_path, renderer, ent)));
        }

        ~Image() {
            SDL_DestroyTexture(texture);
        }

    private:
        inline Image(string image_path, SDL_Renderer *renderer, Entity ent);

        SDL_Texture *texture;
    };

    inline Image::Image(string image_path, SDL_Renderer *renderer, Entity ent) : Component(ent) {
        SDL_Surface *surface = IMG_Load((constants::gResPath + image_path).c_str());
        texture = SDL_CreateTextureFromSurface(renderer, surface);
    }

    inline SDL_Texture *Image::getTexture() const {
        return texture;
    }

    inline void Image::setTexture(SDL_Texture *_texture) {
        this->texture = _texture;
    }
}

#endif