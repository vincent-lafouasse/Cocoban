#pragma once

#include <vector>

#include "geometry.hpp"
#include "ints.hpp"

struct Board {
    enum Tile : u8 {
        Wall = '#',
        Empty = ' ',
        Player = '@',
        Token = '$',
        Hole = '.',
    };

    std::vector<std::string> rows;
    IntVec playerPosition;

    bool inBounds(IntVec pos) const
    {
        const bool horizontal = pos.x >= 0 && pos.x < this->width();
        const bool vertical = pos.y >= 0 && pos.y < this->height();
        return horizontal && vertical;
    }

    Tile at(IntVec position) const
    {
        return static_cast<Tile>(this->rows[position.y][position.x]);
    }

    i32 width() const { return rows[0].size(); }

    i32 height() const { return rows.size(); }

    static Board hardcoded()
    {
        std::vector<std::string> rows = {
            "#####",
            "#.$ #",
            "#####",
        };
        IntVec player = {3, 1};

        return {rows, player};
    }
};
