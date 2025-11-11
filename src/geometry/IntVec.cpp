#include "IntVec.hpp"

#include <format>

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
    return std::format("{{ {}, {} }}", x, y);
}

bool IntVec::operator<(const IntVec& other) const
{
    if (x < other.x)
        return true;
    if (x > other.x)
        return false;
    return y < other.y;
}
