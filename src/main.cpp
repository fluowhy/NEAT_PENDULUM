#include "config.h"
#include "neat.h"
#include "cart_pole.h"
#include "random.h"
#include "events.h"
#include <SFML/Graphics.hpp>
#include <iostream>


float deg_to_rad(float deg){
    return deg * 3.1416f / 180.f;
}


float get_reward(CartPole& cart_pole){
    float reward { 0.f };
    if (std::abs(cart_pole.angle) < deg_to_rad(config::ang_thr_deg)){
        reward = 1.f;
    }
    if (std::abs(cart_pole.x) > 100){
        reward -= 0.5f;
    }
    return reward;
}


template <typename T>
void print_vector(std::vector<T>& v){
    for (T vi : v){
        std::cout << vi << ", ";
    }
    std::cout << "\n";
}


std::vector<float> get_nn_input(CartPole& cart_pole){
    std::vector<float> input_array(config::n_in, 0);
    input_array[0] = cart_pole.x / (static_cast<float>(config::width) * 0.5f);
    input_array[1] = cart_pole.angle;
    input_array[2] = cart_pole.vx / (static_cast<float>(config::width) * 0.5f);
    input_array[3] = cart_pole.w;
    return input_array;
}


void print_state(CartPole& cart_pole){
    std::cout << "x: " << cart_pole.x << ", vx: " << cart_pole.vx << ", ax: " << cart_pole.ax << ", ang: " << cart_pole.angle << ", w: " << cart_pole.w << ", dot_w: " << cart_pole.w_dot << "\n";
}


float simulate(NEAT::Genome& genome){
    CartPole cart_pole {};
    cart_pole.set();

    std::vector<float> nn_input(config::n_in, 0);
    std::vector<float> nn_output(config::n_out, 0);
    std::vector<float> rewards {};
    float sim_time { config::max_sim_time };

    while (sim_time > 0){
        cart_pole.force = 0;

        nn_input = get_nn_input(cart_pole);
        nn_output = NEAT::forward(nn_input, genome);
        cart_pole.force = nn_output[0] * config::max_force;

        dynamics(cart_pole);
        cart_pole.update();
        // print_state(cart_pole);
        rewards.push_back(get_reward(cart_pole));
        sim_time -= config::dt;
    }
    return std::reduce(rewards.begin(), rewards.end());
}


float simulate(sf::RenderWindow& window, NEAT::Genome& genome){
    CartPole cart_pole {};
    cart_pole.set();

    sf::RectangleShape rectangle({ config::width, config::height });
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition({ static_cast<float>(config::offset_x_rect), static_cast<float>(config::offset_y_rect) });

    std::vector<float> nn_input(config::n_in, 0);
    std::vector<float> nn_output(config::n_out, 0);
    std::vector<float> rewards {};
    float sim_time { config::max_sim_time };

    while (window.isOpen()){
        cart_pole.force = 0;
        process_events(window);
        // process_events_user_inputs(window, cart_pole);
        nn_input = get_nn_input(cart_pole);
        nn_output = NEAT::forward(nn_input, genome);
        cart_pole.force = nn_output[0] * config::max_force;

        dynamics(cart_pole);
        cart_pole.update();
        // print_state(cart_pole);
        rewards.push_back(get_reward(cart_pole));
        sim_time -= config::dt;

        // rectangle.setFillColor(sf::Color::Black);        
        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.draw(cart_pole.body);
        window.draw(cart_pole.arm);
        window.display();

        if (sim_time < 0){
            window.close();
            return std::reduce(rewards.begin(), rewards.end());
        }
    }
    return std::reduce(rewards.begin(), rewards.end());
}


int main(){
    std::vector<NEAT::Genome> new_genomes { NEAT::generate_initial_networks(config::n_pop, config::n_in, config::n_out) };
    std::vector<NEAT::Genome> genomes {};
    std::vector<float> fitness(config::n_pop, 0);

    float sim_reward {};
    std::vector<int> sort_index {};

    for (int gen {0}; gen<config::n_gen; ++gen){
        std::cout << "Generation: " << gen << "\n";
        genomes.clear();
        genomes = new_genomes;
        for (int i {0}; i<config::n_pop; ++i){
            sim_reward = simulate(genomes[i]);
            fitness[i] = sim_reward;
            
        }
        sort_index = NEAT::get_sorted_index(fitness);
        std::cout << "Max fitness: " << fitness[sort_index[0]] << "\n";
        // print_vector(fitness);
        new_genomes = NEAT::get_next_population(genomes, sort_index);
    }


    sf::RenderWindow window(sf::VideoMode({ config::window_size_x, config::window_size_y }), "Cart Pole");
    window.setFramerateLimit(config::frame_rate);
    window.setVerticalSyncEnabled(true);

    sim_reward = simulate(window, genomes[sort_index[0]]);
    std::cout << "Reward: " << sim_reward << "\n";

    return 0;
}
