#include "include/search_algo.h"

#include <queue>
#include <unordered_map>
#include <array>

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

class SearchAlgo::Impl
{
public:
    Impl(Vector2d grid, Pos start_pos, Pos end_pos):
        grid_{grid},
        start_pos_{start_pos},
        end_pos_{end_pos},
        visited{}
    {}

    int shortestPath()
    {
        const std::array<int, 4> row_shift{-1, 0, 1, 0};
        const std::array<int, 4> col_shift{0, 1, 0, -1};

        int row = start_pos_.first;
        int col = start_pos_.second;
        int dist = std::abs(end_pos_.first-row) + std::abs(end_pos_.second-col);
        pq.push(std::make_tuple(row, col, dist));

        while (!pq.empty() and !(row==end_pos_.first and col==end_pos_.second))
        {
            std::tie(row, col, dist) = pq.top();
            pq.pop();
            dist -= std::abs(end_pos_.first-row) + std::abs(end_pos_.second-col);
            visited[std::make_pair(row, col)] = dist;

            for (int i=0; i<4; i++)
            {
                int const tmp_row = row + row_shift[i];
                int const tmp_col = col + col_shift[i];
                bool is_visited = visited.find(std::make_pair(tmp_row, tmp_col)) != visited.end();
                if (tmp_row<0 or tmp_row>=grid_.size() or 
                    tmp_col<0 or tmp_col>=grid_[0].size() or 
                    is_visited or  
                    grid_[tmp_row][tmp_col] > grid_[row][col] + 1)
                {
                    continue;
                }
                int tmp_dist  = dist + 1 + std::abs(end_pos_.first-tmp_row) + std::abs(end_pos_.second-tmp_col);
                pq.push(std::make_tuple(tmp_row, tmp_col, tmp_dist));
            }
        }

        return dist;
    }


private:
    Vector2d grid_;
    Pos start_pos_;
    Pos end_pos_;
    std::unordered_map<Pos, int, pairhash> visited;

    using T=std::tuple<int, int, int>;
    struct customGreater{
        bool operator() (const T l, const T r) const { return std::get<2>(l) > std::get<2>(r); }
    };

    std::priority_queue<
        T,
        std::vector<T>,
        customGreater> pq;

    
};

SearchAlgo::SearchAlgo(std::tuple<Vector2d, Pos, Pos> inputs):
    impl_{std::make_unique<Impl>(
        std::get<0>(inputs), 
        std::get<1>(inputs), 
        std::get<2>(inputs))}
{}

int SearchAlgo::shortestPath()
{
    return impl_->shortestPath();
}


SearchAlgo::~SearchAlgo() = default;