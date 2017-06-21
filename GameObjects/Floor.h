#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"

struct FloorState : public BaseState
{
    sf::Vector2f position = { 0.0f, 450.0f };
    sf::Vector2f size = { 800.0f, 100.0f };
    sf::IntRect textureRect = { 0, 0, 800, 100 };
};

class FloorGraphicsComponent : public Component<FloorState> 
{
        sf::Texture texture;
        sf::Sprite sprite;

    public:
        FloorGraphicsComponent() 
        {
            texture.loadFromFile("Resources\\ground.png");
            texture.setRepeated(true);
            sprite.setTexture(texture);
            sprite.setPosition(0.0f, 450.0f);
            sprite.setTextureRect({ 0, 0, 800, 100 });
        }

        void update(BaseState &floorStateBase, GameState &gameState)
        {
            FloorState &floorState = reinterpret_cast<FloorState &>(floorStateBase);
            floorState.textureRect.left = floorState.textureRect.left + 1;		//ciagle tworzenie tekstury

            sprite.setTextureRect(floorState.textureRect);

            gameState.render(sprite);
        }
};