#include "Logic.h"
#include "State.h"
#include "Input.h"

    Logic::Logic(sf::Time logicTickDelta) :
        logicTickDelta(logicTickDelta), stateStack()
    {
        auto eventQueue = std::make_shared<EventQueue>(*this);
        this->eventQueue = eventQueue;
        stateStack.push(std::make_shared<MenuScreenState>(logicTickDelta, eventQueue));
    }

    bool Logic::update(sf::Time timeElapsed, std::function<Input()> getInput)
    {
        lag += timeElapsed;

        while (lag > logicTickDelta) {
            auto input = getInput();
            getCurrentState()->update(input);

            if (!gameRunning) { return false; }

            lag -= logicTickDelta;
        }

        return true;
    }

    void Logic::render(sf::RenderWindow &renderer)
    {
        renderer.clear();

        float interpolationFactor = lag / logicTickDelta;

        getCurrentState()->render(renderer , interpolationFactor);

        renderer.display();
    }