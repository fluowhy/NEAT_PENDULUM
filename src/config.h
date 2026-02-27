#pragma once

namespace config{
    const int window_size_x { 900 };
    const int window_size_y { 900 };
    const int frame_rate { 60 };
    const float dt { 1.f / static_cast<float>(frame_rate) }; 
    const int width { 800 };
    const int height { 800 };
    const int offset_x {(window_size_x - width) / 2};
    const int offset_y {(window_size_y - height) / 2};
    const float g { 9.81 };
    const float gamma { 1e-1 };
    const float max_force { 100 };
    namespace cart{
        const float width { 40 };
        const float height { 20 };
        const float pole_width { 2 };
        const float pole_length { 100 };
    }
}
