#include "simple_nn.hpp"
#include <curses.h>
int main() {

    auto linear = std::make_shared<std::function<float(float)>>([](float x){return x;});
    auto relu = std::make_shared<std::function<float(float)>>([](float x){return std::max(0.0f, x);});
    
    auto network = neural_net();

    ADD_LAYER(input, network, 10*10, std::nullopt, linear);
    ADD_LAYER(hidden_1, network, 128, &input, relu);
    ADD_LAYER(hidden_2, network, 128, &hidden_1, relu);
    ADD_LAYER(hidden_3, network, 128, &hidden_2, relu);
    ADD_LAYER(output, network, 128, &hidden_3, linear);

    network.add_layer(&output);

    network.mutate(10.0f);


    // TODO Snake game and evolution
    auto window = initscr();
    box(window, 0, 0);
    move(1,1);
    printw("Hello curses!");
    getch();
    endwin();
    exit_curses(0);
    return 0;
}