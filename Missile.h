#ifndef Image

class Missile : public Component<Missile> {

public:

    static void New(Entity ent) {
        comps.push_back(std::unique_ptr<Missile>(new Missile(ent)));
    }

//    ~Image() {
//        SDL_DestroyTexture(texture);
//    }

private:
    inline Missile(Entity ent);

};

inline Missile::Missile(Entity ent) : Component(ent) {

}

#endif