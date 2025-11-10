#include "Renderer.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <format>
#include <iostream>

#include "Rgb.hpp"

static constexpr float degToRad = M_PI / 180.0f;

Renderer::Renderer(const Board& board)
{
    const int screenWidth = board.width() * Renderer::tileSize;
    const int screenHeight = board.height() * Renderer::tileSize;
    static const char* title = "Cocoban";

    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(60);
}

Renderer::~Renderer()
{
    CloseWindow();
}

void Renderer::render(const Game& game)
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
                case Board::Outside:
                    Renderer::fillTile({x, y}, BLACK);
                    break;
                default:
                    std::cerr << std::format(
                        "Unexpected tile {} at position {}, {}",
                        Board::tileRepr(game.board.at({x, y})), x, y);
                    std::cerr << std::endl;
                    game.board.log();
                    std::exit(1);
            }
        }
    }

    std::ranges::for_each(game.state.boxes, Renderer::renderBox);
    Renderer::renderPlayer(game.state.player);
}

void Renderer::fillTile(Position position, Color color)
{
    DrawRectangle(position.x * Renderer::tileSize,
                  position.y * Renderer::tileSize, Renderer::tileSize,
                  Renderer::tileSize, color);
}

void Renderer::drawCross(Position position, Color color)
{
    constexpr float lineWidth = static_cast<float>(Renderer::tileSize) * 0.1f;
    constexpr float lineLength = static_cast<float>(Renderer::tileSize) / 2;

    const Position centerInt =
        Position{
            position.x * Renderer::tileSize,
            position.y * Renderer::tileSize,
        } +
        Position{Renderer::tileSize / 2, Renderer::tileSize / 2};
    const Vector2 center = {
        static_cast<float>(centerInt.x),
        static_cast<float>(centerInt.y),
    };

    constexpr std::array<float, 4> angles = {45.0f, 135.0f, 225.0f, 315.0f};

    for (float angle : angles) {
        const float rad = angle * degToRad;
        const Vector2 end = {center.x + std::cos(rad) * lineLength,
                             center.y + std::sin(rad) * lineLength};

        DrawLineEx(center, end, lineWidth, color);
    }
}

void Renderer::renderHole(Position position)
{
    Renderer::renderFloor(position);
    constexpr Color color = catpuccin::Lavender.opaque();
    // Renderer::fillTile(position, color);
    Renderer::drawCross(position, color);
}

void Renderer::renderBox(Position position)
{
    constexpr Color color = catpuccin::Blue.opaque();
    constexpr Color innerColor = catpuccin::Sky.opaque();

    Renderer::fillTile(position, color);

    constexpr float innerScale = 0.66f;
    constexpr float innerSize =
        static_cast<float>(Renderer::tileSize) * innerScale;
    constexpr Vector2 sz = {innerSize, innerSize};

    constexpr float offset =
        static_cast<float>(Renderer::tileSize) * (1.0f - innerScale) / 2.0f;
    const Vector2 corner = {
        static_cast<float>(position.x * Renderer::tileSize) + offset,
        static_cast<float>(position.y * Renderer::tileSize) + offset,
    };

    DrawRectangleV(corner, sz, innerColor);
}

void Renderer::renderPlayer(Position position)
{
    constexpr Color color = catpuccin::Red.opaque();
    Renderer::fillTile(position, color);
}

void Renderer::renderFloor(Position position)
{
    constexpr Color color = catpuccin::Yellow.opaque();
    Renderer::fillTile(position, color);
}

void Renderer::renderWall(Position position)
{
    constexpr Color color = catpuccin::DarkGray.opaque();
    Renderer::fillTile(position, color);
}
