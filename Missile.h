#ifndef Missile
namespace Engine {
//Komponent som endast utmärker ifall en entity är en Missil eller inte
    class Missile : public Component<Missile> {

    public:

        static void New(Entity ent) {
            getComps().push_back(std::unique_ptr<Missile>(new Missile(ent)));
        }

    private:
        inline Missile(Entity ent);

    };

    inline Missile::Missile(Entity ent) : Component(ent) {

    }
}
#endif