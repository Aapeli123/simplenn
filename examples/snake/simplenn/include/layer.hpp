#include <memory>
#include <optional>
#include <vector>
#include <random>
#include "neuron.hpp"
struct layer
{
    layer(int neuron_count, std::optional<layer*> prev_layer, activation_function activator);
    std::vector<neuron> layerNeurons;
    void add_random(float mutation_amount);
    private:
        std::mt19937 rng;
};
