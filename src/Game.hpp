#pragma once

#include <algorithm>

#include "Board.hpp"

class Game {
   public:
    Board board;
    struct State {
        IntVec player;
        std::vector<IntVec> boxes;
    } state;

    Game(const Board& board) : board(board), state()
    {
        for (i32 x = 0; x < board.width(); ++x) {
            for (i32 y = 0; y < board.height(); ++y) {
                Board::Tile& tile = this->board.at({x, y});

                if (tile == Board::Player) {
                    this->state.player = {x, y};
                    tile = Board::Empty;
                } else if (tile == Board::Box) {
                    this->state.boxes.push_back({x, y});
                    tile = Board::Empty;
                }
            }
        }
    }

    void update(Direction action)
    {
        IntVec movement = action.asVec();
        IntVec newPosition = this->state.player + movement;

        if (board.at(newPosition) == Board::Wall) {
            return;
        }

        if (board.inBounds(newPosition)) {
            this->state.player = newPosition;
        }

        this->log();
    }

    void log() const
    {
        this->board.log();

        auto logPosition = [](const IntVec pos) {
            std::cerr << "{ " << pos.x << ", " << pos.y << " }";
        };

        std::cerr << "Player:\n\t";
        logPosition(this->state.player);
        std::cerr << '\n';

        std::cerr << "Boxes:\n";
        for (IntVec box : this->state.boxes) {
            std::cerr << '\t';
            logPosition(box);
            std::cerr << '\n';
        }
        std::cerr << '\n';
    }

   private:
    bool hasBoxAt(IntVec position) const
    {
        const auto& boxes = this->state.boxes;
        return std::find(boxes.cbegin(), boxes.cend(), position) !=
               boxes.cend();
    }
};
