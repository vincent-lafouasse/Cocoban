#include <raylib.h>

#include "colors/ColorMap.hpp"
#include "ints.hpp"

struct IntVec {
    int x;
    int y;
};

struct Grid {
    static constexpr i32 tileSize = 64;
    static constexpr i32 width = 7;
    static constexpr i32 height = 5;

    static i32 index(IntVec v)
    {
        if (v.x < 0 || v.x >= Grid::width) {
            return -1;
        }
        if (v.y < 0 || v.y >= Grid::height) {
            return -1;
        }

        return v.x + Grid::width * v.y;
    }
};

struct Map {
    static constexpr char blank = '_';
    static constexpr char wall = 'X';
    static constexpr char walkable = ' ';
    static constexpr char player = 'P';
    static constexpr char token = '-';
    static constexpr char hole = 'O';
};

int main()
{
    InitWindow(Grid::width * Grid::tileSize, Grid::height * Grid::tileSize,
               "Cocoban");
    SetTargetFPS(60);

    const ColorMap cmap = ColorMap::Catpuccin();

    while (!WindowShouldClose()) {
        BeginDrawing();
        for (i32 x = 0; x < Grid::width; ++x) {
            for (i32 y = 0; y < Grid::height; ++y) {
                const float progression = static_cast<float>(x + y) /
                                          (Grid::height + Grid::width - 2);
                DrawRectangle(x * Grid::tileSize, y * Grid::tileSize,
                              Grid::tileSize, Grid::tileSize,
                              cmap.get(progression).opaque());
            }
        }
        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
}
