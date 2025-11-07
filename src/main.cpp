#include <vector>

#include <raylib.h>

#include "colors/Rgb.hpp"
#include "ints.hpp"

struct IntVec {
    i32 x;
    i32 y;

    IntVec up() const { return {x, y - 1}; }
    IntVec down() const { return {x, y + 1}; }
    IntVec left() const { return {x - 1, y}; }
    IntVec right() const { return {x + 1, y}; }

    IntVec scaleUp(i32 scale) const { return {scale * x, scale * y}; }

    IntVec operator+(const IntVec& other) const
    {
        return {x + other.x, y + other.y};
    }
};

struct Board {
    static constexpr char wall = '#';
    static constexpr char empty = ' ';
    static constexpr char player = '@';
    static constexpr char token = '$';
    static constexpr char hole = '.';

    std::vector<std::string> rows;
    i32 width;
    i32 height;
    IntVec playerPosition;

    bool inBounds(IntVec pos) const
    {
        const bool horizontal = pos.x >= 0 && pos.x < this->width;
        const bool vertical = pos.y >= 0 && pos.y < this->height;
        return horizontal && vertical;
    }

    static Board hardcoded()
    {
        std::vector<std::string> rows = {
            "#####",
            "#.$ #",
            "#####",
        };
        IntVec player = {3, 1};

        i32 width = rows[0].size();
        i32 height = rows.size();

        return {rows, width, height, player};
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

struct Direction {
    enum Self {
        Up,
        Down,
        Left,
        Right,
    };

    Direction(Self self) : self(self) {}

    IntVec asVec() const
    {
        IntVec zero = {0, 0};

        switch (self) {
            case Up:
                return zero.up();
            case Down:
                return zero.down();
            case Left:
                return zero.left();
            case Right:
                return zero.right();
            default:
                return zero;
        }
    }

    Self self;
};

class Game {
   public:
    Game(const Board& board) : board(board) {}

    void update(Direction action)
    {
        IntVec movement = action.asVec();
        IntVec newPosition = board.playerPosition + movement;

        if (board.inBounds(newPosition)) {
            board.playerPosition = newPosition;
        }
    }

    void render() const
    {
        for (i32 x = 0; x < board.width; ++x) {
            for (i32 y = 0; y < board.height; ++y) {
                Color color = Render::tileColor(board.rows[y][x]);

                DrawRectangle(x * Render::tileSize, y * Render::tileSize,
                              Render::tileSize, Render::tileSize, color);
            }
        }

        DrawRectangle(board.playerPosition.x * Render::tileSize,
                      board.playerPosition.y * Render::tileSize,
                      Render::tileSize, Render::tileSize,
                      Render::tileColor(Board::player));
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
        if (IsKeyDown(KEY_RIGHT))
            game.update(Direction::Right);
        if (IsKeyDown(KEY_LEFT))
            game.update(Direction::Left);
        if (IsKeyDown(KEY_UP))
            game.update(Direction::Up);
        if (IsKeyDown(KEY_DOWN))
            game.update(Direction::Down);

        BeginDrawing();
        game.render();
        EndDrawing();
    }

    CloseWindow();
}
