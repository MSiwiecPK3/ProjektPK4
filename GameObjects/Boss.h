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
#include "Enemy.h"

#include "EnemyCounter.h"

class BossGraphicsComponent : public Component<EnemyState>
{
	sf::RectangleShape rectangle;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	BossGraphicsComponent()
	{
		texture.loadFromFile("Resources\\boss.png");
		sprite.setTexture(texture);
	}

	void update(BaseState &enemyBaseState, GameState &gameState)
	{
		EnemyState &enemyState = reinterpret_cast<EnemyState &>(enemyBaseState);
		sf::Vector2f newposition = enemyState.position;
		if (newposition.y == 50)
			newposition.y = 40;
		else
			newposition.y -= 160;
		sprite.setPosition(newposition);
		enemyState.sprite = &sprite;
		gameState.render(sprite);
	}
};

class BossPhysicsComponent : public Component<EnemyState>
{
public:
	BossPhysicsComponent()
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