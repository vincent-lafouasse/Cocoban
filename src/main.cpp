#include <raylib.h>

#include "colors/ColorMap.hpp"
#include "ints.hpp"

struct IntVec {
    int x;
    int y;
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

int main()
{
    Map map = Map::hardcoded();
    Render::init(map);

    while (!WindowShouldClose()) {
        BeginDrawing();
        for (std::size_t x = 0; x < map.width; ++x) {
            for (std::size_t y = 0; y < map.height; ++y) {
                Color color = Render::tileColor(map.rows[y][x]);

                DrawRectangle(x * Render::tileSize, y * Render::tileSize,
                              Render::tileSize, Render::tileSize, color);
            }
        }
        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
}
