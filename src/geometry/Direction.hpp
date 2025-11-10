#pragma once

#include <array>

#include "IntVec.hpp"

struct Direction {
    enum Inner {
        Up,
        Down,
        Left,
        Right,
    } inner;

    Direction(Inner inner);  // NOLINT(*-explicit-constructor)

    [[nodiscard]] IntVec asVec() const;
    [[nodiscard]] static std::array<Direction, 4> all();
};
