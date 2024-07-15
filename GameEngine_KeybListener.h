//Grupp 80
//Adrian Andersson Martvall adan2936
//Ida Laaksonen idla8418
#ifndef keybListener_h
#define keybListener_h

#include <functional>
#include "GameEngine_Component.h"

namespace Engine {

    class Keyboard_Listener : public Component<Keyboard_Listener>{
    public:
        std::function<void(int event)> handler;

        inline Keyboard_Listener(Entity ent, std::function<void(int event)> handler) : Component(ent) {
            this->handler = handler;
        }

        static void New(Entity ent, std::function<void(int event)> handler) {
            getComps().push_back(std::make_unique<Keyboard_Listener>(ent, handler));
        }

    };

}
#endif //CPROG_TEMPLATE_WIN_231017_KEYBOARD_LISTENER_H