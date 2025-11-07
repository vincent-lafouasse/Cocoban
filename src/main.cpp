#include <raylib.h>

#include "colors/ColorMap.hpp"
#include "ints.hpp"

namespace Render {
static constexpr i32 tileSize = 64;
static constexpr Rgb wallColor = catpuccin::DarkGray;
static constexpr Rgb floorColor = catpuccin::Rosewater;
static constexpr Rgb playerColor = catpuccin::Red;
static constexpr Rgb tokenColor = catpuccin::Blue;
static constexpr Rgb holeColor = catpuccin::Lavender;
};  // namespace Render

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

int main()
{
    Map map = Map::hardcoded();

    InitWindow(map.width * Render::tileSize, map.height * Render::tileSize,
               "Cocoban");
    SetTargetFPS(60);

    const ColorMap cmap = ColorMap::Catpuccin();

    while (!WindowShouldClose()) {
        BeginDrawing();
        for (std::size_t x = 0; x < map.width; ++x) {
            for (std::size_t y = 0; y < map.height; ++y) {
                Rgb color = Render::floorColor;

                DrawRectangle(x * Render::tileSize, y * Render::tileSize,
                              Render::tileSize, Render::tileSize,
                              color.opaque());
            }
        }
        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
}
