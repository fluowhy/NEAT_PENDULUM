#pragma once

#include "config.h"
#include <SFML/Graphics.hpp>


struct CartPole{
    float x { 0 };
    float vx { 0 };
    float ax { 0 };
    float angle { 180 };
    float w { 0 };

    sf::RectangleShape body{};
    sf::RectangleShape arm{};

    void update(){
        vx += ax * config::dt;
        x += vx * config::dt;
        body.setPosition({ x + static_cast<float>(config::offset_x), static_cast<float>(config::offset_y) });
        arm.setPosition({ x + static_cast<float>(config::offset_x), static_cast<float>(config::offset_y) });
    }

    void set(){
        body.setFillColor(sf::Color(0, 255, 0));
        body.setSize({ config::cart::width, config::cart::height });
        body.setOrigin({ config::cart::width * 0.5f, config::cart::height * 0.5f });

        arm.setFillColor(sf::Color(0, 128, 0));
        arm.setSize({ config::cart::pole_width, config::cart::pole_length });
        arm.setOrigin({ config::cart::pole_width, config::cart::height * 0.5f });
    }
};
