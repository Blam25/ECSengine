#ifndef Missile_h
#define Missile_h

//Komponent som endast utmärker ifall en entity är en Missil eller inte
class Missile : public Engine::Component<Missile> {

    public:

        static void New(Engine::Entity ent) {
            getComps().push_back(std::unique_ptr<Missile>(new Missile(ent)));
        }

    private:
        inline Missile(Engine::Entity ent);

    };

    inline Missile::Missile(Engine::Entity ent) : Component(ent) {

    }

#endif