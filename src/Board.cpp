#include "Board.hpp"

#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>

namespace {
void padToSameLength(std::vector<std::string>& lines)
{
    u32 maxLen = std::transform_reduce(
        lines.cbegin(), lines.cend(), static_cast<u32>(0),
        [](u32 acc, u32 e) { return std::max(acc, e); },
        [](const std::string& line) { return line.size(); });

    for (std::string& line : lines) {
        line += std::string(maxLen - line.size(), ' ');
    }
}
}  // namespace

Board Board::load(const std::string& path)
{
    std::ifstream input(path);
    if (!input.good()) {
        std::cout << std::format("Failed to open file {}", path);
        std::cerr << std::endl;
        std::exit(1);
    }

    // append a blank line at the top for drawing stuff
    std::vector<std::string> data = {""};

    for (std::string line; std::getline(input, line);) {
        data.push_back(line);
    }
    padToSameLength(data);

    return {data};
}

Board::Tile Board::at(Position position) const
{
    return this->data[position.y][position.x];
}

Board::Tile& Board::at(Position position)
{
    return this->data[position.y][position.x];
}

i32 Board::width() const
{
    return static_cast<i32>(data[0].size());
}

i32 Board::height() const
{
    return static_cast<i32>(data.size());
}

bool Board::inBounds(Position pos) const
{
    const bool horizontal = pos.x >= 0 && pos.x < this->width();
    const bool vertical = pos.y >= 0 && pos.y < this->height();
    return horizontal && vertical;
}

void Board::log() const
{
    for (const auto& line : data) {
        std::cout << std::quoted(line) << std::endl;
    }
}

std::string Board::tileRepr(Tile t)
{
    switch (t) {
        case Wall:
            return "Wall";
        case Empty:
            return "Empty";
        case Player:
            return "Player";
        case Box:
            return "Box";
        case Hole:
            return "Hole";
        case Outside:
            return "Outside";
        default:
            return std::format("Unknown: {}", t);
    }
}
