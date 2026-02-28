#include "config.h"
#include "neat.h"
#include "cart_pole.h"
#include "random.h"
#include "events.h"
#include <SFML/Graphics.hpp>
#include <iostream>


std::vector<float> get_nn_input(CartPole& cart_pole){
    std::vector<float> input_array(config::n_in, 0);
    input_array[0] = cart_pole.x;
    input_array[1] = cart_pole.angle;
    input_array[2] = cart_pole.vx;
    input_array[3] = cart_pole.w;
    return input_array;
}


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

    std::vector<NEAT::Genome> genomes { NEAT::generate_initial_networks(config::n_pop, config::n_in, config::n_out) };
    std::vector<float> nn_input(config::n_in, 0);
    std::vector<float> nn_output(config::n_out, 0);
    NEAT::Genome& genome { genomes[0] };

    while (window.isOpen()){
        cart_pole.force = 0;
        process_events(window);
        // process_events_user_inputs(window, cart_pole);
        nn_input = get_nn_input(cart_pole);
        nn_output = NEAT::forward(nn_input, genomes[0]);
        cart_pole.force = nn_output[0] * config::max_force;

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
