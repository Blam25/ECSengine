//
// Created by Martvall on 17/11/2023.
//

#ifndef CPROG_TEMPLATE_WIN_231017_ENTITY_H
#define CPROG_TEMPLATE_WIN_231017_ENTITY_H

class Entity {
public:
    inline static int id_counter = 0;
    const int id;

    Entity() : id(id_counter) {
        id_counter++;
    }

    static unique_ptr<Entity> New() {
        return make_unique<Entity>();
    }

    bool operator== (Entity entity) {
        if (this->id == entity.id) {
            return true;
        }
        return false;
    }

};


#endif //CPROG_TEMPLATE_WIN_231017_ENTITY_H
