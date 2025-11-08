#pragma once

#include <raylib.h>

#include "Board.hpp"
#include "Game.hpp"
#include "colors/Rgb.hpp"

struct Renderer {
    static constexpr i32 tileSize = 64;

    Renderer(const Board& board)
    {
        const int screenWidth = board.width() * Renderer::tileSize;
        const int screenHeight = board.height() * Renderer::tileSize;
        static const char* title = "Cocoban";

        InitWindow(screenWidth, screenHeight, title);
        SetTargetFPS(60);
    }

    ~Renderer() { CloseWindow(); }

    void render(const Game& game) const
    {
        for (i32 x = 0; x < game.board.width(); ++x) {
            for (i32 y = 0; y < game.board.height(); ++y) {
                switch (game.board.at({x, y})) {
                    case Board::Wall:
                        Renderer::renderWall({x, y});
                        break;
                    case Board::Empty:
                        Renderer::renderFloor({x, y});
                        break;
                    default:
                        std::cerr << "Unexpected tile "
                                  << game.board.at({x, y});
                        std::cerr << " at position " << x << " " << y
                                  << std::endl;
                        game.board.log();
                        std::exit(1);
                }
            }
        }
        for (IntVec hole : game.holes) {
            Renderer::renderHole(hole);
        }
        for (IntVec box : game.boxes) {
            Renderer::renderBox(box);
        }
        Renderer::renderPlayer(game.player);
    }

    static void fillTile(IntVec position, Color color)
    {
        DrawRectangle(position.x * Renderer::tileSize,
                      position.y * Renderer::tileSize, Renderer::tileSize,
                      Renderer::tileSize, color);
    }

    static void renderHole(IntVec position)
    {
        const Color color = catpuccin::Lavender.opaque();
        Renderer::fillTile(position, color);
    }

    static void renderBox(IntVec position)
    {
        const Color color = catpuccin::Blue.opaque();
        Renderer::fillTile(position, color);
    }

    static void renderPlayer(IntVec position)
    {
        const Color color = catpuccin::Red.opaque();
        Renderer::fillTile(position, color);
    }

    static void renderFloor(IntVec position)
    {
        const Color color = catpuccin::Rosewater.opaque();
        Renderer::fillTile(position, color);
    }

    static void renderWall(IntVec position)
    {
        const Color color = catpuccin::DarkGray.opaque();
        Renderer::fillTile(position, color);
    }
};
