#pragma once

#include <raylib.h>

#include "Board.hpp"
#include "Game.hpp"

class Renderer {
   public:
    using Position = Game::Position;

    static constexpr i32 tileSize = 64;

    explicit Renderer(const Board& board);
    ~Renderer();

    static void render(const Game& game);

   private:
    static void renderHole(Position position);
    static void renderBox(Position position);
    static void renderPlayer(Position position);
    static void renderFloor(Position position);
    static void renderWall(Position position);

    static void fillTile(Position position, Color color);
    static void drawCross(Position position, Color color);
};
