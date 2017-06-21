#include "Logic.h"
#include "Input.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <thread>

const sf::Time LOGIC_TICK_DELTA = sf::milliseconds(10);

Input input;

void updateInput(sf::Window &window) {
	sf::Event event;

    if (input.jump == KeyState::PRESSED) {
        input.jump = KeyState::DOWN;
    }
    else if (input.jump == KeyState::RELEASED) {
        input.jump = KeyState::UP;
    }

    if (input.fall == KeyState::PRESSED) {
        input.fall = KeyState::DOWN;
    }
    else if (input.fall == KeyState::RELEASED) {
        input.fall = KeyState::UP;
    }

    if (input.left == KeyState::PRESSED) {
        input.left = KeyState::DOWN;
    }
    else if (input.left == KeyState::RELEASED) {
        input.left = KeyState::UP;
    }

    if (input.right == KeyState::PRESSED) {
        input.right = KeyState::DOWN;
    }
    else if (input.right == KeyState::RELEASED) {
        input.right = KeyState::UP;
    }

    if (input.yes == KeyState::PRESSED) {
        input.yes = KeyState::DOWN;
    }
    else if (input.yes == KeyState::RELEASED) {
        input.yes = KeyState::UP;
    }

    if (input.no == KeyState::PRESSED) {
        input.no = KeyState::DOWN;
    }
    else if (input.no == KeyState::RELEASED) {
        input.no = KeyState::UP;
    }

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
                case sf::Keyboard::Up:    input.jump = KeyState::PRESSED; break;
                case sf::Keyboard::Down:  input.fall = KeyState::PRESSED; break;
                case sf::Keyboard::Left:  input.left = KeyState::PRESSED; break;
                case sf::Keyboard::Right: input.right = KeyState::PRESSED; break;
                case sf::Keyboard::Y:	  input.yes = KeyState::PRESSED; break;
                case sf::Keyboard::N:     input.no = KeyState::PRESSED; break;
			}
		}
		else if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code) {
                case sf::Keyboard::Up:    input.jump = KeyState::RELEASED; break;
                case sf::Keyboard::Down:  input.fall = KeyState::RELEASED; break;
                case sf::Keyboard::Left:  input.left = KeyState::RELEASED; break;
                case sf::Keyboard::Right: input.right = KeyState::RELEASED; break;
                case sf::Keyboard::Y:	  input.yes = KeyState::RELEASED; break;
                case sf::Keyboard::N:     input.no = KeyState::RELEASED; break;
			}
        }
	}
}

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 500, 32), "KNIGHTSCROLLER");
	srand(time(NULL));
	sf::Clock clock;
	Logic logic(LOGIC_TICK_DELTA);

	while (window.isOpen()) {
		sf::Time delta = clock.restart();
		
        auto shouldGameRun = logic.update(delta, [&]{ updateInput(window); return input; }); 
        //wywo³ywany jest update logiki z parametrem zwróconym przez updateInput czyli tym co u¿ytkownik nacisn¹³
        if (!shouldGameRun) {
            window.close();
            break;
        }
		logic.render(window);		
	}
    return 0;
}