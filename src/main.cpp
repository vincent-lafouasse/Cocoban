#include <vector>

#include <raylib.h>

#include "colors/Rgb.hpp"
#include "ints.hpp"

struct IntVec {
    int x;
    int y;

    IntVec up() const { return {x, y - 1}; }

    IntVec down() const { return {x, y + 1}; }

    IntVec left() const { return {x - 1, y}; }

    IntVec right() const { return {x + 1, y}; }
};

struct Board {
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
        const bool horizontal =
            pos.x >= 0 && pos.x < static_cast<int>(this->width);
        const bool vertical =
            pos.y >= 0 && pos.y < static_cast<int>(this->height);
        return horizontal && vertical;
    }

    static Board hardcoded()
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

void init(const Board& board)
{
    InitWindow(board.width * Render::tileSize, board.height * Render::tileSize,
               "Cocoban");
    SetTargetFPS(60);
}

Color tileColor(char tile)
{
    if (tile == Board::wall) {
        return catpuccin::DarkGray.opaque();
    } else if (tile == Board::empty) {
        return catpuccin::Rosewater.opaque();
    } else if (tile == Board::player) {
        return catpuccin::Red.opaque();
    } else if (tile == Board::token) {
        return catpuccin::Blue.opaque();
    } else if (tile == Board::hole) {
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
    Game(const Board& board) : board(board) {}

    void render() const
    {
        for (std::size_t x = 0; x < board.width; ++x) {
            for (std::size_t y = 0; y < board.height; ++y) {
                Color color = Render::tileColor(board.rows[y][x]);

                DrawRectangle(x * Render::tileSize, y * Render::tileSize,
                              Render::tileSize, Render::tileSize, color);
            }
        }
        DrawFPS(0, 0);
    }

   private:
    Board board;
};

int main()
{
    Board board = Board::hardcoded();
    Render::init(board);
    Game game(board);

    while (!WindowShouldClose()) {
        BeginDrawing();
        game.render();
        EndDrawing();
    }

    CloseWindow();
}
