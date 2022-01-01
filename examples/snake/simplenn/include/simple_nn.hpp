#include "layer.hpp"

#define ADD_LAYER(name, network, size, prev_layer, activation) auto name = layer(size, prev_layer, activation); network.add_layer(&name)

class neural_net
{
private:
public:
    std::vector<layer*> layers;
    neural_net();
    void add_layer(layer* layer);
    void run_step();
    void input(std::vector<float> data);
    void mutate(float mutation_factor);
    std::vector<neuron> getOutput();
};

