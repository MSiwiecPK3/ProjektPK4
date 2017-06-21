#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <map>
#include <vector>
#include <algorithm>
#include <list>

#include "Input.h"
#include "State.h"

class BaseState
{

};

class GameState;

class BaseComponent
{
    public:
        virtual void update(BaseState &, GameState &) = 0;
};

class BaseGameObject
{
    public:
        virtual BaseState &getState() = 0;
        virtual std::map<std::string, std::unique_ptr<BaseComponent>> &getComponents() = 0;

        void update(GameState &gameState);
        void render(GameState &gameState);
};

typedef std::tuple<std::string, std::shared_ptr<BaseGameObject>> GameObjectListPair;  // definicja GameObjectListPair

class GameState 
{
        std::list<GameObjectListPair> gameObjects;
        sf::RenderWindow *renderer;
        State *currentState;

    public:
        GameState(State *currentState);
        GameState(State *currentState, sf::RenderWindow *renderer);
        Input getInput();
        sf::Time getTickDelta();
        sf::RenderWindow *getRenderWindow();
        std::vector<std::shared_ptr<BaseGameObject>> getGameObjectsByTag(std::string tag);
        
        GameState &addGameObject(std::shared_ptr<BaseGameObject> gameObject, std::string tag = "");
        GameState &removeGameObject(std::shared_ptr<BaseGameObject> gameObject);
        template<typename StateT>
        GameState &pushState()
        {
            auto state = std::make_shared<StateT>(getTickDelta(), currentState->eventQueue);
            currentState->eventQueue->pushState(state);
            return *this;
        }
        GameState &popState();
        GameState &exitGame();
        GameState &render(sf::Drawable &drawable);

};

template <typename StateT>
class Component : public BaseComponent
{
      
};

template <typename StateT>
class GameObject : public BaseGameObject
{
        StateT state; 
        std::map<std::string, std::unique_ptr<BaseComponent>> components; //mapka componentów
            
    public:
        StateT &getState() { return state; }
        std::map<std::string, std::unique_ptr<BaseComponent>> &getComponents() { return components; } 


        GameObject &addComponent(std::string name, std::unique_ptr<Component<StateT>> component)
        {
            components[name] = std::move(component);
            return *this;
        }
};