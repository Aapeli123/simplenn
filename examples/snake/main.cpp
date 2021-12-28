#include <curses.h>
#include <iostream>

#include "simple_nn.hpp"
#include "snake.hpp"

void drawSnake(snake& s, WINDOW* win) {
    wclear(win);
    for (int y = 0; y < s.map.size(); y++)
    {
        for (int x = 0; x < s.map.size(); x++)
        {
            move(y, x);
            auto c = s.map[y][x];
            waddch(win,c);
        }    
    }
    
}

std::vector<neural_net> generateNetworks() {

}

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
    auto s = snake(10);
    auto window = initscr();
    s.update();
    drawSnake(s, window);
    getch();
    s.update();
    drawSnake(s, window);
    refresh();
    getch();
    endwin();
    exit_curses(0);
    return 0;
}