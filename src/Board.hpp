#pragma once

#include <string>
#include <vector>

#include "geometry.hpp"
#include "ints.hpp"

struct Board {
    using Tile = char;
    static constexpr Tile Wall = '#';
    static constexpr Tile Empty = ' ';
    static constexpr Tile Player = '@';
    static constexpr Tile Box = '$';
    static constexpr Tile Hole = '.';
    static constexpr Tile Outside = '+';

    std::vector<std::string> data;

    static Board load(const std::string& path);

    [[nodiscard]] Tile at(IntVec position) const;
    Tile& at(IntVec position);

    [[nodiscard]] i32 width() const;
    [[nodiscard]] i32 height() const;
    [[nodiscard]] bool inBounds(IntVec pos) const;

    void log() const;
};
