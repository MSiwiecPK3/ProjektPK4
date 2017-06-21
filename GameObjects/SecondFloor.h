#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"

struct SecFloorState : public BaseState
{
	sf::Vector2f position = { 0.0f, 300.0f };
	sf::Vector2f size = { 800.0f, 50.0f };
	sf::IntRect textureRect = { 0, 0, 800, 50 };
};

class SecFloorGraphicsComponent : public Component<SecFloorState> //component jest template
{
	sf::Texture texture;
	sf::Sprite sprite;

public:
	SecFloorGraphicsComponent()
	{
		texture.loadFromFile("Resources\\ground.png");
		texture.setRepeated(true);
		sprite.setTexture(texture);
		sprite.setPosition(0.0f, 300.0f);
		sprite.setTextureRect({ 0, 0, 800, 50 });
	}

	void update(BaseState &floorStateBase, GameState &gameState)
	{
		SecFloorState &secFloorState = reinterpret_cast<SecFloorState &>(floorStateBase);
		secFloorState.textureRect.left = secFloorState.textureRect.left + 1;

		sprite.setTextureRect(secFloorState.textureRect);

		gameState.render(sprite);
	}
};