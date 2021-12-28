#include <vector>
#include <random>
#include <algorithm>
#include <string>
struct point {
    int x, y;
    void operator +=(point other) {
        x += other.x;
        y += other.y;
    };
    bool operator == (point& other) {
        return other.x == x && other.y == y;
    };
    point operator +(point other) {
        return point{
            x = x + other.x,
            y = y + other.y
        };
    };
    point operator -() {
        return point{
            x = -x,
            y = -y
        };
    };
};

enum Direction {
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
};
class snake
{
private:
    std::vector<point> snake_blocks;
    point apple;
    bool lost = false;
    std::mt19937 random_engine;
    point prev_last_tail;
public:
    std::vector<std::vector<char>> map;
    Direction snake_direction;
    snake(int map_width);
    void update();
    int snake_len();
    int generateRandom(int min, int max);
};