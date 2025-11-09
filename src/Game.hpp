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
    void log() const;

   private:
    bool hasBoxAt(IntVec position) const;
};
