//
// Created by Martvall on 13/01/2024.
//

#ifndef NPC
namespace Engine {

//Komponent som endast utmärker ifall en entity är en NPC eller inte
    class NPC : public Component<NPC> {

    public:

        static void New(Entity ent) {
            getComps().push_back(std::unique_ptr<NPC>(new NPC(ent)));
        }

    private:
        inline NPC(Entity ent);

    };

    inline NPC::NPC(Entity ent) : Component(ent) {

    }
}
#endif
