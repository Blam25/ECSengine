//
// Created by Martvall on 18/11/2023.
//


#include "Event.h"

#ifndef CPROG_TEMPLATE_WIN_231017_KEYBOARD_EVENT_H
#define CPROG_TEMPLATE_WIN_231017_KEYBOARD_EVENT_H


namespace Engine {
    class Keyboard_Event : Event {
    public:
        static std::vector<Entity> entitys;
        SDL_KeyCode key;

        explicit Keyboard_Event(SDL_KeyCode key) {
            this->key = key;
        }

        bool operator<(const Keyboard_Event &other) const {
            return this->key < other.key;  //assume that you compare the record based on a
        }
    };
}

#endif //CPROG_TEMPLATE_WIN_231017_KEYBOARD_EVENT_H