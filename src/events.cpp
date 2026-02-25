#include "events.h"
#include <iostream>

void process_events(sf::Window& window, CartPole& cart_pole){
    while (const std::optional event = window.pollEvent()){
        if (event->is<sf::Event::Closed>()){
            window.close();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
            window.close();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
            cart_pole.ax = -1;
            std::cout << "Left\n";
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
            cart_pole.ax = 1;
            std::cout << "Right\n";
        }
    }
}
