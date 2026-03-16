#include "display.h"

template <typename T>
void shift_and_insert(std::vector<T>& x, T value){
    // shifts vector in one position and insert value in index 0
    int shift { 1 };
    std::move(begin(x), end(x) - shift, begin(x) + shift);
    std::fill(begin(x), begin(x) + shift, value);
}

Probe::Probe(const std::string& title, int w, int l, int xo, int yo, float tw, float dt, float yr, sf::Font font)
    : my_title { title }
    , x_size { w }
    , y_size { l }
    , x_offset { xo }
    , y_offset { yo }
    , temporal_window { tw }
    , sampling_period { dt }
    , y_range { yr }
    , my_font { font }
    {

    n_samples = static_cast<float>(static_cast<int>(temporal_window / sampling_period));
    values.resize(n_samples);
    lines.resize(n_samples);
    scale_factor = static_cast<float>(y_size) / yr;

    for (int i{ 0 }; i<n_samples; ++i){
        lines[i].setSize({ static_cast<float>(x_size) / n_samples, 2 });
        lines[i].setFillColor(sf::Color{ 0xE97F4AFF });
        lines[i].setPosition({ x_offset + x_size * (1 - static_cast<float>(i + 1) / static_cast<float>(n_samples)), y_offset + static_cast<float>(y_size) * 0.5f - values[i] * scale_factor});
    }

    std::fill(values.begin(), values.end(), 0.f);

    rectangle.setSize({ static_cast<float>(x_size), static_cast<float>(y_size) });
    rectangle.setFillColor(sf::Color{ 0xE2E2E2FF });
    rectangle.setPosition({ static_cast<float>(x_offset), static_cast<float>(y_offset) });

    lim_inf = std::format("{:.1f}", - y_range * 0.5f);
    lim_sup = std::format("{:.1f}", + y_range * 0.5f);

    t0_text = std::format("{:.1f}", 0.f);
    tf_text = std::format("{:.1f}", tw);
}

void Probe::update(float value, float time){
    shift_and_insert(values, value);
    for (int i{ 0 }; i<n_samples; ++i){
        lines[i].setPosition({ x_offset + x_size * (1 - static_cast<float>(i + 1) / static_cast<float>(n_samples)), y_offset + static_cast<float>(y_size) * 0.5f - values[i] * scale_factor});
    }
}

void Probe::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Text my_text(my_font, my_title, 18);
    my_text.setPosition({ static_cast<float>(x_offset), static_cast<float>(y_offset) - 20 });
    my_text.setFillColor(sf::Color{ 0x1C0F13FF });

    sf::Text linf(my_font, lim_inf, 10);
    linf.setPosition({ static_cast<float>(x_offset) - 20, static_cast<float>(y_offset) + y_size - 10});
    linf.setFillColor(sf::Color{ 0x1C0F13FF });

    sf::Text lsup(my_font, lim_sup, 10);
    lsup.setPosition({ static_cast<float>(x_offset) - 20, static_cast<float>(y_offset) });
    lsup.setFillColor(sf::Color{ 0x1C0F13FF });

    sf::Text tini(my_font, t0_text, 10);
    tini.setPosition({ static_cast<float>(x_offset) + x_size, static_cast<float>(y_offset) + y_size });
    tini.setFillColor(sf::Color{ 0x1C0F13FF });

    sf::Text tfin(my_font, tf_text, 10);
    tfin.setPosition({ static_cast<float>(x_offset), static_cast<float>(y_offset) + y_size });
    tfin.setFillColor(sf::Color{ 0x1C0F13FF });

    target.draw(rectangle, states);
    target.draw(my_text, states);
    target.draw(linf, states);
    target.draw(lsup, states);
    target.draw(tini, states);
    target.draw(tfin, states);
    for (auto& rec : lines){
        target.draw(rec);
    }
}
