#include "Game.hpp"

#include <algorithm>
#include <format>
#include <iostream>
#include <numeric>
#include <queue>

Game::Game(const Board& board) : board(board), state()
{
    for (i32 x = 0; x < board.width(); ++x) {
        for (i32 y = 0; y < board.height(); ++y) {
            Board::Tile& tile = this->board.at({x, y});

            if (tile == Board::Player) {
                this->state.player = {x, y};
                tile = Board::Empty;
            } else if (tile == Board::Box) {
                this->state.boxes.push_back({x, y});
                tile = Board::Empty;
            }
        }
    }

    this->computeInaccessible();
}

namespace {
using Position = Game::Position;

template <typename T>
bool vectorContains(const std::vector<T>& v, T e)
{
    return std::ranges::find(v, e) != v.cend();
}

std::vector<Position> bfs(const Board& board, Position start)
{
    std::vector<Position> explored;
    std::queue<Position> queue;
    queue.push(start);

    while (!queue.empty()) {
        const Position e = queue.front();
        explored.push_back(e);
        queue.pop();

        for (const Direction d : Direction::all()) {
            const Position candidate = e + d.asVec();
            if (vectorContains(explored, candidate)) {
                continue;
            }
            if (!board.inBounds(candidate) ||
                board.at(candidate) == Board::Wall) {
                continue;
            }

            queue.push(candidate);
        }
    }

    return explored;
}
}  // namespace

void Game::computeInaccessible()
{
    std::vector<Position> accessible = bfs(this->board, this->state.player);

    for (i32 x = 0; x < board.width(); x++) {
        for (i32 y = 0; y < board.height(); y++) {
            const Position pos = {x, y};
            Board::Tile& tile = board.at(pos);

            if (tile != Board::Wall && !vectorContains(accessible, pos)) {
                tile = Board::Outside;
            }
        }
    }
}

void Game::update(Direction action)
{
    const Position movement = action.asVec();
    const Position newPosition = this->state.player + movement;

    if (!board.inBounds(newPosition) || board.at(newPosition) == Board::Wall) {
        return;
    }

    const auto maybeBox = std::ranges::find(state.boxes, newPosition);
    if (maybeBox == state.boxes.cend()) {
        state.player = newPosition;
        return;
    }

    Position& box = *maybeBox;
    const Position boxNewPosition = box + movement;
    if (!board.inBounds(boxNewPosition) ||
        board.at(boxNewPosition) == Board::Wall || hasBoxAt(boxNewPosition)) {
        return;
    }

    box = boxNewPosition;
    state.player = newPosition;
}

[[nodiscard]] u32 Game::numberOfFilledHoles() const
{
    return std::transform_reduce(
        state.boxes.cbegin(), state.boxes.cend(), static_cast<u32>(0),
        std::plus<>(), [&](const Position& box) {
            return static_cast<u32>(board.at(box) == Board::Hole);
        });
}

void Game::log() const
{
    this->board.log();

    std::cerr << std::format("Player:\n\t{}\n", state.player.str());

    std::cerr << "Boxes:\n";
    for (Position box : this->state.boxes) {
        std::cerr << std::format("\t{}\n", box.str());
    }
    std::cerr << std::endl;
}

bool Game::hasBoxAt(Position position) const
{
    const auto& boxes = this->state.boxes;
    return std::ranges::find(boxes, position) != boxes.cend();
}
