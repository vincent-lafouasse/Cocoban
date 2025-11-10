#pragma once

#include <array>

#include "ints.hpp"

struct IntVec {
    i32 x;
    i32 y;

    [[nodiscard]] IntVec up() const;
    [[nodiscard]] IntVec down() const;
    [[nodiscard]] IntVec left() const;
    [[nodiscard]] IntVec right() const;

    [[nodiscard]] IntVec scaleUp(i32 scale) const;

    [[nodiscard]] std::string str() const;

    IntVec operator+(const IntVec& other) const;
    bool operator==(const IntVec& other) const;
    bool operator!=(const IntVec& other) const;
};

struct Direction {
    enum Inner {
        Up,
        Down,
        Left,
        Right,
    } inner;

    Direction(Inner inner);  // NOLINT(*-explicit-constructor)

    [[nodiscard]] IntVec asVec() const;
    static std::array<Direction, 4> all();
};
