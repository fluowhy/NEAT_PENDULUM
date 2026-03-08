#pragma once

#include "random.h"
#include "config.h"
#include "cmath"
#include <SFML/Graphics.hpp>


struct CartPole{
    // float x { Random::random_normal<float>(0, 10) };
    float x { 0 };
    float vx { 0 };
    float ax { 0 };
    float angle_ant { -3.1416 };
    float angle { -3.1416 };
    float w { 0 };
    float w_dot { 0 };
    float force { 0 };
    float M { 1e-1 };
    float m { 1e-3};
    float l { config::cart::pole_length };
    float rot {};

    sf::RectangleShape body{};
    sf::RectangleShape arm{};

    void update();

    void set();    
};


void dynamics(CartPole& cart_pole);
