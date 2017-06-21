#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <iostream>

#include "../Input.h"
#include "../GameObject.h"
#include "Floor.h"
#include "State.h"

struct MenuState : public BaseState
{

};

class MenuInputComponent : public Component<MenuState>
{
    public:
        void update(BaseState &menuStateBase, GameState &gameState)
        {
            MenuState &menuState = reinterpret_cast<MenuState &>(menuStateBase);

            if (gameState.getInput().yes == KeyState::RELEASED) { //reaguje tylko na klawisze Y i N
                gameState.pushState<PlayingGameState>();
            }
            else if (gameState.getInput().no == KeyState::RELEASED) {
                gameState.exitGame();
            }
        }
};

class MenuGraphicsComponent : public Component<MenuState>
{
        sf::Font font;
        sf::Text text;
        sf::Text bigText;
        sf::Texture texture;
        sf::Sprite backgroundSprite;

    public:
        MenuGraphicsComponent()
        {
            if(!font.loadFromFile("Resources\\Space Comics.ttf")) {
	            throw new std::runtime_error("Couldn't load font");
	        }
            
            if (!texture.loadFromFile("Resources\\backgroundmenu.png")) {
                throw new std::runtime_error("Couldn't load background");
            }
            backgroundSprite.setTexture(texture);
            backgroundSprite.setPosition(0, 0);
	        text.setFont(font);
	        text.setString("U ARE BRAVE KNIGHT,\n\nEVIL ORCS WANT TO KILL YOU!\n\nYOU HAVE TO RUN!\n\nAVOID SPIKES!!\n\nPLAY USING ARROWS!\n\nPRESS 'Y' TO PRAY GAME.\n\nPRESS 'N' TO KILL GAME.");
	        text.setCharacterSize(12);
	        text.setFillColor(sf::Color::Red);
	        text.setPosition({ 300.0f, 300.0f });

            bigText.setFont(font);
            bigText.setString("BRAVE KNIGHT\n\n       VS\n\nTHE ORCS");
            bigText.setCharacterSize(40);
            bigText.setFillColor(sf::Color::Black);
            bigText.setPosition({ 70.0f, 40.0f });
        }

        void update(BaseState &playerStateBase, GameState &gameState)
        {
            gameState.render(backgroundSprite);
            gameState.render(text);
            gameState.render(bigText);
        }
};