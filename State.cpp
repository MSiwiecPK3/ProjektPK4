#include "State.h"

#include "EventQueue.h"
#include "Input.h"
#include "GameObject.h"

#include "GameObjects/Player.h"
#include "GameObjects/Floor.h"
#include "GameObjects/Menu.h"
#include "GameObjects/Enemy.h"
#include "GameObjects/EnemySpawner.h"
#include "GameObjects/GameBackground.h"
#include "GameObjects/EnemyCounter.h"
#include "GameObjects/LeftSpikes.h"
#include "GameObjects/RightSpikes.h"
#include "GameObjects/SecondFloor.h"
#include "GameObjects/ThirdFloor.h"


    State &State::addGameObject(std::shared_ptr<BaseGameObject> gameObject, std::string tag)
    {
        gameObjects.push_back(std::make_tuple(tag, gameObject)); //make tuple dodaje nowy obiekt
        return *this;
    }

    State &State::removeGameObject(std::shared_ptr<BaseGameObject> gameObject)
    {
        gameObjects.remove_if([&](std::tuple<std::string, std::shared_ptr<BaseGameObject>> pair) { 
            return std::get<1>(pair) == gameObject;
        });
        return *this;
    }

    State::State(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :
        tickDelta(tickDelta)
    {
        this->eventQueue = eventQueue;
	}

    std::vector<std::shared_ptr<BaseGameObject>> State::getGameObjectsByTag(std::string tag)
    {
        std::vector<std::shared_ptr<BaseGameObject>> result; //vector

        for (auto &pair : gameObjects) {
            auto pairTag = std::get<0>(pair);
            if (pairTag == tag) {
                auto ptr = std::get<1>(pair);
                result.push_back(ptr);
            }
        }

        return result;
    }

    void State::update(Input input)
    {
        this->input = input;

        GameState gameState(this);
        
        for (auto &gameObjectPair : gameObjects) //updejtujê wszystkie obiekty po kolei
        {
            std::get<1>(gameObjectPair)->update(gameState);
        }
    }

    void State::render(sf::RenderWindow &renderer, float interpolationFactor)
    {
        GameState gameState(this, &renderer);

        for (auto &gameObjectPair : gameObjects)
        {
            std::get<1>(gameObjectPair)->render(gameState);
        }
    }

    Input State::getInput()
    {
        return input;
    }

    sf::Time State::getTickDelta()
    {
        return tickDelta;
    }

// ======================

    PlayingGameState::PlayingGameState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :		//tworzenie gameobjectow i przypisywanie im componentow
        State(tickDelta, eventQueue)
    {       
        auto Player = std::make_shared<GameObject<PlayerState>>();
        auto floor = std::make_shared<GameObject<FloorState>>();
        auto spawner = std::make_shared<GameObject<EnemySpawnerState>>();
        auto background = std::make_shared<GameObject<GameBackgroundState>>();
        auto enemiesDeadCounter = std::make_shared<GameObject<EnemyCounterState>>();
		auto leftSpikes = std::make_shared<GameObject<LeftSpikeState>>();
		auto rightSpikes = std::make_shared<GameObject<RightSpikeState>>();
		auto secFloor = std::make_shared<GameObject<SecFloorState>>();
		auto thirdFloor = std::make_shared<GameObject<ThirdFloorState>>();

        enemiesDeadCounter->addComponent("graphicsComponent", std::make_unique<EnemyCounterGraphicsComponent>());

        background->addComponent("graphicsComponent", std::make_unique<GameBackgroundGraphicsComponent>());

        (*Player).addComponent("inputComponent", std::make_unique<PlayerInputComponent>())
                     .addComponent("physicsComponent", std::make_unique<PlayerPhysicsComponent>())
                     .addComponent("graphicsComponent", std::make_unique<PlayerGraphicsComponent>());

        floor->addComponent("graphicsComponent", std::make_unique<FloorGraphicsComponent>());

        (*spawner).addComponent("enemySpawnerComponent", std::make_unique<EnemySpawnerComponent>())
                  .addComponent("graphicsComponent", std::make_unique<EnemySpawnerInfoLabelComponent>());

		leftSpikes->addComponent("graphicsComponent", std::make_unique<LeftSpikeGraphicsComponent>());

		rightSpikes->addComponent("graphicsComponent", std::make_unique<RightSpikeGraphicsComponent>());

		secFloor->addComponent("graphicsComponent", std::make_unique<SecFloorGraphicsComponent>());

		thirdFloor->addComponent("graphicsComponent", std::make_unique<ThirdFloorGraphicsComponent>());

        addGameObject(background, "background");
        addGameObject(floor, "floor");
		addGameObject(secFloor, "floor");
		addGameObject(thirdFloor, "floor");
        addGameObject(spawner, "enemySpawner");
        addGameObject(Player, "player");
        addGameObject(enemiesDeadCounter, "enemiesDeadCounter");
		addGameObject(leftSpikes, "leftSpikes");
		addGameObject(rightSpikes, "rightSpikes");

    }

// ====================

    MenuScreenState::MenuScreenState(sf::Time tickDelta, std::shared_ptr<EventQueue> eventQueue) :
        State(tickDelta, eventQueue)
    {
        auto menu = std::make_shared<GameObject<MenuState>>();

        (*menu).addComponent("inputComponent", std::make_unique<MenuInputComponent>())
               .addComponent("graphicsComponent", std::make_unique<MenuGraphicsComponent>());

        addGameObject(menu);
    }