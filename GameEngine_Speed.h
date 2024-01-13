//
// Created by Martvall on 11/01/2024.
//

#ifndef Speed_h
#define Speed_h
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

        inline Speed(int xspeed, int yspeed, Entity ent);

    };

    inline Speed::Speed(int _xspeed, int _yspeed, Entity ent) : Component(ent), xspeed(_xspeed), yspeed(_yspeed) {

    }

    inline int Speed::getXspeed() const {
        return xspeed;
    }

    inline void Speed::setXspeed(int _xspeed) {
        Speed::xspeed = _xspeed;
    }

    inline int Speed::getYspeed() const {
        return yspeed;
    }

    inline void Speed::setYspeed(int _yspeed) {
        Speed::yspeed = _yspeed;
    }
}
#endif //TEST1_SPEED_H
