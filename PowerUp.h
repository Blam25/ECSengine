#ifndef PowerUp
namespace Engine {
//Komponent som endast utmärker ifall en entity är en Missil eller inte
    class PowerUp : public Component<PowerUp> {

    public:

        static void New(Entity ent) {
            getComps().push_back(std::unique_ptr<PowerUp>(new PowerUp(ent)));
        }

    private:
        inline PowerUp(Entity ent);

    };

    inline PowerUp::PowerUp(Entity ent) : Component(ent) {

    }
}
#endif