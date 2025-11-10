#include "Game.hpp"

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
    this->board.log();
}

namespace {
using Position = Game::Position;

bool vectorContains(const std::vector<Position>& v, Position e)
{
    return std::ranges::find(v, e) != v.cend();
}

std::vector<Position> bfs(const Board& board, Position start)
{
    std::vector<Position> explored;
    std::queue<Position> queue;
    queue.push(start);

    while (!queue.empty()) {
        Position e = queue.back();
        explored.push_back(e);
        queue.pop();

        for (Direction d : Direction::all()) {
            Position candidate = e + d.asVec();
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

void subtractInPlace(std::vector<Position>& a, const std::vector<Position>& b)
{
    auto newEnd = std::remove_if(
        a.begin(), a.end(), [&](Position e) { return vectorContains(b, e); });

    a.erase(newEnd, a.end());
}
}  // namespace

void Game::computeInaccessible()
{
    std::vector<Position> candidates;

    for (i32 x = 0; x < board.width(); x++) {
        for (i32 y = 0; y < board.height(); y++) {
            if (board.at({x, y}) != Board::Wall) {
                candidates.push_back({x, y});
            }
        }
    }

    std::vector<Position> outside;

    while (!candidates.empty()) {
        Position randomElement = *candidates.cbegin();
        std::vector<Position> blob = bfs(board, randomElement);

        if (!vectorContains(blob, state.player)) {
            for (Position pos : blob)
                outside.push_back(pos);
        }

        subtractInPlace(candidates, blob);
    }

    for (Position pos : outside) {
        board.at(pos) = Board::Outside;
    }
}

void Game::update(Direction action)
{
    Position movement = action.asVec();
    Position newPosition = this->state.player + movement;

    if (!board.inBounds(newPosition) || board.at(newPosition) == Board::Wall) {
        return;
    }

    auto maybeBox = std::ranges::find(state.boxes, newPosition);
    if (maybeBox == state.boxes.cend()) {
        state.player = newPosition;
        return;
    }

    Position& box = *maybeBox;
    Position boxNewPosition = box + movement;
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

    auto logPosition = [](const Position pos) {
        std::cerr << "{ " << pos.x << ", " << pos.y << " }";
    };

    std::cerr << "Player:\n\t";
    logPosition(this->state.player);
    std::cerr << '\n';

    std::cerr << "Boxes:\n";
    for (Position box : this->state.boxes) {
        std::cerr << '\t';
        logPosition(box);
        std::cerr << '\n';
    }
    std::cerr << '\n';
}

bool Game::hasBoxAt(Position position) const
{
    const auto& boxes = this->state.boxes;
    return std::ranges::find(boxes, position) != boxes.cend();
}
