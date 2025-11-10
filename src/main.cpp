#include <array>
#include <format>
#include <iostream>

#include <raylib.h>

#include "game/Game.hpp"

#include "render/Renderer.hpp"
#include "render/Rgb.hpp"

int main(int ac, char* av[])
{
    if (ac != 2) {
        std::cerr << std::format("Usage: {} level.map\n\n", av[0]);
        std::cerr << "You may find maps in levels/";
        std::cerr << std::endl;
        std::exit(1);
    }
    const char* levelPath = av[1];

    const Board board = Board::load(levelPath);

    Game game(board);
    std::vector<Game::State> undoMemory = {game.state};

    Renderer renderer(board);

    std::array keydowns = {
        std::make_pair(KEY_UP, false),   std::make_pair(KEY_DOWN, false),
        std::make_pair(KEY_LEFT, false), std::make_pair(KEY_RIGHT, false),
        std::make_pair(KEY_R, false),    std::make_pair(KEY_SPACE, false),
        std::make_pair(KEY_W, false),    std::make_pair(KEY_S, false),
        std::make_pair(KEY_A, false),    std::make_pair(KEY_D, false),
    };

    while (!WindowShouldClose()) {
        for (auto& [key, isDown] : keydowns) {
            if (!isDown && IsKeyDown(key)) {
                isDown = true;
                switch (key) {
                    case KEY_W:
                    case KEY_UP:
                        game.update(Direction::Up);
                        undoMemory.push_back(game.state);
                        break;
                    case KEY_S:
                    case KEY_DOWN:
                        game.update(Direction::Down);
                        undoMemory.push_back(game.state);
                        break;
                    case KEY_A:
                    case KEY_LEFT:
                        game.update(Direction::Left);
                        undoMemory.push_back(game.state);
                        break;
                    case KEY_D:
                    case KEY_RIGHT:
                        game.update(Direction::Right);
                        undoMemory.push_back(game.state);
                        break;
                    case KEY_SPACE:
                        if (undoMemory.size() > 1) {
                            // do not remove resetState from memory
                            undoMemory.pop_back();
                        }
                        game.state = undoMemory.back();
                        break;
                    case KEY_R:
                        game.state = undoMemory.front();
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
                 0, 0, 50, catpuccin::Lavender.opaque());
        // DrawFPS(0, 0);
        EndDrawing();
    }
}
