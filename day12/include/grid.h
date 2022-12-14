#include <vector>
#include <utility>
#include <memory>

#include "search_algo.h"


class Grid final
{
public:
    explicit Grid();

    Grid (const Grid&) = delete;
    Grid& operator= (const Grid&) = delete;

    ~Grid() noexcept;

    int shortestPath();

private:
    std::unique_ptr<SearchAlgo> impl_;

    std::tuple<Vector2d, Pos, Pos> initGrid();

    int dijkstras();

};