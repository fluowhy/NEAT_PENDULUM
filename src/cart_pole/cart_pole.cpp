#include "cart_pole.h"


void dynamics(CartPole& cart_pole){
    float max_pos { static_cast<float>(config::width) * 0.5f };
    if (cart_pole.x > max_pos){
        cart_pole.x = max_pos;
        cart_pole.vx = 0;
        cart_pole.ax = (cart_pole.force > 0) ? 0 : cart_pole.force;
    }
    else if (cart_pole.x < - max_pos){
        cart_pole.x = - max_pos;
        cart_pole.vx = 0;
        cart_pole.ax = (cart_pole.force < 0) ? 0 : cart_pole.force;
    }
    else{
        cart_pole.ax = (cart_pole.force + cart_pole.m * cart_pole.l * (cart_pole.w_dot * std::cos(cart_pole.angle) - std::sin(cart_pole.angle) * cart_pole.w * cart_pole.w)) / (cart_pole.M + cart_pole.m);
    }
        
    cart_pole.w_dot = (config::g * std::sin(cart_pole.angle) + cart_pole.ax * std::cos(cart_pole.angle)) / cart_pole.l;

    cart_pole.ax += - config::gamma * cart_pole.vx;
    cart_pole.w_dot += - config::gamma * cart_pole.w;

    cart_pole.w += cart_pole.w_dot * config::dt;
    cart_pole.angle += cart_pole.w * config::dt;

    cart_pole.vx += cart_pole.ax * config::dt;
    cart_pole.x += cart_pole.vx * config::dt;
}


void CartPole::update(){
    rot = angle - angle_ant;
    body.setPosition({ x + static_cast<float>(config::offset_x), static_cast<float>(config::offset_y) });
    arm.setPosition({ x + static_cast<float>(config::offset_x), static_cast<float>(config::offset_y) });
    arm.rotate(sf::radians(rot));
    angle_ant = angle;
}


void CartPole::set(){
    body.setFillColor(sf::Color(0, 255, 0));
    body.setSize({ config::cart::width, config::cart::height });
    body.setOrigin({ config::cart::width * 0.5f, config::cart::height * 0.5f });

    arm.setFillColor(sf::Color(0, 128, 0));
    arm.setSize({ config::cart::pole_width, config::cart::pole_length });
    arm.setOrigin({ config::cart::pole_width * 0.5f, 0 });
}
