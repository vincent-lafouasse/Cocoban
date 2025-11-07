#pragma once

#include <vector>

#include "geometry.hpp"
#include "ints.hpp"

struct Board {
    using Tile = char;
    static constexpr Tile Wall = '#';
    static constexpr Tile Empty = ' ';
    static constexpr Tile Player = '@';
    static constexpr Tile Token = '$';
    static constexpr Tile Hole = '.';

    std::vector<std::string> rows;

    bool inBounds(IntVec pos) const
    {
        const bool horizontal = pos.x >= 0 && pos.x < this->width();
        const bool vertical = pos.y >= 0 && pos.y < this->height();
        return horizontal && vertical;
    }

    Tile at(IntVec position) const
    {
        return this->rows[position.y][position.x];
    }

    Tile& at(IntVec position) { return this->rows[position.y][position.x]; }

    i32 width() const { return rows[0].size(); }

    i32 height() const { return rows.size(); }

    static Board hardcoded()
    {
        std::vector<std::string> rows = {
            "#####",
            "#.$@#",
            "#####",
        };
        return {rows};
    }
};
