//Grupp 80
//Adrian Andersson Martvall adan2936
//Ida Laaksonen idla8418
#include "GameEngine_Speed.h"

namespace Engine {

    Speed::Speed(int _xspeed, int _yspeed, Entity ent) : Component(ent), xspeed(_xspeed), yspeed(_yspeed) {

    }

    int Speed::getXspeed() const {
        return xspeed;
    }

    void Speed::setXspeed(int _xspeed) {
        Speed::xspeed = _xspeed;
    }

    int Speed::getYspeed() const {
        return yspeed;
    }

    void Speed::setYspeed(int _yspeed) {
        Speed::yspeed = _yspeed;
    }
}
