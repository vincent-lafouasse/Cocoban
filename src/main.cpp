#include <raylib.h>

#include "Board.hpp"
#include "colors/Rgb.hpp"
#include "ints.hpp"

namespace Render {
static constexpr i32 tileSize = 64;

void init(const Board& board)
{
    InitWindow(board.width() * Render::tileSize,
               board.height() * Render::tileSize, "Cocoban");
    SetTargetFPS(60);
}

Color tileColor(char tile)
{
    if (tile == Board::Wall) {
        return catpuccin::DarkGray.opaque();
    } else if (tile == Board::Empty) {
        return catpuccin::Rosewater.opaque();
    } else if (tile == Board::Player) {
        return catpuccin::Red.opaque();
    } else if (tile == Board::Token) {
        return catpuccin::Blue.opaque();
    } else if (tile == Board::Hole) {
        return catpuccin::Lavender.opaque();
    } else {
        Rgb black = {0, 0, 0};
        return black.opaque();
    }
}
};  // namespace Render

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
        for (i32 x = 0; x < board.width(); ++x) {
            for (i32 y = 0; y < board.height(); ++y) {
                Color color = Render::tileColor(board.rows[y][x]);

                DrawRectangle(x * Render::tileSize, y * Render::tileSize,
                              Render::tileSize, Render::tileSize, color);
            }
        }

        DrawRectangle(board.playerPosition.x * Render::tileSize,
                      board.playerPosition.y * Render::tileSize,
                      Render::tileSize, Render::tileSize,
                      Render::tileColor(Board::Player));
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
