#include "snake.hpp"

// This file includes some really cursed code for a game of snake. 
// this should not be this long but due to my bad decisions earlier and the sunk cost fallacy
// I have not refactored this to be better...

point dirToPointVector(Direction d) {
    switch (d)
    {
    case UP:
        return point{0, -1};
    case DOWN:
        return point{0, 1}; 
    case LEFT:
        return point{-1, 0};
    case RIGHT:
        return point{1, 0};
    }
    return point{0,0};
}

snake::snake(int map_width)
{
    mapWidth = map_width;

    auto rnd_dev = std::random_device();
    random_engine = std::mt19937(rnd_dev());
    
    // Start coordinates only in the middle of the map
    auto startX = generateRandom(2, map_width - 2);
    auto startY = generateRandom(2, map_width - 2); 
    auto dir = generateRandom(0, 3);
    
    auto appleX = generateRandom(0, map_width - 1);
    auto appleY = generateRandom(0, map_width - 1);
    
    while (appleX == startX && appleY == startY)
    {
        appleX = generateRandom(0, mapWidth - 1);
        appleY = generateRandom(0, mapWidth - 1);
    }

    
    apple = point{
        appleX, appleY
    };

    Direction startDir = static_cast<Direction>(dir);
    auto startPoint = point{
        startX, startY
    };
    auto bodyPoint = startPoint + -(dirToPointVector(startDir)); // Im sorry for this...
    snake_blocks.push_back(startPoint);
    snake_blocks.push_back(bodyPoint);
    snake_direction = startDir;
    // snake_blocks.push_back(startPoint + -(dirToPointVector(startDir)));
}




void snake::update() {
    if(lost) {
        return;
    }
    turnCounter++;
    for(int i = (snake_blocks.size() - 1); i >= 1; i--) {
        snake_blocks[i] = snake_blocks[i - 1];
    }
    snake_blocks[0] += dirToPointVector(snake_direction);

    if(snake_blocks[0].x >= mapWidth || snake_blocks[0].y >= mapWidth) {
        lost = true;
        return;
    }
    if(snake_blocks[0].x < 0 || snake_blocks[0].y < 0) {
        lost = true;
        return;
    }

    for(int i = 1; i < snake_blocks.size(); i++) {
        if(snake_blocks[i] == snake_blocks[0]) {
            lost = true;
            break;
        }
    }

    if(snake_blocks[0].x == apple.x && snake_blocks[0].y == apple.y) {
        auto appleX = generateRandom(0, mapWidth - 1);
        auto appleY = generateRandom(0, mapWidth - 1);
        while (appleX == snake_blocks[0].x && appleY == snake_blocks[0].y)
        {
            appleX = generateRandom(0, mapWidth - 1);
            appleY = generateRandom(0, mapWidth - 1);
        }

        
        apple = point{
            appleX, appleY
        };
        snake_blocks.push_back(prev_last_tail);
    }

    auto lastPoint = snake_blocks[snake_blocks.size() -1];
    prev_last_tail = lastPoint;
}

int snake::snake_len() {
    return snake_blocks.size();
}

int snake::generateRandom(int min, int max) {
    std::uniform_int_distribution<> distr(min, max);
    return distr(random_engine);
}
