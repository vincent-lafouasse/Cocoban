#pragma once

#include <raylib.h>

#include "Board.hpp"
#include "Game.hpp"

class Renderer {
   public:
    static constexpr i32 tileSize = 64;

    explicit Renderer(const Board& board);
    ~Renderer();

    static void render(const Game& game);

   private:
    static void renderHole(IntVec position);
    static void renderBox(IntVec position);
    static void renderPlayer(IntVec position);
    static void renderFloor(IntVec position);
    static void renderWall(IntVec position);

    static void fillTile(IntVec position, Color color);
    static void drawCross(IntVec position, Color color);
};
