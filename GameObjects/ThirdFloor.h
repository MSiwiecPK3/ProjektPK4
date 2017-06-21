#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"

struct ThirdFloorState : public BaseState
{
	sf::Vector2f position = { 0.0f, 150.0f };
	sf::Vector2f size = { 800.0f, 50.0f };
	sf::IntRect textureRect = { 0, 0, 800, 50 };
};

class ThirdFloorGraphicsComponent : public Component<ThirdFloorState> //component jest template
{
	sf::Texture texture;
	sf::Sprite sprite;

public:
	ThirdFloorGraphicsComponent()
	{
		texture.loadFromFile("Resources\\ground.png");
		texture.setRepeated(true);
		sprite.setTexture(texture);
		sprite.setPosition(0.0f, 150.0f);
		sprite.setTextureRect({ 0, 0, 800, 50 });
	}

	void update(BaseState &floorStateBase, GameState &gameState)
	{
		ThirdFloorState &thirdFloorState = reinterpret_cast<ThirdFloorState &>(floorStateBase);
		thirdFloorState.textureRect.left = thirdFloorState.textureRect.left + 1;

		sprite.setTextureRect(thirdFloorState.textureRect);

		gameState.render(sprite);
	}
};