#ifndef Rect
class Rect : public Component<Rect> {

public:
    unique_ptr<SDL_Rect> rect;
    unique_ptr<SDL_Rect> rect_up;
    unique_ptr<SDL_Rect> rect_right;
    unique_ptr<SDL_Rect> rect_left;
    unique_ptr<SDL_Rect> rect_down;
    bool collided = false;
    static void New(int x, int y, int width_height, Entity ent);
private:
    inline Rect(int x, int y, int width_height, Entity ent);

};

void Rect::New(int x, int y, int width_height, Entity ent) {
    comps.push_back(std::unique_ptr<Rect>(new Rect(x, y, width_height, ent)));
}

inline Rect::Rect(int x, int y, int width_height, Entity ent) : Component(ent) {
    rect = make_unique<SDL_Rect>();
    rect->x = x;
    rect->y = y;
    rect->h = width_height;
    rect->w = width_height;
}
#endif