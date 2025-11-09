#include <array>

#include <raylib.h>

#include "Board.hpp"
#include "Game.hpp"
#include "render/Renderer.hpp"

int main()
{
    Board board = Board::load("levels/l2.map");
    board.log();

    Game game(board);
    Renderer renderer(board);

    using RaylibKey = int;
    std::array<std::pair<RaylibKey, bool>, 4> keydowns = {
        std::make_pair(KEY_UP, false),
        std::make_pair(KEY_DOWN, false),
        std::make_pair(KEY_LEFT, false),
        std::make_pair(KEY_RIGHT, false),
    };

    while (!WindowShouldClose()) {
        for (auto& [key, isDown] : keydowns) {
            if (!isDown && IsKeyDown(key)) {
                isDown = true;
                switch (key) {
                    case KEY_UP:
                        game.update(Direction::Up);
                        break;
                    case KEY_DOWN:
                        game.update(Direction::Down);
                        break;
                    case KEY_LEFT:
                        game.update(Direction::Left);
                        break;
                    case KEY_RIGHT:
                        game.update(Direction::Right);
                        break;
                }
            } else if (isDown && IsKeyUp(key)) {
                isDown = false;
            }
        }

        BeginDrawing();
        renderer.render(game);
        DrawFPS(0, 0);
        EndDrawing();
    }
}
