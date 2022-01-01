#include <curses.h>
#include <iostream>
#include <assert.h>

#include "simple_nn.hpp"
#include "snake.hpp"

// Writing this example im still trying to figure what the fuck im doing so this isn't going to be the clearest programming
// you will see. It actually will probably be really bad honestly I don't care anymore at this point. I just wanted to get
// something done.


void drawSnake(snake& s, WINDOW* win) {
    wclear(win);
    box(win, 0,0);
    if(s.lost) {
        wmove(win, 5, 3);
        wprintw(win, "Lost");
        touchwin(win);
        wrefresh(win);
        return;
    }
    auto sb = s.snake_blocks;
    wmove(win, sb[0].y, sb[0].x);
    waddch(win, 'H');
    for(int i = 1; i < sb.size(); i++) {
        wmove(win, sb[i].y, sb[i].x);
        waddch(win, 'B');
    }
    wmove(win, s.apple.y, s.apple.x);
    waddch(win, 'A');
    touchwin(win);
    wrefresh(win);
}

struct Simulation
{
    neural_net* nn;
    snake* game;
};

neural_net* generateNetwork(int mapsize) {
    auto linear = std::make_shared<std::function<float(float)>>([](float x){return x;});
    auto relu = std::make_shared<std::function<float(float)>>([](float x){return std::max(0.0f, x);});

    auto network = new neural_net();


    auto input = new layer(mapsize * mapsize, std::nullopt, linear); network->add_layer(input);
    auto h1 = new layer(10*10, input, relu); 
    network->add_layer(h1);
    
    auto h2 = new layer(10*10, h1, relu); 
    network->add_layer(h2);
    
    auto output = new layer(4, h2, linear);
    network->add_layer(output);
    network->mutate(5.0f);
    return network;

}

Simulation genSim(int mapsize) {
    auto nnptr = generateNetwork(mapsize);
    auto snakeptr = new snake(mapsize);
    snakeptr->update();
    auto sim = Simulation{
        nnptr, snakeptr
    };
    return sim;
}



std::vector<Simulation> generateSimulations(int count, int mapsize) {
    auto sims = std::vector<Simulation>();
    for (int i = 0; i < count; i++)
    {
        auto sim = genSim(mapsize);
        sim.nn->mutate(1.0f);
        sims.push_back(sim);
    }
    return sims;
}


neural_net* duplicate_nn(neural_net* nn, int mapsize) {
    auto newnn = generateNetwork(mapsize);
    assert(newnn->layers.size() == nn->layers.size()); // If this triggers I have fucked up something
    for (int i = 0; i < nn->layers.size(); i++)
    {
        for(int j = 0; j < nn->layers[i]->layerNeurons.size(); j++) {
            newnn->layers[i]->layerNeurons[j].bias = nn->layers[i]->layerNeurons[j].bias;
            newnn->layers[i]->layerNeurons[j].bias_weigth = nn->layers[i]->layerNeurons[j].bias_weigth;

            for (int k = 0; k < nn->layers[i]->layerNeurons[j].connections.size(); k++)
            {
                newnn->layers[i]->layerNeurons[j].connections[k].second = nn->layers[i]->layerNeurons[j].connections[k].second; // Beautiful line of code ( im sorry :( )
            }
        }
    }
    return newnn;
}
std::vector<Simulation> getNewSimulations(neural_net* bestnn, int count, int mapsize) {
    std::vector<Simulation>(sims);
    for (int i = 0; i < count; i++)
    {
        auto nn = duplicate_nn(bestnn, mapsize);
        snake* s = new snake(mapsize);
        auto sim = Simulation{
            nn, s
        };
        sim.nn->mutate(1.0f);
        sims.push_back(sim);
    }
    return sims;
}





Simulation pickBest(std::vector<Simulation> sims, int mapsize) {
    int bestScore = -999999999;
    Simulation* bestSim;
    for (int i = 0; i < sims.size(); i++)
    {   
        auto s = &sims[i];
        auto score = s->game->turnCounter * 0.2 + s->game->snake_len();
        if(s->game->lost) {
            score -= 10;
        }
        if(score > bestScore) {
            bestScore = score;
            bestSim = s;
        }
    }
    auto nn = duplicate_nn(bestSim->nn, mapsize);
    return Simulation{
        nn, new snake(mapsize)
    };
}

std::vector<WINDOW*> createWindows(WINDOW* mainwindow,int simcount) {
    std::vector<WINDOW*>(windows);
    for (size_t i = 0; i < simcount; i++)
    {
        auto win = subwin(mainwindow, 10, 10, 0, 10 * i);
        windows.push_back(win);
    }
    return windows;
} 

void drawSims(std::vector<Simulation> sims, std::vector<WINDOW*> windows) {
    assert(sims.size() == windows.size());
    for(int i = 0; i<sims.size(); i++) {
        auto sim = sims[i]; 
        auto game = *(sim.game);
        drawSnake(game, windows[i]);
    }
}

void runSimStep(std::vector<Simulation> sims) {
    for (auto &s : sims)
    {
        for(int y = 0; y<s.game->mapWidth; y++) {
            for(int x = 0; x<s.game->mapWidth; x++) {
                bool foundSnakeblock = false;
                if(s.game->apple.x == x && s.game->apple.y == y) {
                    s.nn->layers[0]->layerNeurons[x+y*s.game->mapWidth].value = 1;
                    continue;
                }
                for(auto sb:s.game->snake_blocks) {
                    if(sb.x == x && sb.y == y) {
                        s.nn->layers[0]->layerNeurons[x+y*s.game->mapWidth].value = -1;
                        foundSnakeblock = true;
                        break;
                    }
                }
                if (!foundSnakeblock)
                    s.nn->layers[0]->layerNeurons[x+y*s.game->mapWidth].value = 0;
            }
        }
        s.nn->run_step();
        auto out = s.nn->getOutput();
        float maxVal = -9999999;
        int bestDir;
        for(int i = 0; i<out.size(); i++) {
            if(out[i].value > maxVal) {
                maxVal = out[i].value;
                bestDir = i;
            }
        }
        s.game->snake_direction = static_cast<Direction>(bestDir);
        s.game->update();
    }
}

int main() {
    constexpr int SIMSTEPS = 40;
    constexpr int SIMCOUNT = 10;
    constexpr int EPOCHES = 50;
    auto window = initscr();
    curs_set(0);

    auto sims = generateSimulations(SIMCOUNT, 10);
    auto windows = createWindows(window, SIMCOUNT);
    for(int epoch = 0; epoch < EPOCHES; epoch++) {
        for(int i = 0; i < SIMSTEPS; i++) {
            drawSims(sims, windows);
            runSimStep(sims);
            getch();
        }
        auto best = pickBest(sims, 10);
        for(auto s : sims) {
            delete s.game;
            for(auto l: s.nn->layers) {
                delete l;
            }
            delete s.nn;
        }
        sims.clear();
        sims.push_back(best);
        auto mutated = getNewSimulations(best.nn, SIMCOUNT - 1, 10);
        sims.insert(sims.end(), mutated.begin(), mutated.end());
        printw("Epoch %d done", epoch);
        getch();
    }



    // TODO Snake game and evolution
    /* auto s = snake(10);
    auto s2 = snake(10);

    auto g1 = subwin(window, 10,10, 0, 0);
    auto g2 = subwin(window, 10,10, 0, 10);
    for(int i = 0; i < 100; i++) {
        s.update();
        s2.update();
        drawSnake(s, g1);
        drawSnake(s2, g2);
        move(11, 0);
        printw("%d", i);
        refresh();
        getch();
    }

     */
    curs_set(1);
    exit_curses(0);
    for(auto s : sims) {
        delete s.game;
        for(auto l: s.nn->layers) {
            delete l;
        }
        delete s.nn;

    }
    return 0;
}