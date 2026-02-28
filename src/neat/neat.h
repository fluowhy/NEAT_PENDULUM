#ifndef NEAT_H
#define NEAT_H

#include "neat_config.h"
#include "topological_sort.h"
#include "random.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

namespace NEAT{
    extern int innovation_number;
    extern std::vector<int> species;


    float sum_if_value(std::vector<int>& x, int value);


    std::vector<int> get_matched_state(std::vector<int>& alig_gen_1, std::vector<int>& alig_gen_2, int max_inn_num, int min_inn_num);


    struct NodeGene{
        int node_idx {};
        int node_type {};
        float bias {};
    };


    struct ConnectionGene{
        int in_node {};
        int out_node {};
        float weight {};
        bool enabled {};
        int innovation_num {};
    };


    extern std::vector<ConnectionGene> existing_connections;


    struct Genome{
        std::vector<NodeGene> node_genes {};
        std::vector<ConnectionGene> conn_genes {};
        std::vector<std::vector<int>> adj_list {};
        std::vector<int> sorted_nodes {};
        std::vector<int> in_nodes {};
        std::vector<int> out_nodes {};
        int n_input {0};
        int n_output {0};
        int n_hidden {0};
        int n_nodes {0};
        int n_conn {0};
        int species {0};
        
        void add_node(NodeGene gene);
        void add_connection(ConnectionGene gene);
        int size();
        void compute_adjacency_list();
        void sort_nodes();
    };


    void bias_mod(NEAT::Genome& network);


    void weight_mod(NEAT::Genome& network);


    void mutate(Genome& network);


    void sort_nodes(Genome& network);


    void add_connection_to_offspring(Genome& offspring, ConnectionGene& conn, Genome& network);


    Genome crossover(Genome& network1, Genome& network2, float f1, float f2);


    void estimate_species_of_population(std::vector<Genome>& population, std::vector<Genome>& new_population, float thr);


    void add_gen_idx_to_aligned_vector(std::vector<int>& aligned_genomes, Genome& network, int inn_num);


    int get_max_innovation_number(Genome& network);


    // bool find_innovation_number(Genome& network, int value);

    
    float compatibility_distance(Genome& network1, Genome& network2, float c1, float c2, float c3);


    std::vector<Genome> get_next_population(std::vector<Genome>& population, std::vector<int>& sort_index);


    void add_node(Genome& network);
    void add_connection(Genome& network);
    std::vector<int> get_sorted_index(std::vector<float> data);
    std::vector<Genome> generate_initial_networks(int n, int n_in, int n_out);
    std::vector<float> forward(std::vector<float>& input, Genome& network);
    void print_nodes(Genome& network);
    void print_connections(Genome& network);

    bool is_connection_valid(std::vector<ConnectionGene>& connections, int in_node, int out_node);
    bool is_connection_valid(std::vector<ConnectionGene>& connections, int in_node, int out_node, std::vector<int>& topo_sort);

    std::vector<std::vector<int>> get_adjacency_list(std::vector<ConnectionGene>& connections);

    float get_activation(int node_type, float value);
}

#endif
