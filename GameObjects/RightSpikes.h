#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"

struct RightSpikeState : public BaseState
{
	sf::Vector2f position = { 840.0f, 0.0f };
	sf::Vector2f size = { 10.0f, 400.0f };
	sf::IntRect textureRect = { 0, 0, 10, 450 };
	sf::Sprite *sprite;
};


class RightSpikeGraphicsComponent : public Component<RightSpikeState> //component jest template
{
	sf::Texture texture;
	sf::Sprite sprite;

public:
	RightSpikeGraphicsComponent()
	{
		texture.loadFromFile("Resources\\rightspikes.png");
		sprite.setTexture(texture);
		sprite.setPosition(790.0f, 0.0f);
		sprite.setTextureRect({ 0, 0, 10, 450 });
	}

	void update(BaseState &rightSpikeStateBase, GameState &gameState)
	{
		RightSpikeState &rightSpikeState = reinterpret_cast<RightSpikeState &>(rightSpikeStateBase);
		rightSpikeState.sprite = &sprite;
		gameState.render(sprite);
	}
};