#ifndef Image

class Image : public Component<Image> {

public:
    SDL_Texture *texture;

    static void New(string image_path, SDL_Renderer *renderer, Entity ent) {
        comps.push_back(std::unique_ptr<Image>(new Image(image_path, renderer, ent)));
    }

private:
    inline Image(string image_path, SDL_Renderer *renderer, Entity ent);

};

inline Image::Image(string image_path, SDL_Renderer *renderer, Entity ent) : Component(ent) {
    SDL_Surface *surface = IMG_Load((constants::gResPath + image_path).c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
}

#endif