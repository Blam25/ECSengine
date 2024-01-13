//
// Created by Martvall on 10/01/2024.
//


#ifndef Component
#include <memory>
#include <vector>
#include <boost/optional/optional.hpp>
namespace Engine {

//Basklass för samtliga komponenter som innehåller en statisk samling av komponenten,
//samt en referens till en specifik entity genom en instansvariabel.
//Klassen förbjuder värdesemantik och definierar olika funktioner som går att utföra på samlingen
//som är gemensamma för alla komponenter. Det går heller inte att skapa objekt av denna klass då
//den är abstrakt
    template<typename T>
    class Component {
    public:
        static void Remove(Entity ent) {
            auto iter = comps.begin();
            bool remove = false;
            for (auto i = comps.begin(); i != comps.end(); i++) {
                auto instance = Get(ent);
                if (instance.has_value() && *i == instance.get()) {
                    iter = i;
                    remove = true;
                }
            }
            if (remove) comps.erase(iter);
        }

        static boost::optional<std::unique_ptr<T> &> Get(Entity ent) {
            for (auto &comp: comps) {
                if (ent == comp->ent) {
                    return comp;
                }
            }
            return boost::none;
        }

        virtual ~Component() = 0;

        Component(const Component &other) = delete;

        Component &operator=(const Component &other) = delete;

        Entity getEnt() const {
            return ent;
        }

        void setEnt(Entity _ent) {
            this->ent = _ent;
        }

        static std::vector<std::unique_ptr<T>> &getComps() {
            return comps;
        }


    protected:
        Component(Entity entity) : ent(entity) {};

    private:
        Entity ent;
        inline static std::vector<std::unique_ptr<T>> comps;
    };

    template<typename T>
    Component<T>::~Component() {};

}
#endif
