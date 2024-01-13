//
// Created by Martvall on 17/11/2023.
//

#ifndef Entity_h
#define Entity_h

namespace Engine {

//Wrapper klass för en int med tillhörande counter som räknar uppåt.
//Eftersom klassen endast innehåller en int så tillåts värdesemantik då
//endast själva int:en kopieras, inte någon tillhörande komponent, såsom bilder eller liknande.
//De ligger i de olika komponentklasser som inte tillåter värdesemantik
    class Entity {
    public:
        Entity() : id(id_counter) {
            id_counter++;
        }

        bool operator==(Entity entity) {
            if (this->id == entity.id) {
                return true;
            }
            return false;
        }

    private:
        int id;
        inline static int id_counter = 0;
    };

}
#endif //CPROG_TEMPLATE_WIN_231017_ENTITY_H
