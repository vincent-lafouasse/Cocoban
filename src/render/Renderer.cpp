#include "Renderer.hpp"

#include <array>
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
                default:
                    std::cerr << "Unexpected tile " << game.board.at({x, y});
                    std::cerr << " at position " << x << " " << y << std::endl;
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

void Renderer::fillTile(IntVec position, Color color)
{
    DrawRectangle(position.x * Renderer::tileSize,
                  position.y * Renderer::tileSize, Renderer::tileSize,
                  Renderer::tileSize, color);
}

void Renderer::drawCross(IntVec position, Color color)
{
    constexpr float lineWidth = static_cast<float>(Renderer::tileSize) * 0.1f;
    constexpr float lineLength = static_cast<float>(Renderer::tileSize) / 2;

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

    constexpr std::array<float, 4> angles = {45.0f, 135.0f, 225.0f, 315.0f};

    for (float angle : angles) {
        const float rad = angle * degToRad;
        const Vector2 end = {center.x + std::cos(rad) * lineLength,
                             center.y + std::sin(rad) * lineLength};

        DrawLineEx(center, end, lineWidth, color);
    }
}

void Renderer::renderHole(IntVec position)
{
    Renderer::renderFloor(position);
    constexpr Color color = catpuccin::Lavender.opaque();
    // Renderer::fillTile(position, color);
    Renderer::drawCross(position, color);
}

void Renderer::renderBox(IntVec position)
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

void Renderer::renderPlayer(IntVec position)
{
    constexpr Color color = catpuccin::Red.opaque();
    Renderer::fillTile(position, color);
}

void Renderer::renderFloor(IntVec position)
{
    constexpr Color color = catpuccin::Rosewater.opaque();
    Renderer::fillTile(position, color);
}

void Renderer::renderWall(IntVec position)
{
    constexpr Color color = catpuccin::DarkGray.opaque();
    Renderer::fillTile(position, color);
}
