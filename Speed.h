//
// Created by Martvall on 11/01/2024.
//

#ifndef TEST1_SPEED_H
#define TEST1_SPEED_H

class Speed : public Component<Speed> {

public:
    int xspeed;
    int yspeed;
    static void New(int xspeed, int yspeed, Entity ent) {
        comps.push_back(std::unique_ptr<Speed>(new Speed(xspeed, yspeed, ent)));
    }

private:
    inline Speed(int xspeed, int yspeed, Entity ent);

};

inline Speed::Speed(int _xspeed, int _yspeed, Entity ent) : Component(ent), xspeed(_xspeed), yspeed(_yspeed) {

}

#endif //TEST1_SPEED_H
