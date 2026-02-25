#include "config.h"
#include "cart_pole.h"
#include "random.h"
#include "events.h"
#include <SFML/Graphics.hpp>
#include <iostream>


int main(){
    sf::RenderWindow window(sf::VideoMode({ config::window_size_x, config::window_size_x }), "PuckWorld");
    window.setFramerateLimit(config::frame_rate);
    window.setVerticalSyncEnabled(true);

    CartPole cart_pole {};
    cart_pole.set();

    sf::RectangleShape rectangle({ config::width, config::height });
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition({ static_cast<float>(config::offset_x), static_cast<float>(config::offset_y) });

    

    while (window.isOpen()){
        process_events(window, cart_pole);
        
        cart_pole.update();   
        
        rectangle.setFillColor(sf::Color::Black);        
        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.draw(cart_pole.body);
        window.display();
    }

    
    return 0;
}
