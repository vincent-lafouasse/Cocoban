#pragma once

#include <array>

#define _USE_MATH_DEFINES
#include <cmath>

#include <raylib.h>

#include "Board.hpp"
#include "Game.hpp"
#include "colors/Rgb.hpp"

static constexpr float degToRad = M_PI / 180.0f;

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
                    case Board::Hole:
                        Renderer::renderHole({x, y});
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
        for (IntVec box : game.state.boxes) {
            Renderer::renderBox(box);
        }
        Renderer::renderPlayer(game.state.player);
    }

    static void fillTile(IntVec position, Color color)
    {
        DrawRectangle(position.x * Renderer::tileSize,
                      position.y * Renderer::tileSize, Renderer::tileSize,
                      Renderer::tileSize, color);
    }

    static void drawCross(IntVec position, Color color)
    {
        const float lineWidth = static_cast<float>(Renderer::tileSize) * 0.1f;
        const float lineLength = static_cast<float>(Renderer::tileSize) / 2;

        const IntVec centerInt =
            IntVec{
                position.x * Renderer::tileSize,
                position.y * Renderer::tileSize,
            } +
            IntVec{Renderer::tileSize / 2, Renderer::tileSize / 2};
        const Vector2 center = {
            static_cast<float>(centerInt.x),
            static_cast<float>(centerInt.y),
        };

        const std::array<float, 4> angles = {45.0f, 135.0f, 225.0f, 315.0f};

        for (float angle : angles) {
            const float rad = angle * degToRad;
            const Vector2 end = {center.x + std::cosf(rad) * lineLength,
                                 center.y + std::sinf(rad) * lineLength};

            DrawLineEx(center, end, lineWidth, color);
        }
    }

    static void renderHole(IntVec position)
    {
        Renderer::renderFloor(position);
        const Color color = catpuccin::Lavender.opaque();
        // Renderer::fillTile(position, color);
        Renderer::drawCross(position, color);
    }

    static void renderBox(IntVec position)
    {
        const Color color = catpuccin::Blue.opaque();
        const Color innerColor = catpuccin::Sky.opaque();

        Renderer::fillTile(position, color);

        const float innerScale = 0.66f;
        const float innerSize =
            static_cast<float>(Renderer::tileSize) * innerScale;

        const float offset =
            static_cast<float>(Renderer::tileSize) * (1.0f - innerScale) / 2.0f;
        const Vector2 corner = {
            static_cast<float>(position.x * Renderer::tileSize) + offset,
            static_cast<float>(position.y * Renderer::tileSize) + offset,
        };

        DrawRectangle(corner.x, corner.y, innerSize, innerSize, innerColor);
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
