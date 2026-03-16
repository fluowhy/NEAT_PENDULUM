#include "config.h"
#include "display.h"
#include "neat.h"
#include "cart_pole.h"
#include "random.h"
#include "events.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>


std::mutex mutex;


template <typename T>
void print_vector(std::vector<T>& v){
    for (T vi : v){
        std::cout << vi << ", ";
    }
    std::cout << "\n";
}





float deg_to_rad(float deg){
    return deg * 3.1416f / 180.f;
}


float get_reward(CartPole& cart_pole){
    float reward { 0.f };
    if (std::abs(cart_pole.angle) < deg_to_rad(config::ang_thr_deg)){
        reward = 2.f * config::dt / (std::abs(cart_pole.w) + 1);
    }
    if (std::abs(cart_pole.x) < 10){
        reward += 0.5f * config::dt;
    }
    if (std::abs(cart_pole.x) >= 10){
        reward -= 0.1f * config::dt;
    }
    // if (std::abs(cart_pole.x) > 100){
    //     reward -= 0.5f;
    // }
    // reward -= std::abs(cart_pole.x) / (static_cast<float>(config::width)) * config::dt;
    // reward -= std::abs(cart_pole.w) * config::dt * 0.5f;
    return reward;
}


std::vector<float> get_nn_input(CartPole& cart_pole){
    std::vector<float> input_array(config::n_in, 0);
    input_array[0] = cart_pole.x / (static_cast<float>(config::width) * 0.5f);
    input_array[1] = cart_pole.angle;
    // input_array[1] = std::cos(cart_pole.angle);
    // input_array[2] = std::sin(cart_pole.angle);
    input_array[2] = cart_pole.vx / (static_cast<float>(config::width) * 0.5f);
    input_array[3] = cart_pole.w;
    return input_array;
}


void print_state(CartPole& cart_pole){
    std::cout << "x: " << cart_pole.x << ", vx: " << cart_pole.vx << ", ax: " << cart_pole.ax << ", ang: " << cart_pole.angle << ", w: " << cart_pole.w << ", dot_w: " << cart_pole.w_dot << "\n";
}


void simulate(NEAT::Genome& genome, std::vector<float>& fitness_array, int idx){
    CartPole cart_pole {};
    cart_pole.set();
    
    std::vector<float> nn_input(config::n_in, 0);
    std::vector<float> nn_output(config::n_out, 0);
    float fitness { 0 };
    float sim_time { config::max_sim_time };
    float xpos { std::abs(cart_pole.x) };

    while ((sim_time > 0) & (static_cast<int>(fitness) < config::max_fitness) & (xpos < static_cast<float>(config::width) * 0.5f)){
        cart_pole.force = 0;

        nn_input = get_nn_input(cart_pole);
        nn_output = NEAT::forward(nn_input, genome);
        cart_pole.force = nn_output[0] * config::max_force;

        dynamics(cart_pole);
        cart_pole.update();
        // print_state(cart_pole);
        fitness += get_reward(cart_pole);
        sim_time -= config::dt;
        xpos = std::abs(cart_pole.x);
    }

    // float fitness { std::reduce(rewards.begin(), rewards.end()) };
    std::unique_lock<std::mutex> lock(mutex);
    fitness_array[idx] = fitness;
}


float simulate_display(NEAT::Genome& genome){
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({ config::window_size_x, config::window_size_y }), "Cart Pole", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(config::frame_rate);
    window.setVerticalSyncEnabled(true);

    CartPole cart_pole {};
    cart_pole.set();

    // font
    sf::Font font;
    if (!font.openFromFile("fonts/Roboto.ttf")){
        std::cerr << "Couldn't load font\n";
    }

    sf::RectangleShape rectangle({ config::width, config::height });
    rectangle.setFillColor(sf::Color{ 0xFFF8DEFF });
    rectangle.setPosition({ static_cast<float>(config::offset_x_rect), static_cast<float>(config::offset_y_rect) });

    std::vector<float> nn_input(config::n_in, 0);
    std::vector<float> nn_output(config::n_out, 0);
    float fitness { 0 };

    // std::vector<float> x_pos(5, 0);
    // std::vector<float> probe_time(5, 0);
    int probe_window_length { 100 };
    int probe_window_width { 200 };
    int xoff { 40 };
    int xdelta { probe_window_width + 30 };
    int yoff { config::window_size_y - config::offset_y_rect - 2 * (30 + probe_window_length) };
    int ydelta { probe_window_length + 30 };
    Probe x_probe   { "position", probe_window_width, probe_window_length, xoff, yoff + ydelta, 4.f, config::dt, config::width, font };
    Probe ang_probe { "angle", probe_window_width, probe_window_length, xoff + xdelta, yoff + ydelta, 4.f, config::dt, 6.4f, font };
    Probe v_probe   { "velocity", probe_window_width, probe_window_length, xoff + 2 * xdelta, yoff + ydelta, 4.f, config::dt, 800.f, font };
    Probe w_probe   { "angular velocity", probe_window_width, probe_window_length, xoff + 3 * xdelta, yoff + ydelta, 4.f, config::dt, 10.f, font };
    Probe f_probe   { "force", probe_window_width, probe_window_length, xoff, yoff, 4.f, config::dt, config::max_force * 2, font };
    sf::Clock clock;

    while (window.isOpen()){
        cart_pole.force = 0;        
        // process_events_user_inputs(window, cart_pole);
        nn_input = get_nn_input(cart_pole);
        nn_output = NEAT::forward(nn_input, genome);
        cart_pole.force = nn_output[0] * config::max_force;
        // process_events(window);
        process_events_user_inputs(window, cart_pole);
        // std::cout << "Force: " << cart_pole.force << " N\n";

        dynamics(cart_pole);
        cart_pole.update();
        // print_state(cart_pole);
        // shift_and_insert(x_pos, cart_pole.x);
        // print_vector<float>(x_pos);
        fitness += get_reward(cart_pole);

        x_probe.update(cart_pole.x, clock.getElapsedTime().asSeconds());
        ang_probe.update(cart_pole.angle, clock.getElapsedTime().asSeconds());
        v_probe.update(cart_pole.vx, clock.getElapsedTime().asSeconds());
        w_probe.update(cart_pole.w, clock.getElapsedTime().asSeconds());
        f_probe.update(cart_pole.force, clock.getElapsedTime().asSeconds());
        // rectangle.setFillColor(sf::Color::Black);        
        window.clear(sf::Color{ 0x1C0F13FF });
        window.draw(rectangle);
        window.draw(cart_pole.body);
        window.draw(cart_pole.arm);
        window.draw(x_probe);
        window.draw(ang_probe);
        window.draw(v_probe);
        window.draw(w_probe);
        window.draw(f_probe);
        window.display();
    }
    return fitness;
}


int main(){
    std::vector<NEAT::Genome> new_genomes { NEAT::generate_initial_networks(config::n_pop, config::n_in, config::n_out) };
    std::vector<NEAT::Genome> genomes {};
    std::vector<float> fitness(config::n_pop, 0);
    int n_threads = std::thread::hardware_concurrency();
    std::cout << "Max number of threads: " << n_threads << "\n";
    std::vector<std::thread> thread_pool {};
    int thread_counter { 0 };

    float sim_reward {};
    std::vector<int> sort_index {};

    for (int gen {0}; gen<config::n_gen; ++gen){
        std::cout << "Generation: " << gen << "\n";
        genomes.clear();
        // thread_pool.clear();
        genomes = new_genomes;
        for (int i {0}; i<config::n_pop; ++i){
            // sim_reward = simulate(genomes[i], fitness, i);
            // fitness[i] = sim_reward;
            thread_pool.push_back(std::thread(simulate, std::ref(genomes[i]), std::ref(fitness), i));
            thread_counter += 1;
            if (thread_counter >= n_threads){
                // std::cout << "Joining threads.\n";
                for (int j {0}; j<n_threads; ++j){
                    thread_pool[j].join();
                }
                thread_counter = 0;
                thread_pool.clear();
            }
        }        

        sort_index = NEAT::get_sorted_index(fitness);
        std::cout << "Max fitness: " << fitness[sort_index[0]] << "\n";
        // print_vector(fitness);
        new_genomes = NEAT::get_next_population(genomes, sort_index);
    }   

    sim_reward = simulate_display(genomes[sort_index[0]]);
    std::cout << "Reward: " << sim_reward << "\n";

    return 0;
}
