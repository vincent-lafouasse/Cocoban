#pragma once

#include "Board.hpp"

class Game {
   public:
    using Position = Board::Position;

    Board board;
    struct State {
        Position player;
        std::vector<Position> boxes;
    } state;

    explicit Game(const Board& board);
    void update(Direction action);

    [[nodiscard]] u32 numberOfFilledHoles() const;

    void log() const;

   private:
    [[nodiscard]] bool hasBoxAt(Position position) const;
    void computeInaccessible();
};
