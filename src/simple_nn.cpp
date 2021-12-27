#include "simple_nn.hpp"
neural_net::neural_net()
{
    layers = std::vector<layer*>();
}

void neural_net::add_layer(layer* l) {
    layers.push_back(l);
}

void neural_net::mutate(float mutation_factor) {
    for(int i = 1; i<layers.size(); i++) {
        auto l = layers[i];
        l->add_random(mutation_factor);
    }
}

void neural_net::run_step() {
    for(int i = 1; i<layers.size(); i++) {
        auto l = layers[i];
        for(auto& n: l->layerNeurons) {
            n.calculateValue();
        }
    }
}

std::vector<neuron> neural_net::getOutput() {
    auto lastLayer = layers[layers.size() - 1];
    return lastLayer->layerNeurons;
}

void neural_net::input(std::vector<float> inputs) {
    assert(inputs.size() == layers[0]->layerNeurons.size());
    for(int i = 0; i<layers[0]->layerNeurons.size(); i++) {
        layers[0]->layerNeurons[i].value = inputs[i];
    }
}