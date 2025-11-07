#include <array>
#include <unordered_set>

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

        if (board.at(newPosition) == Board::Wall) {
            return;
        }

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
        for (IntVec hole : this->holes) {
            Render::fillTile(hole, Render::tileColor(Board::Hole));
        }
        for (IntVec token : this->tokens) {
            Render::fillTile(token, Render::tileColor(Board::Token));
        }
        Render::fillTile(this->player, Render::tileColor(Board::Player));
    }

   private:
    Board board;
    IntVec player;
    std::vector<IntVec> tokens;
    std::vector<IntVec> holes;
};

int main()
{
    Board board = Board::load("levels/l2.map");
    board.log();

    Render::init(board);
    Game game(board);

    using RaylibKey = int;
    std::unordered_set<RaylibKey> keyDowns;

    auto keys = [](Direction direction) {
        switch (direction.inner) {
            case Direction::Up:
                return KEY_UP;
            case Direction::Down:
                return KEY_DOWN;
            case Direction::Left:
                return KEY_LEFT;
            case Direction::Right:
                return KEY_RIGHT;
        }
    };

    while (!WindowShouldClose()) {
        for (const Direction& direction : Direction::all()) {
            int key = keys(direction);

            if (!keyDowns.contains(key) && IsKeyDown(key)) {
                keyDowns.insert(key);
                game.update(direction);
            } else if (keyDowns.contains(key) && IsKeyUp(key)) {
                keyDowns.erase(key);
            }
        }

        BeginDrawing();
        game.render();
        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
}
