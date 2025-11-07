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

Color tileColor(Board::Tile tile)
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

void fillTile(IntVec position, Color color)
{
    DrawRectangle(position.x * Render::tileSize, position.y * Render::tileSize,
                  Render::tileSize, Render::tileSize, color);
}
};  // namespace Render

class Game {
   public:
    Game(const Board& board) : board(board), player(), tokens(), holes()
    {
        for (i32 x = 0; x < board.width(); ++x) {
            for (i32 y = 0; y < board.height(); ++y) {
                Board::Tile& tile = this->board.at({x, y});

                if (tile == Board::Player) {
                    this->player = {x, y};
                    tile = Board::Empty;
                } else if (tile == Board::Token) {
                    this->tokens.push_back({x, y});
                    tile = Board::Empty;
                } else if (tile == Board::Hole) {
                    this->holes.push_back({x, y});
                    tile = Board::Empty;
                }
            }
        }
    }

    void update(Direction action)
    {
        IntVec movement = action.asVec();
        IntVec newPosition = this->player + movement;

        if (board.inBounds(newPosition)) {
            this->player = newPosition;
        }
    }

    void render() const
    {
        for (i32 x = 0; x < board.width(); ++x) {
            for (i32 y = 0; y < board.height(); ++y) {
                IntVec position = {x, y};
                Color color = Render::tileColor(board.at(position));

                Render::fillTile(position, color);
            }
        }
        Render::fillTile(this->player, Render::tileColor(Board::Player));
        for (IntVec hole : this->holes) {
            Render::fillTile(hole, Render::tileColor(Board::Hole));
        }
        for (IntVec token : this->tokens) {
            Render::fillTile(token, Render::tileColor(Board::Token));
        }
    }

   private:
    Board board;
    IntVec player;
    std::vector<IntVec> tokens;
    std::vector<IntVec> holes;
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
        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
}
