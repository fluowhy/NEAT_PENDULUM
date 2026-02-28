#include "neat.h"


void NEAT::sort_nodes(NEAT::Genome& network){
    std::vector<NEAT::NodeGene> sorted_node_gene {};
    int n_genes { static_cast<int>(network.node_genes.size()) };
    for (int i {0}; i<n_genes; ++i){
        for (NEAT::NodeGene& gene : network.node_genes){
            if (gene.node_idx == i){
                sorted_node_gene.push_back(gene);
                break;
            }
        }
    }
    network.node_genes.clear();
    for (int i {0}; i<n_genes; ++i){
        network.node_genes.push_back(sorted_node_gene[i]);
    }
}


int NEAT::innovation_number { 0 };
std::vector<int> NEAT::species {};


void NEAT::add_connection_to_offspring(NEAT::Genome& offspring, NEAT::ConnectionGene& conn, NEAT::Genome& network){    
    // offspring.compute_adjacency_list();
    // offspring.sort_nodes();
    // if (NEAT::is_connection_valid(offspring.conn_genes, conn.in_node, conn.out_node, offspring.sorted_nodes)){
    //     offspring.add_node({conn.in_node, network.node_genes[conn.in_node].node_type});
    //     offspring.add_node({conn.out_node, network.node_genes[conn.out_node].node_type});
    //     offspring.add_connection(conn);
    // }
    offspring.add_node({conn.in_node, network.node_genes[conn.in_node].node_type, network.node_genes[conn.in_node].bias});
    offspring.add_node({conn.out_node, network.node_genes[conn.out_node].node_type, network.node_genes[conn.out_node].bias});
    offspring.add_connection(conn);
}


NEAT::Genome NEAT::crossover(Genome& network1, Genome& network2, float f1, float f2){
    int max_inn_num_1 { NEAT::get_max_innovation_number(network1) };
    int max_inn_num_2 { NEAT::get_max_innovation_number(network2) };
    int max_inn_num { std::max(max_inn_num_1, max_inn_num_2) };
    int min_inn_num { std::min(max_inn_num_1, max_inn_num_2) };
    std::vector<int> aligned_genomes_1 {};
    std::vector<int> aligned_genomes_2 {};
    
    for (int inn {0}; inn<=max_inn_num; ++inn){
        NEAT::add_gen_idx_to_aligned_vector(aligned_genomes_1, network1, inn);
        NEAT::add_gen_idx_to_aligned_vector(aligned_genomes_2, network2, inn);
    }

    std::vector<int> matched_state { NEAT::get_matched_state(aligned_genomes_1, aligned_genomes_2, max_inn_num, min_inn_num) };
    
    Genome offspring {};
    int idx {};
    ConnectionGene conn {};
    for (int i {0}; i<=max_inn_num; ++i){
        if (matched_state[i] == 0){
            // match between genes
            idx = Random::get(0, 1);
            if (idx == 0){
                conn = network1.conn_genes[aligned_genomes_1[i]];
                NEAT::add_connection_to_offspring(offspring, conn, network1);
            }
            else{
                conn = network2.conn_genes[aligned_genomes_2[i]];
                NEAT::add_connection_to_offspring(offspring, conn, network2);
            }
        }
        else if ((matched_state[i] == 1) || (matched_state[i] == 2)){
            if ((f1 > f2) && (aligned_genomes_1[i] != -99)){
                conn = network1.conn_genes[aligned_genomes_1[i]];
                NEAT::add_connection_to_offspring(offspring, conn, network1);
            }
            else if ((f2 > f1) && (aligned_genomes_2[i] != -99)){
                conn = network2.conn_genes[aligned_genomes_2[i]];
                NEAT::add_connection_to_offspring(offspring, conn, network2);
            }
            else if (f2 == f1){
                if (aligned_genomes_1[i] != -99){
                    conn = network1.conn_genes[aligned_genomes_1[i]];
                    NEAT::add_connection_to_offspring(offspring, conn, network1);
                }
                else if (aligned_genomes_2[i] != -99){
                    conn = network2.conn_genes[aligned_genomes_2[i]];
                    NEAT::add_connection_to_offspring(offspring, conn, network2);
                }
            }
        }
    }
    NEAT::sort_nodes(offspring);
    return offspring;
}


void NEAT::estimate_species_of_population(std::vector<NEAT::Genome>& population, std::vector<NEAT::Genome>& new_population, float thr){
    // delta = NEAT::compatibility_distance(networks[0], networks[1], 1, 1, 0.4);
    int counter { 0 };
    std::vector<int> species_pop_idx {};
    std::vector<int> species_network_idx {};

    for (int sp : NEAT::species){        
        counter = 0;
        species_pop_idx.clear();
        for (NEAT::Genome& network : population){
            if (network.species == sp){
                species_pop_idx.push_back(counter);
            }
            ++counter;
        }
        species_network_idx.push_back(Random::random_choice(species_pop_idx, 1)[0]);
    }

    float delta {};
    bool has_species {false};
    int net_counter {0};
    for (NEAT::Genome& network : new_population){
        has_species = false;
        for (int sp : NEAT::species){
            delta = NEAT::compatibility_distance(network, population[species_network_idx[sp - 1]], 1, 1, 0.4);
            if (delta < thr){
                network.species = sp;
                has_species = true;
                break;
            }
        }
        if (!has_species){
            NEAT::species.push_back(static_cast<int>(NEAT::species.size()) + 1);
            species_network_idx.push_back(net_counter);
        }
        ++net_counter;
    }    
}


int NEAT::get_max_innovation_number(NEAT::Genome& network){
    int max_inn_num { -1 };
    for (NEAT::ConnectionGene& conn : network.conn_genes){
        if (conn.innovation_num > max_inn_num){
            max_inn_num = conn.innovation_num;
        }
    }
    return max_inn_num;
}

void NEAT::add_gen_idx_to_aligned_vector(std::vector<int>& aligned_genomes, NEAT::Genome& network, int inn_num){
    bool found { false };
    int idx { 0 };
    for (NEAT::ConnectionGene& conn : network.conn_genes){
        if (inn_num == conn.innovation_num){
            found = true;
            aligned_genomes.push_back(idx);
            break;
        }
        ++idx;
    }
    if (!found){
        aligned_genomes.push_back(-99);
    }
}


std::vector<int> NEAT::get_matched_state(std::vector<int>& alig_gen_1, std::vector<int>& alig_gen_2, int max_inn_num, int min_inn_num){
    // match: 0
    // disjoint: 1
    // excess: 2
    // empty: 3
    std::vector<int> matched_state {};
    for (int inn {0}; inn<=max_inn_num; ++inn){
        if (inn <= min_inn_num){
            if ((alig_gen_1[inn] != -99) && (alig_gen_2[inn] != -99)){
                matched_state.push_back(0);
            }
            else if ((alig_gen_1[inn] == -99) && (alig_gen_2[inn] == -99)){
                matched_state.push_back(3);
            }
            else if ((alig_gen_1[inn] == -99) && (alig_gen_2[inn] != -99)){
                matched_state.push_back(1);
            }
            else if ((alig_gen_1[inn] != -99) && (alig_gen_2[inn] == -99)){
                matched_state.push_back(1);
            }
        }
        else{
            if ((alig_gen_1[inn] == -99) && (alig_gen_2[inn] == -99)){
                matched_state.push_back(3);
            }
            else if ((alig_gen_1[inn] == -99) && (alig_gen_2[inn] != -99)){
                matched_state.push_back(2);
            }
            else if ((alig_gen_1[inn] != -99) && (alig_gen_2[inn] == -99)){
                matched_state.push_back(2);
            }
        }
    }
    return matched_state;
}


float NEAT::sum_if_value(std::vector<int>& x, int value){
    float sum { 0 };
    for (int xi : x){
        if (xi == value){
            ++sum;
        }
    }
    return sum;
}


float NEAT::compatibility_distance(NEAT::Genome& network1, NEAT::Genome& network2, float c1, float c2, float c3){
    int max_inn_num_1 { NEAT::get_max_innovation_number(network1) };
    int max_inn_num_2 { NEAT::get_max_innovation_number(network2) };
    int max_inn_num { std::max(max_inn_num_1, max_inn_num_2) };
    int min_inn_num { std::min(max_inn_num_1, max_inn_num_2) };
    std::vector<int> aligned_genomes_1 {};
    std::vector<int> aligned_genomes_2 {};
    
    for (int inn {0}; inn<=max_inn_num; ++inn){
        NEAT::add_gen_idx_to_aligned_vector(aligned_genomes_1, network1, inn);
        NEAT::add_gen_idx_to_aligned_vector(aligned_genomes_2, network2, inn);
    }

    std::vector<int> matched_state { NEAT::get_matched_state(aligned_genomes_1, aligned_genomes_2, max_inn_num, min_inn_num) };

    std::vector<float> n_gen {};

    for (int i {0}; i<=3; ++i){
        n_gen.push_back(NEAT::sum_if_value(matched_state, i));
    }

    float w_bar { 0 };
    for (int i {0}; i<static_cast<int>(matched_state.size()); ++i){
        if (matched_state[i] == 0){
            w_bar += std::abs(network1.conn_genes[aligned_genomes_1[i]].weight - network2.conn_genes[aligned_genomes_2[i]].weight);
        }
    }
    // idx 0: matched genes
    w_bar = w_bar / n_gen[0];

    float n_exc { n_gen[2] };
    float n_dis { n_gen[1] };
    float n { std::max(static_cast<float>(network1.conn_genes.size()), static_cast<float>(network2.conn_genes.size())) };
    // excess, disjoint, avg weight diff
    float delta { c1 * n_exc / n + c2 * n_dis / n + c3 * w_bar };
    return delta;
}


std::vector<NEAT::ConnectionGene> NEAT::existing_connections {};


void NEAT::bias_mod(NEAT::Genome& network){
	int idx { Random::get(0, static_cast<int>(network.node_genes.size() - 1)) };
    if (Random::get_uniform<float>(0, 1) <= neat_config::mut::bias_disable_prob){
        network.node_genes[idx].bias = 0;
    }
    else{
        if (Random::get_uniform<float>(0, 1) <= neat_config::mut::new_value_prob){
		    network.node_genes[idx].bias = Random::random_normal<float>(0.f, 0.1f);
	    }
        else{
            network.node_genes[idx].bias += Random::random_normal<float>(0.f, 0.1f);
	    }
    }	
}


void NEAT::weight_mod(NEAT::Genome& network){
	int idx { Random::get(0, static_cast<int>(network.conn_genes.size() - 1)) };
	if (Random::get_uniform<float>(0, 1) <= neat_config::mut::new_value_prob){
		network.conn_genes[idx].weight = Random::random_normal<float>(0.f, 0.1f);
	}
	else{
		network.conn_genes[idx].weight += Random::random_normal<float>(0.f, 0.1f);
	}	
}


void NEAT::mutate(Genome& network){
    if (Random::get_uniform<float>(0, 1) <= neat_config::mut::mutate_genome_prob){
        if (Random::get_uniform<float>(0, 1) <= neat_config::mut::bias_mod_prob){
            bias_mod(network);
        }
        else{
            weight_mod(network);
        }
    }
    
    if (Random::get_uniform<float>(0, 1) <= neat_config::mut::new_conn_prob){
        NEAT::add_connection(network);
    }
    if (Random::get_uniform<float>(0, 1) <= neat_config::mut::new_node_prob){
        NEAT::add_node(network);
    }
    network.species = 0;
    network.compute_adjacency_list();
    network.sort_nodes();
}


std::vector<NEAT::Genome> NEAT::get_next_population(std::vector<NEAT::Genome>& population, std::vector<int>& sort_index){
    std::vector<NEAT::Genome> new_population {};
    int n_top { static_cast<int>(std::roundf(static_cast<float>(population.size()) * neat_config::top_ratio)) };
    int counter { 0 };
    NEAT::Genome network {};

    for (int i {}; i<neat_config::n_pop; ++i){
        network = population[sort_index[counter]];
        NEAT::mutate(network);
        network.species = 0;        
        new_population.push_back(network);
        ++counter;
        if (counter >= n_top){
            counter = 0;
        }
    }
    return new_population;
}


void NEAT::add_node(Genome& network){
    if (static_cast<int>(network.node_genes.size()) > neat_config::max_nodes){
        return;
    }

    std::vector<int> enabled_connections {};
    for (int i {0}; i<network.n_conn; ++i){
        if (network.conn_genes[i].enabled){
            enabled_connections.push_back(i);
        }
    }
    int selected_conn { Random::random_choice<int>(enabled_connections, 1)[0] };

    network.add_node({ network.n_nodes, 2, Random::random_normal<float>(0, 0.1)});
    network.add_connection(
        {
            network.conn_genes[selected_conn].in_node,
            network.node_genes.back().node_idx, 
            1,
            true,
            1
        }
    );
    network.add_connection(
        {
            network.node_genes.back().node_idx,
            network.conn_genes[selected_conn].out_node,
            network.conn_genes[selected_conn].weight,
            true,
            1
        }
    );
    network.conn_genes[selected_conn].enabled = false;
}


void NEAT::add_connection(Genome& network){
    int in_node {};
    int out_node {};

    in_node = Random::random_choice(network.in_nodes, 1)[0];
    out_node = Random::random_choice(network.out_nodes, 1)[0];
    if (is_connection_valid(network.conn_genes, in_node, out_node, network.sorted_nodes)){
        network.add_connection({in_node, out_node, Random::random_normal<float>(0, 0.1), true, 0});
    }
}


std::vector<int> NEAT::get_sorted_index(std::vector<float> data){
    // https://stackoverflow.com/questions/17554242/how-to-obtain-the-index-permutation-after-the-sorting

    std::vector<int> index(data.size(), 0);
    for (int i {}; i<static_cast<int>(index.size()); ++i){
        index[i] = i;
    }
    std::sort(index.begin(), index.end(), [&](const int& a, const int& b){return (data[a] > data[b]);});
    return index;
}


std::vector<NEAT::Genome> NEAT::generate_initial_networks(int n, int n_in, int n_out){
    std::vector<NEAT::Genome> networks {};

    for (int i {}; i<n; ++i){        
        NEAT::Genome network {};
        network.species = 1;
        // input nodes
        for (int j {0}; j<n_in; ++j){
            network.add_node({j, 0, Random::random_normal<float>(0, 0.1)});
        }
        // output nodes
        for (int j {0}; j<n_out; ++j){
            network.add_node({n_in + j, 1, Random::random_normal<float>(0, 0.1)});
        }
        // connections
        for (int j {0}; j<n_in; ++j){
            for (int k {0}; k<n_out; ++k){
                network.add_connection({j, n_in + k, Random::random_normal<float>(0, 0.1), true, 0});
            }
        }
        network.compute_adjacency_list();
        network.sort_nodes();
        networks.push_back(network);
    }
    species.push_back(1);
    return networks;
}


std::vector<float> NEAT::forward(std::vector<float>& input, NEAT::Genome& network){
    std::vector<float> result(network.n_nodes, 0);
    for (int node : network.sorted_nodes){
        if (network.node_genes[node].node_type == 0){
            result[node] = input[node];
        }
        else{
            result[node] += network.node_genes[node].bias;
            result[node] = NEAT::get_activation(network.node_genes[node].node_type, result[node]);
        }
        for (NEAT::ConnectionGene& conn : network.conn_genes){
            if ((conn.in_node == node) && (conn.enabled)){
                result[conn.out_node] += conn.weight * result[node];
            }
        }
    }
    std::vector<float> output(network.n_output, 0);
    for (int i{0}; i<network.n_output; ++i){
        output[i] = result[i + network.n_input];
    }
    return output;
}


void NEAT::print_connections(Genome& network){
    std::cout << "**********************\n";
    for (ConnectionGene& conn : network.conn_genes){
        if (conn.enabled){
            std::cout << "[E] " << conn.in_node << " -> " << conn.out_node << ", weight: " << conn.weight << "\n";
        }
    }
}


void NEAT::print_nodes(Genome& network){
    std::cout << "**********************\n";
    for (NodeGene& node : network.node_genes){
        std::cout << "idx: " << node.node_idx << ", type: " << node.node_type << ", bias: " << node.bias << "\n";
    }
}


bool NEAT::is_connection_valid(std::vector<NEAT::ConnectionGene>& connections, int in_node, int out_node){
    if (in_node == out_node){
        return false;
    }
    for (NEAT::ConnectionGene& conn : connections){
        if ((conn.in_node == in_node) && (conn.out_node == out_node)){
            return false;
        }
        else if ((conn.in_node == out_node) && (conn.out_node == in_node)){
            return false;
        }
    }
    return true;
}


bool NEAT::is_connection_valid(std::vector<NEAT::ConnectionGene>& connections, int in_node, int out_node, std::vector<int>& topo_sort){
    if (in_node == out_node){
        return false;
    }

    int appear_in {0};
    int appear_out {0};
    int counter {0};
    for (int idx : topo_sort){
        if (idx == in_node){
            appear_in = counter;
        }
        if (idx == out_node){
            appear_out = counter;
        }
        ++counter;
    }
    if (appear_in > appear_out){
        return false;
    }

    for (NEAT::ConnectionGene& conn : connections){
        if ((conn.in_node == in_node) && (conn.out_node == out_node)){
            return false;
        }
        else if ((conn.in_node == out_node) && (conn.out_node == in_node)){
            return false;
        }
    }
    return true;
}


std::vector<std::vector<int>> NEAT::get_adjacency_list(std::vector<NEAT::ConnectionGene>& connections){
    std::vector<std::vector<int>> adj_list {};
    for (NEAT::ConnectionGene& conn : connections){
        if (conn.enabled){
            adj_list.push_back(std::vector<int>{ conn.in_node, conn.out_node });
        }
    }
    return adj_list;
}


void NEAT::Genome::add_node(NodeGene gene){
    bool exists_node { false };
    for (NodeGene& gen : node_genes){
        if (gene.node_idx == gen.node_idx){
            exists_node = true;
            break;
        }
    }
    if (!exists_node){
        if (gene.node_type == 0){
            in_nodes.push_back(n_nodes);
            ++n_input;
        }
        else if (gene.node_type == 1){
            out_nodes.push_back(n_nodes);
            ++n_output;
        }
        else if (gene.node_type == 2){
            in_nodes.push_back(n_nodes);
            out_nodes.push_back(n_nodes);
            ++n_hidden;
        }
        ++n_nodes;
        node_genes.push_back(gene);
    }    
}


void NEAT::Genome::add_connection(ConnectionGene gene){
    if (NEAT::innovation_number != 0){
        bool exist_gene { false} ;
        for (NEAT::ConnectionGene& egene : NEAT::existing_connections){
            if ((egene.in_node == gene.in_node) && (egene.out_node == gene.out_node)){
                exist_gene = true;
                gene.innovation_num = egene.innovation_num;
                break;
            }            
        }
        if (exist_gene){
            conn_genes.push_back(gene);
        }
        else{
            gene.innovation_num = NEAT::innovation_number;
            conn_genes.push_back(gene);
            NEAT::existing_connections.push_back(gene);   
            ++NEAT::innovation_number;
        }        
    }
    else{
        gene.innovation_num = NEAT::innovation_number;
        conn_genes.push_back(gene);
        NEAT::existing_connections.push_back(gene);   
        ++NEAT::innovation_number;
    }
    ++n_conn;
}


int NEAT::Genome::size(){
    return static_cast<int>(node_genes.size());
}


void NEAT::Genome::compute_adjacency_list(){
    adj_list = get_adjacency_list(conn_genes);
}


void NEAT::Genome::sort_nodes(){
    sorted_nodes = topological_sort_w_nodes(adj_list, n_nodes);
}


float NEAT::get_activation(int node_type, float value){
    switch(node_type){
        case 0: return value;
        case 1: return std::tanh(value);
        case 2: return std::tanh(value);
        case 3: return value;
        default: return value;
    }
}
