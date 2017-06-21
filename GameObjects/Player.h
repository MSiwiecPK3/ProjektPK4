#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <iostream>

#include "../Input.h"
#include "../GameObject.h"
#include "Floor.h"
#include "SecondFloor.h"
#include "ThirdFloor.h"
#include "Enemy.h"
#include "LeftSpikes.h"
#include "RightSpikes.h"

#include "../Collision.h"

const float GravityConstant = 6.0f;
const float JumpIntentMargin = 20.0f;
const float JumpSpeed = 600.0f;

struct PlayerState : public BaseState
{
    sf::Vector2f position { 200.0f, 40.0f };
    sf::Vector2f size { 112.0f, 90.0f };
    sf::Vector2f accelerationVector { 0.0f, 0.0f };
    sf::Vector2f speed { 0.0f, 0.0f };
    sf::Vector2f direction { 0.0f, 0.0f };
    bool wantJump = false;
    bool didJump = false;
	bool wantFall = false;
	bool didFall = false;
    bool isDead = false;
	int floor = 3;
    sf::Sprite *sprite;
};

bool isColliding(PlayerState &player, RightSpikeState &rightspike, float margin = 0.0)
{
	auto playerLeft = player.position.x,
		playerRight = player.position.x + player.size.x,
		rightSpikeLeft = rightspike.position.x - margin,
		rightSpikeRight = rightspike.position.x + rightspike.size.x + margin;

	return !(playerLeft > rightSpikeLeft ||
		playerRight < rightSpikeRight);
}

bool isColliding(PlayerState &player, LeftSpikeState &leftspike, float margin = 0.0)
{
	auto playerLeft = player.position.x,
		playerRight = player.position.x + player.size.x,
		leftSpikeLeft = leftspike.position.x - margin,
		leftSpikeRight = leftspike.position.x + leftspike.size.x + margin;
	
	return !(playerLeft > leftSpikeLeft ||
		playerRight < leftSpikeRight);
}

bool isColliding(PlayerState &player, FloorState &floor, float margin = 0.0)
{
	auto playerBottom = player.position.y + player.size.y,
		rectangleTop = floor.position.y - margin;

    return !(playerBottom < rectangleTop);
}

bool isColliding(PlayerState &player, SecFloorState &floor, float margin = 0.0)
{
	auto playerBottom = player.position.y + player.size.y,
		rectangleTop = floor.position.y - margin;

	return !(playerBottom < rectangleTop);
}

bool isColliding(PlayerState &player, ThirdFloorState &floor, float margin = 0.0)
{
	auto playerBottom = player.position.y + player.size.y,
		rectangleTop = floor.position.y - margin;
		return !(playerBottom < rectangleTop);
}

bool isColliding(PlayerState &player, EnemyState &enemy, float margin = 0.0)
{
    auto playerLeft = player.position.x,
        playerRight = player.position.x + player.size.x,
        playerTop = player.position.y,
        playerBottom = player.position.y + player.size.y,
        rectangleLeft = enemy.position.x - margin,
        rectangleRight = enemy.position.x + enemy.size.x + margin,
        rectangleTop = enemy.position.y - margin,
        rectangleBottom = enemy.position.y + enemy.size.y + margin;

    return !(playerLeft > rectangleRight ||
        playerRight < rectangleLeft ||
        playerTop > rectangleBottom ||
        playerBottom < rectangleTop);
}

class PlayerInputComponent : public Component<PlayerState>
{
    public:
        void update(BaseState &playerStateBase, GameState &gameState)
        {
            PlayerState &playerState = reinterpret_cast<PlayerState &>(playerStateBase);
            Input input = gameState.getInput();

            if (input.no == KeyState::RELEASED || playerState.isDead) {
                gameState.popState();
                return;
            }

            auto floorList = gameState.getGameObjectsByTag("floor");
            if (!floorList.empty()) {
                auto firstfloor = std::dynamic_pointer_cast<GameObject<FloorState>>(floorList[0]);
				auto secondfloor = std::dynamic_pointer_cast<GameObject<SecFloorState>>(floorList[1]);
				auto thirdfloor = std::dynamic_pointer_cast<GameObject<ThirdFloorState>>(floorList[2]);

                if (!playerState.didJump && (isColliding(playerState, firstfloor->getState(), JumpIntentMargin) ||
					isColliding(playerState, secondfloor->getState(), JumpIntentMargin) || isColliding(playerState, thirdfloor->getState(), JumpIntentMargin))
					&& input.jump == KeyState::DOWN) {
                    playerState.wantJump = true;
                }

				if (!playerState.didJump && (isColliding(playerState, firstfloor->getState(), JumpIntentMargin) ||
					isColliding(playerState, secondfloor->getState(), JumpIntentMargin) || isColliding(playerState, thirdfloor->getState(), JumpIntentMargin))
					&& input.fall == KeyState::DOWN && playerState.position.y < 350) {
					playerState.wantFall = true;
				}

                if (playerState.didJump && input.jump == KeyState::RELEASED) {
                    playerState.didJump = false;
                }

				if (playerState.didFall && input.fall == KeyState::RELEASED) {
					playerState.didFall = false;
				}
            }

            playerState.direction = { (input.left == KeyState::DOWN) * -1.0f + (input.right == KeyState::DOWN) * 1.0f, 0.0f};
        }
};

class PlayerGraphicsComponent : public Component<PlayerState>
{
        sf::Texture texture;
        sf::Sprite sprite;

    public:
        PlayerGraphicsComponent()
        {
            texture.loadFromFile("Resources\\knight.png");
            sprite.setTexture(texture);
        }

        void update(BaseState &playerStateBase, GameState &gameState)
        {
            PlayerState &playerState = reinterpret_cast<PlayerState &>(playerStateBase);
            sprite.setPosition(playerState.position);
            playerState.sprite = &sprite;
            gameState.render(sprite);
        }
};

class PlayerPhysicsComponent : public Component<PlayerState>
{
        float acceleration = 600.0f;
        float maxSpeed = 400.0f;
        float frictionFactor = 0.01f;
        float movementThreshold = 0.0001f;

    public:
        PlayerPhysicsComponent()
        {

        }

        void update(BaseState &playerStateBase, GameState &gameState)
        {
			bool passedFirstFloor;
			bool passedSecondFloor;
			bool passedThirdFloor;
            PlayerState &playerState = reinterpret_cast<PlayerState &>(playerStateBase);

            auto enemyList = gameState.getGameObjectsByTag("enemy");
            bool isCollidingWithEnemy = false;
            for (auto enemyPtr : enemyList) {
                auto enemy = std::dynamic_pointer_cast<GameObject<EnemyState>>(enemyPtr); 
                auto enemyState = enemy->getState();
                if (isColliding(playerState, enemyState)) {
                    if (playerState.sprite && enemyState.sprite) {
                        isCollidingWithEnemy = Collision::pixelPerfectTest(*playerState.sprite, *enemyState.sprite);
                    } 
					else {
                        isCollidingWithEnemy = true;
                    }
                }
            }
			
            if (isCollidingWithEnemy) {
                playerState.isDead = true;
                return;
            }
			
			auto leftSpikeList = gameState.getGameObjectsByTag("leftSpikes");
			bool isCollidingWithLeftSpike = false;
			if (!leftSpikeList.empty()) {
				auto leftSpike = std::dynamic_pointer_cast<GameObject<LeftSpikeState>>(leftSpikeList[0]);
				auto spikeState = leftSpike->getState();
				if (isColliding(playerState, spikeState)) {
					if (playerState.sprite && spikeState.sprite) {
						isCollidingWithLeftSpike = Collision::pixelPerfectTest(*playerState.sprite, *spikeState.sprite);
					}
					else {
						isCollidingWithEnemy = true;
					}
				}
			}
			if (isCollidingWithLeftSpike) {
				playerState.isDead = true;
				return;
			}

			auto rightSpikeList = gameState.getGameObjectsByTag("rightSpikes");
			bool isCollidingWithRightSpike = false;
			if (!rightSpikeList.empty()) {
				auto rightSpike = std::dynamic_pointer_cast<GameObject<RightSpikeState>>(rightSpikeList[0]);
				auto spikeState = rightSpike->getState();
				if (isColliding(playerState, spikeState)) {
					if (playerState.sprite && spikeState.sprite) {
						isCollidingWithRightSpike = Collision::pixelPerfectTest(*playerState.sprite, *spikeState.sprite);
					}
					else {
						isCollidingWithEnemy = true;
					}
				}
			}
			if (isCollidingWithRightSpike) {
				playerState.isDead = true;
				return;
			}
			sf::Vector2f playerSpeed = playerState.speed;
            auto floorList = gameState.getGameObjectsByTag("floor");
            bool isCollidingFloor = false;
			 if (!floorList.empty()) {

				auto firstfloor = std::dynamic_pointer_cast<GameObject<FloorState>>(floorList[0]);
				auto secondfloor = std::dynamic_pointer_cast<GameObject<SecFloorState>>(floorList[1]);
				auto thirdfloor = std::dynamic_pointer_cast<GameObject<ThirdFloorState>>(floorList[2]);
				if (playerState.position.y > 370) {
					isCollidingFloor = isColliding(playerState, firstfloor->getState(), JumpIntentMargin);
					playerSpeed.y = -(playerSpeed.y - (playerSpeed.y * 0.05f));
				}
				if (200 < playerState.position.y < 211)
					isCollidingFloor = isColliding(playerState, secondfloor->getState(), JumpIntentMargin);
				if (playerState.position.y < 59) {
					isCollidingFloor = isColliding(playerState, thirdfloor->getState(), JumpIntentMargin);
					playerSpeed.y = -(playerSpeed.y - (playerSpeed.y * 0.05f));
				}
              }

            sf::Vector2f accelerationVector = playerState.direction * acceleration * gameState.getTickDelta().asSeconds();

			if (!isCollidingFloor) {
				accelerationVector += sf::Vector2f{ 0.0f, GravityConstant };
			}

			auto firstfloor = std::dynamic_pointer_cast<GameObject<FloorState>>(floorList[0]);
            if (isColliding(playerState, firstfloor->getState(), JumpIntentMargin)) {
                playerSpeed.y = -(playerSpeed.y - (playerSpeed.y * 0.05f)); // pseudo collision energy loss
            }

            playerSpeed -= playerSpeed * frictionFactor;

            playerSpeed += accelerationVector;
			
            if (std::fabs(playerSpeed.x) <= movementThreshold) {
                playerSpeed.x = 0;
            }
            if (std::fabs(playerSpeed.y) <= movementThreshold) {
                playerSpeed.y = 0;
            }
						
            if (playerState.wantJump && isCollidingFloor) {
                playerSpeed.y = -JumpSpeed;
                playerState.wantJump = false;
                playerState.didJump = true;
            }

			if (playerState.wantFall && isCollidingFloor) {
				playerSpeed.y = JumpSpeed;
				playerState.wantFall = false;
				playerState.didFall = true;
			}

            playerState.speed = playerSpeed;

            auto deltaPosition = playerSpeed * gameState.getTickDelta().asSeconds();

            playerState.position += deltaPosition;
			
        }
};