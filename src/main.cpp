#include <array>
#include <iostream>

#include <raylib.h>

#include "Board.hpp"
#include "Game.hpp"
#include "render/Renderer.hpp"

int main(int ac, char* av[])
{
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " ";
        std::cerr << "level.map" << '\n';
        std::cerr << "\nYou may find maps in levels/";
        std::cerr << std::endl;
        std::exit(1);
    }
    const char* levelPath = av[1];

    const Board board = Board::load(levelPath);
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
                    default:;
                }
            } else if (isDown && IsKeyUp(key)) {
                isDown = false;
            }
        }

        BeginDrawing();
        Renderer::render(game);

        DrawText(TextFormat("Rem: %u", game.state.boxes.size() -
                                           game.numberOfFilledHoles()),
                 0, 0, 50, BLACK);
        // DrawFPS(0, 0);
        EndDrawing();
    }
}
