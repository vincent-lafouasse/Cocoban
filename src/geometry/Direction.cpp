#include "Direction.hpp"

Direction::Direction(Inner inner) : inner(inner) {}

IntVec Direction::asVec() const
{
    constexpr IntVec zero = {0, 0};

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

std::array<Direction, 4> Direction::all()
{
    return {Up, Down, Left, Right};
}
