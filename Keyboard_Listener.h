//
// Created by Martvall on 18/11/2023.
//

#ifndef CPROG_TEMPLATE_WIN_231017_KEYBOARD_LISTENER_H
#define CPROG_TEMPLATE_WIN_231017_KEYBOARD_LISTENER_H

namespace Engine {

    class Keyboard_Listener {
    public:
        inline static std::vector<std::unique_ptr<Keyboard_Listener>> listeners;
        Entity ent;
        std::function<void(Keyboard_Event event)> handler;

        Keyboard_Listener(Entity ent, std::function<void(Keyboard_Event event)> handler) : ent(ent) {
            this->handler = handler;
        }

        static void New(Entity ent, std::function<void(Keyboard_Event event)> handler) {
            listeners.push_back(std::make_unique<Keyboard_Listener>(ent, handler));
        }

        static boost::optional<std::unique_ptr<Keyboard_Listener> &> Get(Entity ent) {
            for (auto &rect: listeners) {
                if (ent == rect->ent) {
                    return rect;
                }
            }
            return boost::none;
        }

    };

}
#endif //CPROG_TEMPLATE_WIN_231017_KEYBOARD_LISTENER_H