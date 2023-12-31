
class Position {

public:
    inline static vector<unique_ptr<Position>> comps;
    inline static vector<Position*> positions2;
    Entity ent;
    int x;
    int y;

    Position(int x, int y, Entity ent) : ent(ent) {
        this->x = x;
        this->y = y;
        //comps.push_back(make_unique<Position>(*this));
        //positions2.push_back(this);

    }

    static void New(int x, int y, Entity ent) {
        comps.push_back(make_unique<Position>(x, y, ent));
    }

};
