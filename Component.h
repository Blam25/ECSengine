//
// Created by Martvall on 10/01/2024.
//

#ifndef TEST1_COMPONENT_H
#define TEST1_COMPONENT_H


template <typename T>
class Component {
public:
    Entity ent;
    inline static vector<unique_ptr<T>> comps;
    static void Remove(Entity ent) {
        auto iter = comps.begin();
        bool remove = false;
        for (auto i = comps.begin(); i != comps.end(); i++) {
            auto instance = Get(ent) ;
            if (instance.has_value() && *i == instance.get()) {
                iter = i;
                remove = true;
            }
        }
        if (remove) comps.erase(iter);
    }
    static boost::optional<unique_ptr<T>&> Get(Entity ent)  {
        for (auto &comp : comps) {
            if (ent == comp->ent) {
                return comp;
            }
        }
        return boost::none;
    }
    virtual ~Component() = 0;
    Component(const Component& other) = delete;
    Component& operator = (const Component& other) = delete;

protected:
    Component(Entity entity) : ent(entity) {};

private:

};

template <typename T>
Component<T>::~Component() {};



#endif //TEST1_COMPONENT_H
