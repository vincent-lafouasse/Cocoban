#include "geometry.hpp"

#include <sstream>

IntVec IntVec::up() const
{
    return {x, y - 1};
}
IntVec IntVec::down() const
{
    return {x, y + 1};
}
IntVec IntVec::left() const
{
    return {x - 1, y};
}
IntVec IntVec::right() const
{
    return {x + 1, y};
}

IntVec IntVec::scaleUp(i32 scale) const
{
    return {scale * x, scale * y};
}

IntVec IntVec::operator+(const IntVec& other) const
{
    return {x + other.x, y + other.y};
}

bool IntVec::operator==(const IntVec& other) const
{
    return x == other.x && y == other.y;
}

bool IntVec::operator!=(const IntVec& other) const
{
    return !(*this == other);
}

std::string IntVec::str() const
{
    std::stringstream ss;

    ss << "{ " << this->x << ", " << this->y << " }";
    return ss.str();
}

Direction::Direction(Inner inner) : inner(inner) {}

IntVec Direction::asVec() const
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

std::array<Direction, 4> Direction::all()
{
    return {Up, Down, Left, Right};
}
