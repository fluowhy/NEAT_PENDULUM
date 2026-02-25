#include "config.h"
#include "random.h"
#include "events.h"
#include <SFML/Graphics.hpp>
#include <iostream>


int main(){
    sf::RenderWindow window(sf::VideoMode({ config::window_size_x, config::window_size_x }), "PuckWorld");
    window.setFramerateLimit(config::frame_rate);
    window.setVerticalSyncEnabled(true);

    sf::RectangleShape rectangle({ config::width, config::height });
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition({ static_cast<float>(config::offset_x), static_cast<float>(config::offset_y) });

    while (window.isOpen()){
        rectangle.setFillColor(sf::Color { Random::get(0, 255), Random::get(0, 255), Random::get(0, 255) });
        process_events(window);
        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.display();
    }

    
    return 0;
}
