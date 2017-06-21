#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"
#include "Enemy.h"
#include "FastEnemy.h"
#include "Boss.h"
#include <random>
#include <stdlib.h>

struct EnemySpawnerState : public BaseState
{
	sf::Time elapsed;
    sf::Time lastSpawn;
    int gameSpeedScale = 1;
};


class EnemySpawnerInfoLabelComponent : public Component<EnemySpawnerState>
{
        sf::Font font;
        sf::Text text;

    public:
        EnemySpawnerInfoLabelComponent()
        {
            if (!font.loadFromFile("Resources\\Space Comics.ttf")) {
                throw new std::runtime_error("Couldn't load font");
            }

            text.setFont(font);
            text.setString("ENEMIES LVL: 0");
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Black);
            text.setPosition({ 500.0f, 40.0f });
        }

        void update(BaseState &spawnerStateBase, GameState &gameState)
        {
            EnemySpawnerState &spawnerState = reinterpret_cast<EnemySpawnerState &>(spawnerStateBase);

            text.setString("ENEMIES LVL: " + std::to_string(spawnerState.gameSpeedScale));
            gameState.render(text);
        }
};

class EnemySpawnerComponent : public Component<EnemySpawnerState>
{
        sf::Time nextInterval;
        sf::Vector2f spawnPosition = { 900.0f, 400.0f };
         std::random_device randomDevice;
         std::mt19937 randomGenerator;
         std::uniform_real_distribution<float> randomDistribution;

        std::shared_ptr<GameObject<EnemyState>> newEnemy()
        {
			auto enemy = std::make_shared<GameObject<EnemyState>>();
			int random = (rand() % 6);
			if (random == 0) {

				(*enemy).addComponent("physicsComponent", std::make_unique<BossPhysicsComponent>())
					.addComponent("graphicsComponent", std::make_unique<BossGraphicsComponent>());

				return enemy;
			}
			if ((random == 4) || (random == 5)) {

				(*enemy).addComponent("physicsComponent", std::make_unique<FastEnemyPhysicsComponent>())
					.addComponent("graphicsComponent", std::make_unique<FastEnemyGraphicsComponent>());

				return enemy;
			}
			if ((random == 1) || (random == 2) || (random == 3)) {
				
				(*enemy).addComponent("physicsComponent", std::make_unique<EnemyPhysicsComponent>())
					.addComponent("graphicsComponent", std::make_unique<EnemyGraphicsComponent>());

				return enemy;
			}
        }

    public:
        EnemySpawnerComponent() :
            randomDevice(), randomGenerator(randomDevice()), randomDistribution(5, 10)
        {
            nextInterval = sf::seconds(randomDistribution(randomGenerator));
        }

	    void update(BaseState &spawnerStateBase, GameState &gameState)
        {
            EnemySpawnerState &spawnerState = reinterpret_cast<EnemySpawnerState &>(spawnerStateBase);

            spawnerState.elapsed += gameState.getTickDelta();

            if (spawnerState.elapsed.asMilliseconds() >= spawnerState.lastSpawn.asMilliseconds() + nextInterval.asMilliseconds()) {
                spawnerState.lastSpawn = spawnerState.elapsed;

                nextInterval = sf::seconds(randomDistribution(randomGenerator));

                if (spawnerState.elapsed > sf::seconds(15 * static_cast<float>(spawnerState.gameSpeedScale))) {
                    if (spawnerState.gameSpeedScale == 1) {
                        randomDistribution = std::uniform_real_distribution<float>(3, 6);
                        spawnerState.gameSpeedScale++;
                    }
                    else if (spawnerState.gameSpeedScale == 2) {
                        randomDistribution = std::uniform_real_distribution<float>(2, 4);
                        spawnerState.gameSpeedScale++;
                    }
                    else if (spawnerState.gameSpeedScale == 3) {
                        randomDistribution = std::uniform_real_distribution<float>(2, 3);
                        spawnerState.gameSpeedScale++;
                    }
                    else if (spawnerState.gameSpeedScale == 4) {
                        randomDistribution = std::uniform_real_distribution<float>(1, 2);
                        spawnerState.gameSpeedScale++;
                    }
                    else if (spawnerState.gameSpeedScale == 5) {
                        randomDistribution = std::uniform_real_distribution<float>(0.5, 1);
                        spawnerState.gameSpeedScale++;
                    }
                }

                gameState.addGameObject(newEnemy(), "enemy");
            }
        }
};