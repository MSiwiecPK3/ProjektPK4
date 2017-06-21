#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "../Input.h"
#include "../GameObject.h"
#include "Floor.h"

#include "EnemyCounter.h"

struct EnemyState : public BaseState
{
	
	float pos = (rand() % 3) * 150.0f + 50.0f;
    sf::Vector2f position = { 800.0f, pos };
    sf::Vector2f size = { 80.f, 90.0f };
    sf::Sprite *sprite;
    bool enemyDead = false;
};

class EnemyGraphicsComponent : public Component<EnemyState>
{
        sf::RectangleShape rectangle;
        sf::Texture texture;
        sf::Sprite sprite;

    public:
        EnemyGraphicsComponent() 
        {
            texture.loadFromFile("Resources\\orc.png");
            sprite.setTexture(texture);
        }

        void update(BaseState &enemyBaseState, GameState &gameState)
        {
            EnemyState &enemyState = reinterpret_cast<EnemyState &>(enemyBaseState);
            sprite.setPosition(enemyState.position);
            enemyState.sprite = &sprite;
            gameState.render(sprite);
        }
};

class EnemyPhysicsComponent : public Component<EnemyState>
{
    public:
        EnemyPhysicsComponent()
        {

        }

        void update(BaseState &enemyBaseState, GameState &gameState)
        {
            EnemyState &enemyState = reinterpret_cast<EnemyState &>(enemyBaseState);
            enemyState.position.x -= 2.0f;

            if (enemyState.position.x < 160.0f && !enemyState.enemyDead) { //po odleglosci 160f zabijamy enemy
                enemyState.enemyDead = true;
                
                auto enemiesDeadCounters = gameState.getGameObjectsByTag("enemiesDeadCounter"); //wyci¹gam z mapy obiekt zliczaj¹cy i go zwiêkszam

                if (enemiesDeadCounters.size() > 0) {
                    auto enemiesDeadCounter = std::dynamic_pointer_cast<GameObject<EnemyCounterState>>(enemiesDeadCounters[0]);
					enemiesDeadCounter->getState().enemiesDeadCount++;
                }
            }
        }
};