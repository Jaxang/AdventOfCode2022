#include <fstream>
#include <iostream>
#include <string>

#include <algorithm>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <vector>

using Mask = std::vector<std::vector<bool>>;
using Array = std::vector<std::vector<int>>;
using Heighest = std::vector<std::vector<std::vector<int>>>;

struct Grid
{
public:
    Grid(Array in_grid): grid{std::move(in_grid)}
    {}

    static std::unique_ptr<Grid> initGrid(){
        int const n_rows = nRowsInFile();

        std::ifstream file("inputs.txt");
        std::string line;
        std::getline(file, line);
        int const n_cols = line.size();

        auto grid = std::make_unique<Grid>(Array(n_rows, std::vector<int>(n_cols, -1)));

        int idx = 0;
        initRow(grid->grid[idx], line);
        idx++;
        while(std::getline(file, line))
        {
            initRow(grid->grid[idx], line);
            idx++;
        }
        
        return std::move(grid);
    }

    size_t nRows() const{
        if (grid.empty()) {
            return 0;
        } else {
            return grid.size();
        }
    }

    size_t nCols() const{
        if (nRows() > 0) {
            return grid[0].size();
        } else {
            return 0;
        }
    }

    Array grid;

private:
    static inline int charDigitToInt(char const c)
    {
        if (!std::isdigit(c)) {
            std::cout << "Unexpected input: " << c << ". Expected a digit!" << std::endl;
            abort();
        }
        return c-'0';
    }

    static void initRow(std::vector<int>& row, std::string line){
        if (row.size() != line.size()) {
            std::cout << "The rows size (" << row.size() << ") didn't match the lines size (" << line.size() << ")" << std::endl;
            abort();
        }
        for (int i=0; i< line.size(); i++)
        {
            row[i] = charDigitToInt(line[i]);
        }
    }

    static int nRowsInFile()
    {

        std::ifstream file("inputs.txt");
        return std::count(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>(), '\n') + 1;
    }
};

class BFS
{
public:
    BFS():
        grid{std::move(Grid::initGrid())}
    {
        heighest_aroud = Heighest(
            grid->nRows(), 
            std::vector<std::vector<int>>(
                grid->nCols(), 
                std::vector<int>({-1, -1, -1, -1})
            )
        );
        n_visibil_trees = Heighest(heighest_aroud);

        tree_visibility = Mask(grid->nRows(), std::vector<bool>(grid->nCols(), false));

        for(int i=0; i<grid->nRows(); i++){
            tree_visibility[i][0]=true;
            tree_visibility[i][grid->nCols()-1]=true;
            for(int j=0; j<4; j++) {
                heighest_aroud[i][0][j]=grid->grid[i][0];
                heighest_aroud[i][grid->nCols()-1][j]=grid->grid[i][grid->nCols()-1];

                n_visibil_trees[i][0][j]=0;
                n_visibil_trees[i][grid->nCols()-1][j]=0;
            }
        }
        for(int i=1; i<grid->nCols()-1; i++){
            tree_visibility[0][i]=true;
            tree_visibility[grid->nRows()-1][i]=true;
            for(int j=0; j<4; j++) {
                heighest_aroud[0][i][j]=grid->grid[0][i];
                heighest_aroud[grid->nRows()-1][i][j]=grid->grid[grid->nRows()-1][i];

                n_visibil_trees[0][i][j]=0;
                n_visibil_trees[grid->nRows()-1][i][j]=0;
            }
        }
    }

    int nVisibleTreesFromOutside()
    {
        std::cout << "Init done" << std::endl;

        nVisibleTreesFromOutside_(1, 1);

        std::cout << "Search done" << std::endl;

        int total{0};
        for (auto const row: tree_visibility){
            for (bool const val: row){
                if (val) {total++;}
            }
        }
        std::cout << total << std::endl;
    }

    int nVisibleTreesFromTree()
    {
        std::cout << "Init done" << std::endl;
        
        std::cout << nVisibleTreesFromTree_(1, 1) << std::endl;
    }

private:
    std::unique_ptr<Grid> const grid;
    Mask tree_visibility;
    Heighest n_visibil_trees;
    Heighest heighest_aroud;

    void nVisibleTreesFromOutside_(int const cur_row, int const cur_col)
    {
        int const height = grid->grid[cur_row][cur_col];

        std::array<int, 4> static row_shift{-1,  0, 1, 0};
        std::array<int, 4> static col_shift{ 0, 1, 0, -1};

        std::vector<bool> visibility{false, false, false, false};
        std::vector<std::vector<int>> queue;
        for (int dir = 0; dir < 4; dir++){
            int const tmp_row{cur_row+row_shift[dir]};
            int const tmp_col{cur_col+col_shift[dir]};
            if (tmp_row < 0 or tmp_col < 0 or tmp_row>=grid->nRows() or tmp_col>=grid->nCols()) 
            {
                continue;
            }
            int const highest = heighest_aroud[tmp_row][tmp_col][dir];
            if (!checkVisibility_(highest, height, cur_row, cur_col, dir, visibility)){
                queue.push_back(std::vector<int>{tmp_row, tmp_col, dir});
            }
        }

        for (auto const& idxs : queue){
            int const tmp_row{idxs[0]};
            int const tmp_col{idxs[1]};
            int const dir{idxs[2]};

            // visit
            nVisibleTreesFromOutside_(cur_row+row_shift[dir], cur_col+col_shift[dir]);

            int const highest = heighest_aroud[tmp_row][tmp_col][dir];
            checkVisibility_(highest, height, cur_row, cur_col, dir, visibility);
        }
        
        tree_visibility[cur_row][cur_col] = visibility[0] or visibility[1] or visibility[2] or visibility[3];
    }
    
    bool checkVisibility_(
        int const highest, int const height, 
        int const cur_row, int const cur_col, int const dir,
        std::vector<bool>& visibility)
    {
        if (highest==-1) {
            // not visited
            return false;
        } else if (highest < height) {
            visibility[dir] = true;
            heighest_aroud[cur_row][cur_col][dir] = height;
        } else {
            visibility[dir] = false;
            heighest_aroud[cur_row][cur_col][dir] = highest;
        }
        return true;
    }

    

    int nVisibleTreesFromTree_(int const cur_row, int const cur_col)
    {
        int best_score{0};
        int const height = grid->grid[cur_row][cur_col];

        std::array<int, 4> static row_shift{-1,  0, 1, 0};
        std::array<int, 4> static col_shift{ 0, 1, 0, -1};

        std::vector<int>& visibility = n_visibil_trees[cur_row][cur_col];
        std::vector<std::vector<int>> queue;
        for (int dir = 0; dir < 4; dir++){
            int tmp_row{cur_row+row_shift[dir]};
            int tmp_col{cur_col+col_shift[dir]};
            if (tmp_row < 0 or tmp_col < 0 or tmp_row>=grid->nRows() or tmp_col>=grid->nCols()) 
            {
                continue;
            }
            int other_height = grid->grid[tmp_row][tmp_col];
            int other_visbility = n_visibil_trees[tmp_row][tmp_col][dir];

            if (n_visibil_trees[tmp_row][tmp_col][dir]==-1) {
                // not visited
                queue.push_back(std::vector<int>{tmp_row, tmp_col, dir});
            } else {
                visibility[dir] = 1;
                while(height > other_height and other_visbility>0){
                    visibility[dir] += other_visbility;
                    tmp_row += other_visbility*row_shift[dir];
                    tmp_col += other_visbility*col_shift[dir];
                    other_height = grid->grid[tmp_row][tmp_col];
                    other_visbility = n_visibil_trees[tmp_row][tmp_col][dir];
                }
            }
        }

        for (auto const& idxs : queue){
            int tmp_row{idxs[0]};
            int tmp_col{idxs[1]};
            int const dir{idxs[2]};

            // visit
            best_score = std::max(best_score, nVisibleTreesFromTree_(cur_row+row_shift[dir], cur_col+col_shift[dir]));

            visibility[dir] = 1;
            int other_height = grid->grid[tmp_row][tmp_col];
            int other_visbility = n_visibil_trees[tmp_row][tmp_col][dir];
            while(height > other_height and other_visbility>0){
                visibility[dir] += other_visbility;
                tmp_row += other_visbility*row_shift[dir];
                tmp_col += other_visbility*col_shift[dir];
                other_height = grid->grid[tmp_row][tmp_col];
                other_visbility = n_visibil_trees[tmp_row][tmp_col][dir];
            }
        }
        
        int score = visibility[0] * visibility[1] * visibility[2] * visibility[3];
        return std::max(best_score, score);
    }
};

void task_1()
{
    BFS impl{};
    impl.nVisibleTreesFromOutside();
}

void task_2()
{
    BFS impl{};
    impl.nVisibleTreesFromTree();
}

int main()
{
    task_1();
    task_2();
}