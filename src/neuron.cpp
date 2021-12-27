#include "neuron.hpp"


neuron::neuron(activation_function activator_fn)
{
    activator = activator_fn;
    value = 0.0f;
    bias = 0.0f;
    bias_weigth = 0.0f;
}

void neuron::calculateValue() {
    float sum = 0;
    for(auto c: connections) {
        sum += c.first * c.second->value;
    }
    auto activation = *activator.get();
    value = activation(sum + (bias * bias_weigth));
}