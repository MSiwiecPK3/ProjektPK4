#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include <functional>

#include "IStateStackable.h"
#include "EventQueue.h"
#include "Input.h"

class Logic : public IStateStackable //<--to klasa abstrakcyjna z metodami wirtualnymi
{
        sf::Time lag, logicTickDelta;
        std::shared_ptr<EventQueue> eventQueue;
        std::stack<std::shared_ptr<State>> stateStack;
        bool gameRunning = true;

        inline std::shared_ptr<State> getCurrentState() {
            return stateStack.top();
        }

    public:
        Logic(sf::Time logicTickDelta);

        bool update(sf::Time timeElapsed, std::function<Input()> getInput);

        void render(sf::RenderWindow &renderer);

        inline void pushState(std::shared_ptr<State> newState)
        {
            stateStack.push(newState);
        }

        inline void popState()
        {
            stateStack.pop();
        }

        inline void exitGame()
        {
            gameRunning = false;
        }
};