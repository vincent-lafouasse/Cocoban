#include <vector>

#include <raylib.h>

#include "colors/Rgb.hpp"
#include "ints.hpp"

struct IntVec {
    int x;
    int y;

    IntVec up() const {
        return {x, y - 1};
    }

    IntVec down() const {
        return {x, y + 1};
    }

    IntVec left() const {
        return {x - 1, y};
    }

    IntVec right() const {
        return {x + 1, y};
    }
};

struct Map {
    static constexpr char wall = '#';
    static constexpr char empty = ' ';
    static constexpr char player = '@';
    static constexpr char token = '$';
    static constexpr char hole = '.';

    std::vector<std::string> rows;
    std::size_t width;
    std::size_t height;

    bool inBounds(IntVec pos) const
    {
        const bool horizontal = pos.x >= 0 && pos.x < static_cast<int>(this->width);
        const bool vertical = pos.y >= 0 && pos.y < static_cast<int>(this->height);
        return horizontal && vertical;
    }

    static Map hardcoded()
    {
        std::vector<std::string> rows = {
            "#####",
            "#.$@#",
            "#####",
        };

        std::size_t width = rows[0].size();
        std::size_t height = rows.size();

        return {rows, width, height};
    }
};

namespace Render {
static constexpr i32 tileSize = 64;

void init(const Map& map)
{
    InitWindow(map.width * Render::tileSize, map.height * Render::tileSize,
               "Cocoban");
    SetTargetFPS(60);
}

Color tileColor(char tile)
{
    if (tile == Map::wall) {
        return catpuccin::DarkGray.opaque();
    } else if (tile == Map::empty) {
        return catpuccin::Rosewater.opaque();
    } else if (tile == Map::player) {
        return catpuccin::Red.opaque();
    } else if (tile == Map::token) {
        return catpuccin::Blue.opaque();
    } else if (tile == Map::hole) {
        return catpuccin::Lavender.opaque();
    } else {
        Rgb black = {0, 0, 0};
        return black.opaque();
    }
}
};  // namespace Render

enum class Direction {
    Up,
    Down,
    Left,
    Right,
};

class Game {
public:
    Game(const Map& map): map(map) {}

    void render() const {
        for (std::size_t x = 0; x < map.width; ++x) {
            for (std::size_t y = 0; y < map.height; ++y) {
                Color color = Render::tileColor(map.rows[y][x]);

                DrawRectangle(x * Render::tileSize, y * Render::tileSize,
                              Render::tileSize, Render::tileSize, color);
            }
        }
        DrawFPS(0, 0);
    }

private:
    Map map;
};

int main()
{
    Map map = Map::hardcoded();
    Render::init(map);
    Game game(map);

    while (!WindowShouldClose()) {
        BeginDrawing();
        game.render();
        EndDrawing();
    }

    CloseWindow();
}
