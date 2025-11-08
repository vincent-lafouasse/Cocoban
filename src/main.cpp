#include <unordered_set>

#include <raylib.h>

#include "Board.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

int main()
{
    Board board = Board::load("levels/l2.map");
    board.log();

    Game game(board);
    Renderer renderer(board);

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
        renderer.render(game);
        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
}
