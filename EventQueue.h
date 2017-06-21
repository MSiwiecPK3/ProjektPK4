#pragma once

#include <memory>

#include "IStateStackable.h"

class State;

class EventQueue
{
        IStateStackable &logic; //IStateStackable tam s¹ abstrakcyjne klasy

    public:
        EventQueue(IStateStackable &logic) :logic(logic){}

        void pushState(std::shared_ptr<State> newState) {
            logic.pushState(newState);
        }

        void popState() {
            logic.popState();
        }

        void exitGame() {
            logic.exitGame();
        }
};