#include "Board.hpp"

#include <fstream>
#include <iostream>

Board Board::load(const std::string& path)
{
    std::ifstream input(path);
    if (!input.good()) {
        std::cout << "Failed to open file " << path << std::endl;
    }

    std::vector<std::string> data;

    for (std::string line; std::getline(input, line);) {
        data.push_back(line);
    }

    return {data};
}

Board::Tile Board::at(IntVec position) const
{
    return this->data[position.y][position.x];
}

Board::Tile& Board::at(IntVec position)
{
    return this->data[position.y][position.x];
}

i32 Board::width() const
{
    return data[0].size();
}

i32 Board::height() const
{
    return data.size();
}

bool Board::inBounds(IntVec pos) const
{
    const bool horizontal = pos.x >= 0 && pos.x < this->width();
    const bool vertical = pos.y >= 0 && pos.y < this->height();
    return horizontal && vertical;
}

void Board::log() const
{
    for (const auto& line : data) {
        std::cout << line << std::endl;
    }
}
