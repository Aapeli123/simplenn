project(genetic-algo)
cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ./out)
set(CMAKE_BINARY_DIR  /out)


include_directories(genetic-algo ./include)
add_library(simplenn STATIC
    src/neuron.cpp src/layer.cpp src/simple_nn.cpp 
)
