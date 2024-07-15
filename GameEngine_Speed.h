//Grupp 80
//Adrian Andersson Martvall adan2936
//Ida Laaksonen idla8418
#ifndef Speed_h
#define Speed_h

#include "GameEngine_Component.h"

namespace Engine {

    class Speed : public Component<Speed> {

    public:
        int getXspeed() const;

        void setXspeed(int xspeed);

        int getYspeed() const;

        void setYspeed(int yspeed);

        static void New(int xspeed, int yspeed, Entity ent) {
            getComps().push_back(std::unique_ptr<Speed>(new Speed(xspeed, yspeed, ent)));
        }

    private:
        int xspeed;
        int yspeed;

        Speed(int xspeed, int yspeed, Entity ent);

    };
}
#endif //TEST1_SPEED_H
