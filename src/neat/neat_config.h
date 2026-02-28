#pragma once

namespace neat_config{
    namespace mut{
        const float new_value_prob { 0.2f };
        const float new_conn_prob { 0.8f };
        const float new_node_prob { 0.05f };
        const int n_mut { 4 };
        const float bias_mod_prob { 0.5f };
        const float mutate_genome_prob { 0.8f };
        const float bias_disable_prob { 0.5f };
    }
    const int max_nodes { 4 };
    const float top_ratio { 0.3f };
    const int n_gen { 1024 };
    const int n_pop { 256 };
    const int n_samples { 128 };
    const float sample_std { 1e-1 };
}
