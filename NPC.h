#ifndef Image

class NPC : public Component<NPC> {

public:

    static void New(Entity ent) {
        comps.push_back(std::unique_ptr<NPC>(new NPC(ent)));
    }

//    ~Image() {
//        SDL_DestroyTexture(texture);
//    }

private:
    inline NPC(Entity ent);

};

inline NPC::NPC(Entity ent) : Component(ent) {

}

#endif