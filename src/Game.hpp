#pragma once

#include "Board.hpp"

class Game {
   public:
    Board board;
    struct State {
        IntVec player;
        std::vector<IntVec> boxes;
    } state;

    explicit Game(const Board& board);
    void update(Direction action);

    [[nodiscard]] u32 numberOfFilledHoles() const;

    void log() const;

   private:
    [[nodiscard]] bool hasBoxAt(IntVec position) const;
    void computeInaccessible();
};
