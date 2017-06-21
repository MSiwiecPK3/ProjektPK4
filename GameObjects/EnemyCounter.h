#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <iostream>

#include "../GameObject.h"

struct EnemyCounterState : public BaseState
{
    int enemiesDeadCount = 0;
};

class EnemyCounterGraphicsComponent : public Component<EnemyCounterState>
{
        sf::Font font;
        sf::Text text;

    public:
        EnemyCounterGraphicsComponent()
        {
             if(!font.loadFromFile("Resources\\Space Comics.ttf")) {
                throw new std::runtime_error("Couldn't load font");
            }

            text.setFont(font);
            text.setString("ENEMIES : 0");
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Black);
            text.setPosition({ 40.0f, 40.0f });
        }

        void update(BaseState &enemyCounterStateBase, GameState &gameState)
        {
            EnemyCounterState &enemyCounterState = reinterpret_cast<EnemyCounterState &>(enemyCounterStateBase);

            text.setString("ENEMIES : " + std::to_string(enemyCounterState.enemiesDeadCount));
            gameState.render(text);
        }
};
