//
// Created by Martvall on 18/11/2023.
//

#ifndef keybListener_h
#define keybListener_h

#include <functional>

namespace Engine {

    class Keyboard_Listener : public Component<Keyboard_Listener>{
    public:
        std::function<void(Keyboard_Event event)> handler;

        Keyboard_Listener(Entity ent, std::function<void(Keyboard_Event event)> handler) : Component(ent) {
            this->handler = handler;
        }

        static void New(Entity ent, std::function<void(Keyboard_Event event)> handler) {
            getComps().push_back(std::make_unique<Keyboard_Listener>(ent, handler));
        }

    };

}
#endif //CPROG_TEMPLATE_WIN_231017_KEYBOARD_LISTENER_H