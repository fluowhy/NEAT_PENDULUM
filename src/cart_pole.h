#pragma once

#include "config.h"
#include <SFML/Graphics.hpp>


struct CartPole{
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

    void update(){
        rot = angle - angle_ant;
        body.setPosition({ x + static_cast<float>(config::offset_x), static_cast<float>(config::offset_y) });
        arm.setPosition({ x + static_cast<float>(config::offset_x), static_cast<float>(config::offset_y) });
        arm.rotate(sf::radians(rot));
        angle_ant = angle;
    }

    void set(){
        body.setFillColor(sf::Color(0, 255, 0));
        body.setSize({ config::cart::width, config::cart::height });
        body.setOrigin({ config::cart::width * 0.5f, config::cart::height * 0.5f });

        arm.setFillColor(sf::Color(0, 128, 0));
        arm.setSize({ config::cart::pole_width, config::cart::pole_length });
        arm.setOrigin({ config::cart::pole_width * 0.5f, 0 });
    }
};
