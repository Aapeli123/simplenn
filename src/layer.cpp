#include "layer.hpp"

layer::layer(int neuron_count, std::optional<layer*> prev_layer, activation_function activator) {
    for(long long i = 0; i < neuron_count; i++) {
        layerNeurons.push_back(neuron(activator));
    }
    if(!prev_layer.has_value()) {
        return;
    }
    
    // Create connections back to previous layer neurons for current layer neurons:
    for(auto& n: layerNeurons) {
        for(auto& prevlayer_neuron: prev_layer.value()->layerNeurons) {
            n.connections.push_back(std::make_pair(0.0f, &prevlayer_neuron));
        }
    }

    rng = std::mt19937();
}

void layer::add_random(float mutation_amount) {
    std::uniform_real_distribution<float> distr(-mutation_amount, mutation_amount);
    for (auto& n: layerNeurons) {
        
        n.bias += distr(rng);
        n.bias_weigth += distr(rng);
        for(auto& c: n.connections) {
            c.first += distr(rng);
        }
    }
}