#include <memory>
#include <utility>
#include <vector>

constexpr int MAX_HEIGHT{'z'-'a'};
using Pos =std::pair<int, int>;
using Vector2d = std::vector<std::vector<int>>;

class SearchAlgo final
{
public:
    SearchAlgo(std::tuple<Vector2d, Pos, Pos> inputs);

    SearchAlgo (const SearchAlgo&) = delete;
    SearchAlgo& operator= (const SearchAlgo&) = delete;

    ~SearchAlgo() noexcept;

    int shortestPath();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};