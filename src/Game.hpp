#pragma once

#include <algorithm>

#include "Board.hpp"

class Game {
   public:
    Board board;
    IntVec player;
    std::vector<IntVec> boxes;
    std::vector<IntVec> holes;

    Game(const Board& board) : board(board), player(), boxes(), holes()
    {
        for (i32 x = 0; x < board.width(); ++x) {
            for (i32 y = 0; y < board.height(); ++y) {
                Board::Tile& tile = this->board.at({x, y});

                if (tile == Board::Player) {
                    this->player = {x, y};
                    tile = Board::Empty;
                } else if (tile == Board::Box) {
                    this->boxes.push_back({x, y});
                    tile = Board::Empty;
                } else if (tile == Board::Hole) {
                    this->holes.push_back({x, y});
                    tile = Board::Empty;
                }
            }
        }
    }

    void update(Direction action)
    {
        IntVec movement = action.asVec();
        IntVec newPosition = this->player + movement;

        if (board.at(newPosition) == Board::Wall) {
            return;
        }

        if (board.inBounds(newPosition)) {
            this->player = newPosition;
        }
    }

   private:
    bool hasBoxAt(IntVec position) const
    {
        return std::find(boxes.cbegin(), boxes.cend(), position) !=
               boxes.cend();
    }

    bool hasHoleAt(IntVec position) const
    {
        return std::find(holes.cbegin(), holes.cend(), position) !=
               holes.cend();
    }
};
