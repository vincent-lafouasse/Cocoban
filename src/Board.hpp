#pragma once

#include <vector>
#include <string>

#include "geometry.hpp"
#include "ints.hpp"

struct Board {
    using Tile = char;
    static constexpr Tile Wall = '#';
    static constexpr Tile Empty = ' ';
    static constexpr Tile Player = '@';
    static constexpr Tile Box = '$';
    static constexpr Tile Hole = '.';

    std::vector<std::string> data;

    static Board load(const std::string& path);

    Tile at(IntVec position) const;
    Tile& at(IntVec position);

    i32 width() const;
    i32 height() const;
    bool inBounds(IntVec pos) const;

    void log() const;
};
