#pragma once

#include <array>

#include "ints.hpp"

struct IntVec {
    i32 x;
    i32 y;

    IntVec up() const;
    IntVec down() const;
    IntVec left() const;
    IntVec right() const;

    IntVec scaleUp(i32 scale) const;

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

    Direction(Inner inner);

    IntVec asVec() const;
    static std::array<Direction, 4> all();
};
