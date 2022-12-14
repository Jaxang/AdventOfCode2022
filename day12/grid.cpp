#include "include/grid.h"

#include <fstream>
#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include <unordered_map>

Grid::Grid():
    impl_{std::make_unique<SearchAlgo>(initGrid())}
{}
 
int Grid::shortestPath()
{
    return impl_->shortestPath();
}

std::tuple<Vector2d, Pos, Pos> Grid::initGrid()
{
    Vector2d grid;
    Pos start_pos;
    Pos end_pos;

    std::ifstream file("inputs.txt");
    std::string str;

    int i_row{0};
    while (std::getline(file, str))
    {
        std::vector<int> row{};
        for (int i_col=0; i_col<str.size(); i_col++)
        {
            char const c = str[i_col];
            if (c == 'S'){
                start_pos = std::make_pair(i_row, i_col);
                row.push_back('a'-'a');
            } else if (c == 'E') {
                end_pos = std::make_pair(i_row, i_col);
                row.push_back('z'-'a');
            } else {
                row.push_back(str[i_col]-'a');
            }
        }
        grid.push_back(row);
        i_row++;
    }

    return std::make_tuple(grid, start_pos, end_pos);
}

Grid::~Grid() = default;