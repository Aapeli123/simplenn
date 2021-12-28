# SimpleNN
Very simple neural network library for genetic algorithms.
## Usage
```cpp
#include "simple_nn.hpp"
int main() {
	// define activation functions:
	auto linear = std::make_shared<std::function<float(float)>>([](float x){return x;});
	auto relu = std::make_shared<std::function<float(float)>>([](float x){return std::max(0.0f, x);});
	// Add layers with
	ADD_LAYER(input, network, 5, std::nullopt, linear);
	// Expands to:
	// auto input = layer(5e, std::nulloptr, linear);
	// network.add_layer(&input)

	ADD_LAYER(hidden_1, network, 10, &input, relu);
	ADD_LAYER(output, network, 5, &hidden_1, linear);

	network.mutate(1.0f); // Add random weights to connections and biases

	std::vector<float> inpts = {0.5f, 4, 4.2f, 3, 1}; // Some test data
	network.input(inpts);
	network.run_step();

	auto output_neurons = network.getOutput(); // Neurons of the last layer

	return 0;
}
```
## Contributing
if you somehow find this project, you can make commits and pull requests. I will review them when I have the time.
