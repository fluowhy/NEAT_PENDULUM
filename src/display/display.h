#pragma once

#include <format>
#include <SFML/Graphics.hpp>

template <typename T>
void shift_and_insert(std::vector<T>& x, T value);

class Probe: public sf::Drawable{
    public:
        Probe(const std::string& title, int w, int l, int xo, int yo, float tw, float dt, float yr, sf::Font font);

        void update(float value, float time);

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        std::string my_title;
        int x_size {};
        int y_size {};
        int x_offset {};
        int y_offset {};
        sf::RectangleShape rectangle {};
        std::vector<float> values {};
        float temporal_window {};
        float sampling_period {};
        int n_samples {};
        float n_div {};
        std::vector<sf::RectangleShape> lines {};
        float scale_factor {};
        float y_range {};
        sf::Font my_font;
        std::string lim_inf;
        std::string lim_sup;
        std::string t0_text;
        std::string tf_text;
    };
