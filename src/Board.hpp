#pragma once

#include <string>
#include <vector>

#include "geometry.hpp"
#include "ints.hpp"

struct Board {
    using Tile = char;
    using Position = IntVec;
    static constexpr Tile Wall = '#';
    static constexpr Tile Empty = ' ';
    static constexpr Tile Player = '@';
    static constexpr Tile Box = '$';
    static constexpr Tile Hole = '.';
    static constexpr Tile Outside = '+';

    std::vector<std::string> data;

    static Board load(const std::string& path);

    [[nodiscard]] Tile at(Position position) const;
    [[nodiscard]] Tile& at(Position position);

    [[nodiscard]] i32 width() const;
    [[nodiscard]] i32 height() const;
    [[nodiscard]] bool inBounds(Position pos) const;

    [[nodiscard]] static std::string tileRepr(Tile t);

    void log() const;
};
