#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "geometry.hpp"
#include "ints.hpp"

struct Board {
    using Tile = char;
    static constexpr Tile Wall = '#';
    static constexpr Tile Empty = ' ';
    static constexpr Tile Player = '@';
    static constexpr Tile Box = '$';
    static constexpr Tile Hole = '.';

    std::vector<std::string> data;

    static Board load(const std::string& path)
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

    Tile at(IntVec position) const
    {
        return this->data[position.y][position.x];
    }

    Tile& at(IntVec position) { return this->data[position.y][position.x]; }

    i32 width() const { return data[0].size(); }

    i32 height() const { return data.size(); }

    bool inBounds(IntVec pos) const
    {
        const bool horizontal = pos.x >= 0 && pos.x < this->width();
        const bool vertical = pos.y >= 0 && pos.y < this->height();
        return horizontal && vertical;
    }

    void log() const
    {
        for (const auto& line : data) {
            std::cout << line << std::endl;
        }
    }
};
