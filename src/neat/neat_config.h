#pragma once

namespace neat_config{
    namespace mut{
        const float new_value_prob { 0.1f };
        const float new_conn_prob { 0.3f };
        const float new_node_prob { 0.05f };
        const float bias_mod_prob { 0.5f };
        const float mutate_genome_prob { 0.8f };
        const float bias_disable_prob { 0.5f };
    }
    const int max_nodes { 10 };
    const float top_ratio { 0.3f };
}
