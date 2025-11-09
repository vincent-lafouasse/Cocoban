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
    board.log();
}

namespace {
bool vectorContains(const std::vector<IntVec>& v, IntVec e)
{
    return std::ranges::find(v, e) != v.cend();
}

std::vector<IntVec> bfs(const Board& board, IntVec start)
{
    std::vector<IntVec> explored = {start};
    std::queue<IntVec> queue;
    queue.push(start);

    while (!queue.empty()) {
        IntVec e = queue.back();
        queue.pop();

        for (Direction d : Direction::all()) {
            IntVec candidate = e + d.asVec();
            if (vectorContains(explored, candidate)) {
                continue;
            }
            if (!board.inBounds(candidate) ||
                board.at(candidate) == Board::Wall) {
                continue;
            }

            queue.push(candidate);
            explored.push_back(candidate);
        }
    }

    return explored;
}

void subtractInPlace(std::vector<IntVec>& a, const std::vector<IntVec>& b)
{
    auto newEnd = std::remove_if(
        a.begin(), a.end(), [&](IntVec e) { return vectorContains(b, e); });

    a.erase(newEnd, a.end());
}
}  // namespace

void Game::computeInaccessible()
{
    std::vector<IntVec> candidates;

    for (i32 x = 0; x < board.width(); x++) {
        for (i32 y = 0; y < board.height(); y++) {
            if (board.at({x, y}) != Board::Wall) {
                candidates.push_back({x, y});
            }
        }
    }

    std::vector<IntVec> outside;

    while (!candidates.empty()) {
        IntVec randomElement = *candidates.cbegin();
        std::vector<IntVec> blob = bfs(board, randomElement);

        if (!vectorContains(blob, state.player)) {
            for (IntVec pos : blob)
                outside.push_back(pos);
        }

        subtractInPlace(candidates, blob);
    }

    for (IntVec pos : outside) {
        board.at(pos) = Board::Outside;
    }
}

void Game::update(Direction action)
{
    IntVec movement = action.asVec();
    IntVec newPosition = this->state.player + movement;

    if (!board.inBounds(newPosition) || board.at(newPosition) == Board::Wall) {
        return;
    }

    auto maybeBox = std::ranges::find(state.boxes, newPosition);
    if (maybeBox == state.boxes.cend()) {
        state.player = newPosition;
        return;
    }

    IntVec& box = *maybeBox;
    IntVec boxNewPosition = box + movement;
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
        std::plus<>(), [&](const IntVec& box) {
            return static_cast<u32>(board.at(box) == Board::Hole);
        });
}

void Game::log() const
{
    this->board.log();

    auto logPosition = [](const IntVec pos) {
        std::cerr << "{ " << pos.x << ", " << pos.y << " }";
    };

    std::cerr << "Player:\n\t";
    logPosition(this->state.player);
    std::cerr << '\n';

    std::cerr << "Boxes:\n";
    for (IntVec box : this->state.boxes) {
        std::cerr << '\t';
        logPosition(box);
        std::cerr << '\n';
    }
    std::cerr << '\n';
}

bool Game::hasBoxAt(IntVec position) const
{
    const auto& boxes = this->state.boxes;
    return std::ranges::find(boxes, position) != boxes.cend();
}
