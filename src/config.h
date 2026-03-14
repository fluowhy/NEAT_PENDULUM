#pragma once

namespace config{
    const int window_size_x { 1000 };
    const int window_size_y { 618 };
    const int frame_rate { 60 };
    const float dt { 1.f / static_cast<float>(frame_rate) }; 
    const int width { window_size_x - 20 };
    const int height { window_size_y - 20 };
    const int offset_x { window_size_x / 2 };
    const int offset_y { window_size_y / 2 };
    const float g { 9.81 };
    const float gamma { 1e-3 };
    const float max_force { 128 };
    const int offset_x_rect { (window_size_x - width) / 2  };
    const int offset_y_rect { (window_size_y - height) / 2 };
    const float max_sim_time { 40 };
    const int n_pop { 1024 };
    const int n_gen { 32 };
    const int n_in { 4 };
    const int n_out { 1 };
    const float ang_thr_deg { 8.f };
    const int max_fitness { 100 };

    namespace cart{
        const float width { 40 };
        const float height { 20 };
        const float pole_width { 4 };
        const float pole_length { 100 };
    }
}
