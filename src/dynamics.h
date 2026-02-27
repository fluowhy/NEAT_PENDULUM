#pragma once

#include "config.h"
#include "cart_pole.h"
#include "cmath"

void dynamics(CartPole& cart_pole){
    float max_pos { static_cast<float>(config::width) * 0.5f };
    if (cart_pole.x > max_pos){
        cart_pole.x = max_pos;
        cart_pole.vx = 0;
    }
    else if (cart_pole.x < - max_pos){
        cart_pole.x = - max_pos;
        cart_pole.vx = 0;
    }

    cart_pole.ax = (cart_pole.force + cart_pole.m * cart_pole.l * (cart_pole.w_dot * std::cos(cart_pole.angle) - std::sin(cart_pole.angle) * cart_pole.w * cart_pole.w)) / (cart_pole.M + cart_pole.m);
    cart_pole.w_dot = (config::g * std::sin(cart_pole.angle) + cart_pole.ax * std::cos(cart_pole.angle)) / cart_pole.l;

    cart_pole.ax += - config::gamma * cart_pole.vx;
    cart_pole.w_dot += - config::gamma * cart_pole.w;

    cart_pole.w += cart_pole.w_dot * config::dt;
    cart_pole.angle += cart_pole.w * config::dt;

    cart_pole.vx += cart_pole.ax * config::dt;
    cart_pole.x += cart_pole.vx * config::dt;
}
