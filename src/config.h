#pragma once

namespace config{
    const int window_size_x { 900 };
    const int window_size_y { 400 };
    const int frame_rate { 120 };
    const float dt { 1.f / static_cast<float>(frame_rate) }; 
    const int width { 800 };
    const int height { 300 };
    const int offset_x { window_size_x / 2 };
    const int offset_y { window_size_y / 2 };
    const float g { 9.81 };
    const float gamma { 1e-2 };
    const float max_force { 512 };
    const int offset_x_rect { 50 };
    const int offset_y_rect { 50 };
    const float max_sim_time { 10 };
    namespace cart{
        const float width { 40 };
        const float height { 20 };
        const float pole_width { 4 };
        const float pole_length { 100 };
    }
}
