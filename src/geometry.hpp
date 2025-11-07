#pragma once

#include <array>

#include "ints.hpp"

struct IntVec {
    i32 x;
    i32 y;

    IntVec up() const { return {x, y - 1}; }
    IntVec down() const { return {x, y + 1}; }
    IntVec left() const { return {x - 1, y}; }
    IntVec right() const { return {x + 1, y}; }

    IntVec scaleUp(i32 scale) const { return {scale * x, scale * y}; }

    IntVec operator+(const IntVec& other) const
    {
        return {x + other.x, y + other.y};
    }
};

struct Direction {
    enum Inner {
        Up,
        Down,
        Left,
        Right,
    };

    Direction(Inner inner) : inner(inner) {}

    IntVec asVec() const
    {
        IntVec zero = {0, 0};

        switch (inner) {
            case Up:
                return zero.up();
            case Down:
                return zero.down();
            case Left:
                return zero.left();
            case Right:
                return zero.right();
            default:
                return zero;
        }
    }

    static std::array<Direction, 4> all() { return {Up, Down, Left, Right}; }

    Inner inner;
};
