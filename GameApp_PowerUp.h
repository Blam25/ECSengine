//Grupp 80
//Adrian Andersson Martvall adan2936
//Ida Laaksonen idla8418
#ifndef PowerUp_h
#define PowerUp_h

#include "GameEngine_Component.h"

//Komponent som endast utmärker ifall en entity är en Missil eller inte
    class PowerUp : public Engine::Component<PowerUp> {

    public:

        static void New(Engine::Entity ent) {
            getComps().push_back(std::unique_ptr<PowerUp>(new PowerUp(ent)));
        }

    private:
        inline PowerUp(Engine::Entity ent);

    };

    inline PowerUp::PowerUp(Engine::Entity ent) : Component(ent) {

    }

#endif