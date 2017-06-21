#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"

struct LeftSpikeState : public BaseState
{
	sf::Vector2f position = { 0.f, 0.0f };
	sf::Vector2f size = { 20.0f, 400.0f };
	sf::IntRect textureRect = { 0, 0, 20, 450 };
	sf::Sprite *sprite;
};


class LeftSpikeGraphicsComponent : public Component<LeftSpikeState> //component jest template
{
	sf::Texture texture;
	sf::Sprite sprite;

public:
	LeftSpikeGraphicsComponent()
	{
		texture.loadFromFile("Resources\\leftspikes.png");
		sprite.setTexture(texture);
		sprite.setPosition(0.f, 0.0f);
		sprite.setTextureRect({ 0, 0, 20, 450 });

	}

	void update(BaseState &leftSpikeStateBase, GameState &gameState)
	{
		LeftSpikeState &leftSpikeState = reinterpret_cast<LeftSpikeState &>(leftSpikeStateBase);
		leftSpikeState.sprite = &sprite;
		gameState.render(sprite);
	}
};