#include "config.h"
#include "cart_pole.h"
#include "random.h"
#include "events.h"
#include <SFML/Graphics.hpp>
#include <iostream>


void print_state(CartPole& cart_pole){
    std::cout << "x: " << cart_pole.x << ", vx: " << cart_pole.vx << ", ax: " << cart_pole.ax << ", ang: " << cart_pole.angle << ", w: " << cart_pole.w << ", dot_w: " << cart_pole.w_dot << "\n";
}

int main(){
    sf::RenderWindow window(sf::VideoMode({ config::window_size_x, config::window_size_y }), "PuckWorld");
    window.setFramerateLimit(config::frame_rate);
    window.setVerticalSyncEnabled(true);

    CartPole cart_pole {};
    cart_pole.set();

    sf::RectangleShape rectangle({ config::width, config::height });
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition({ static_cast<float>(config::offset_x_rect), static_cast<float>(config::offset_y_rect) });
    

    while (window.isOpen()){
        cart_pole.force = 0;
        process_events(window, cart_pole);
        
        dynamics(cart_pole);
        cart_pole.update();
        print_state(cart_pole);
        
        // rectangle.setFillColor(sf::Color::Black);        
        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.draw(cart_pole.body);
        window.draw(cart_pole.arm);
        window.display();
    }

    
    return 0;
}
