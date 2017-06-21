#pragma once

#include <SFML/Graphics.hpp>

#include "../GameObject.h"

struct GameBackgroundState : public BaseState
{
    
};

class GameBackgroundGraphicsComponent : public Component<GameBackgroundState>
{
    sf::Texture texture;
    sf::Sprite sprite;

public:
    GameBackgroundGraphicsComponent()
    {
        texture.loadFromFile("Resources\\backgroundgame.png");
        sprite.setTexture(texture);
    }

    void update(BaseState &backgroundStateBase, GameState &gameState)
    {
        gameState.render(sprite);
    }
};