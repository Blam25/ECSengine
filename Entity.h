//
// Created by Martvall on 17/11/2023.
//

#ifndef CPROG_TEMPLATE_WIN_231017_ENTITY_H
#define CPROG_TEMPLATE_WIN_231017_ENTITY_H

class Entity {
public:
    Entity() : id(id_counter) {
        id_counter++;
    }

    bool operator== (Entity entity) {
        if (this->id == entity.id) {
            return true;
        }
        return false;
    }

private:
    int id;
    inline static int id_counter = 0;
};


#endif //CPROG_TEMPLATE_WIN_231017_ENTITY_H
