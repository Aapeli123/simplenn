#include <functional>
#include <memory>
#include <vector>

using activation_function = std::shared_ptr<std::function<float(float)>>;

class neuron
{
private:
    std::shared_ptr<std::function<float(float)>> activator;
public:
    std::vector<std::pair<float, neuron*>> connections;
    float bias;
    float bias_weigth;
    float value;
    neuron(activation_function activator_fn);
    void calculateValue();
};