#pragma once

#include <vector>

#include "geometry.hpp"
#include "ints.hpp"

struct Board {
    static constexpr char wall = '#';
    static constexpr char empty = ' ';
    static constexpr char player = '@';
    static constexpr char token = '$';
    static constexpr char hole = '.';

    std::vector<std::string> rows;
    i32 width;
    i32 height;
    IntVec playerPosition;

    bool inBounds(IntVec pos) const
    {
        const bool horizontal = pos.x >= 0 && pos.x < this->width;
        const bool vertical = pos.y >= 0 && pos.y < this->height;
        return horizontal && vertical;
    }

    static Board hardcoded()
    {
        std::vector<std::string> rows = {
            "#####",
            "#.$ #",
            "#####",
        };
        IntVec player = {3, 1};

        i32 width = rows[0].size();
        i32 height = rows.size();

        return {rows, width, height, player};
    }
};
