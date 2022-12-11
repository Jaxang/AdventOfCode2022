#include <fstream>
#include <iostream>
#include <string>

#include <algorithm>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <vector>

std::unordered_map<char, int> const CHAR_TO_DIR{{'U', 0},{'R', 1},{'D', 2},{'L', 3}};
std::array<int, 8> const row_shift{-1, 0, 1,  0, -1, 1,  1, -1};  // U, R, D, L, UR, DR, DL, UL
std::array<int, 8> const col_shift{ 0, 1, 0, -1,  1, 1, -1, -1};  // U, R, D, L, UR, DR, DL, UL

struct Position
{
    Position(int row_, int col_): 
        row{row_}, col{col_} 
    {}

    Position relativePosition(Position other)
    {
        return Position(row-other.row, col-other.col);
    }

    void move(int dir, int steps)
    {
        row += row_shift[dir]*steps;
        col += col_shift[dir]*steps;
    }

    void move(int dir, int steps, std::vector<std::vector<bool>>& visited)
    {
        for (int i=0; i< steps; i++)
        {
            move(dir, 1);
            visited[row][col] = true;
        }

    }

    int row;
    int col;
};

static int toDir(int row, int col){
    for (int dir=0; dir <8; dir++)
    {
        if (row_shift[dir] == row and col_shift[dir] == col)
        {
            return dir;
        }
    }
    std::cout <<  "Unexpected input: " << row << "," << col << std::endl;
    abort();
}

void init(std::vector<std::pair<int, int>>& instructions,std::vector<std::vector<bool>>& grid)
{
    std::ifstream file("inputs.txt");
    std::string str;

    int idx{0};
    int row{0};
    int col{0};
    int max_row{0};
    int max_col{0};
    while (std::getline(file, str)){
        int dir = CHAR_TO_DIR.at(str[0]);
        int shift = stoi(str.substr(2, str.size()));
        instructions.push_back(std::make_pair(dir, shift));

        row += row_shift[dir]*shift;
        col += col_shift[dir]*shift;

        max_row = std::max(max_row, std::abs(row));
        max_col = std::max(max_col, std::abs(col));

        idx++;
    }

    grid = std::vector<std::vector<bool>>(max_row*2 + 1, std::vector<bool>(max_col*2 + 1, false));
}

void printGrid(std::vector<std::vector<bool>>& tail_visited, Position const& tail, Position const& head){
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    for (int row=tail_visited.size()/2-10; row <= tail_visited.size()/2+10; row++)
    {
        for (int col=tail_visited[0].size()/2-10; col <= tail_visited[0].size()/2+10; col++){
            if (head.col == col and head.row == row){
                std::cout << "H";
            } else if (tail.col == col and tail.row == row){
                std::cout << "T";
            } else if (tail_visited[row][col]){
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

void printGrid(std::vector<std::vector<bool>>& tail_visited, std::vector<Position> const& knots){
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    for (int row=tail_visited.size()/2-10; row <= tail_visited.size()/2+10; row++)
    {
        for (int col=tail_visited[0].size()/2-10; col <= tail_visited[0].size()/2+10; col++){
            if (knots[0].col == col and knots[0].row == row){
                std::cout << "H";
                continue;
            }

            bool occupied{false};
            for (int i=1; i<knots.size(); i++)
            {
                if (knots[i].col == col and knots[i].row == row){
                    std::cout << i;
                    occupied=true;
                    break;
                }
            }
            if (occupied) {continue;}

            if (tail_visited[row][col]){
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> step(
    Position& tail, Position& rel,
    int const shift, int const dir, 
    std::vector<std::vector<bool>>& tail_visited,
    bool update_visited=true) 
{
    if (shift==0) {return std::vector<std::vector<int>>{};}

    int const rshift=row_shift[dir];
    int const cshift=col_shift[dir];
    
    std::vector<std::vector<int>> moves;
    if (dir >= 4){
        // Move diagonal
        if (shift!=1) {
            std::cout << "Expected only one diagonal move" << std::endl;
            abort();
        }
        int dir_prod = rshift*rel.row + cshift*rel.col;
        
        Position new_rel{rel.row-rshift,rel.col-cshift};
        if (!(dir_prod > 0) and !(rel.row==0 and rel.col==0)){
            if (dir_prod==0) {
                // Off-diagonal compared to dir
                int tmp_dir = toDir((rshift-rel.row)/2, (cshift-rel.col)/2);
                if (update_visited){
                    tail.move(tmp_dir, 1, tail_visited);
                } else {
                    tail.move(tmp_dir, 1);
                }
                moves.push_back(std::vector<int>{tmp_dir, 1});

                new_rel.row += row_shift[tmp_dir];
                new_rel.col += col_shift[tmp_dir];
            } else if ( dir_prod == -1 or dir_prod == -2){
                if (update_visited){
                    tail.move(dir, 1, tail_visited);
                } else {
                    tail.move(dir, 1);
                }
                moves.push_back(std::vector<int>{dir, 1});

                // tail moves as head
                new_rel.row += rshift;
                new_rel.col += cshift;
            } else  {
                std::cout << "Should not happen 2, help!" << std::endl;
                abort();
            }
        }
        std::vector<std::vector<int>> tmp_moves = step(tail, new_rel, shift-1, dir, tail_visited, update_visited);
        moves.reserve(moves.size() + distance(tmp_moves.begin(),tmp_moves.end()));
        moves.insert(moves.end(),tmp_moves.begin(),tmp_moves.end());
    } else if (rshift!=0) {
        // Moving vertical
        if (rel.col ==0){
            // Tail in vertical line
            int shift_offset = rshift*rel.row + 1;
            if (update_visited){
                tail.move(dir, std::max(shift-shift_offset, 0), tail_visited);
            } else {
                tail.move(dir, std::max(shift-shift_offset, 0));
            }
            moves.push_back(std::vector<int>({dir, std::max(shift-shift_offset, 0)}));
        } else {
            int shift_offset = rshift*rel.row + 1;
            if (shift - shift_offset > 0) {
                // Move diag
                int diag_dir = toDir(rshift, -rel.col);
                if (update_visited){
                    tail.move(diag_dir, 1, tail_visited);
                } else {
                    tail.move(diag_dir, 1);
                }
                moves.push_back(std::vector<int>({diag_dir, 1}));

                // Tail behind head
                if (update_visited){
                    tail.move(dir, std::max(shift-shift_offset-1, 0), tail_visited);
                } else {
                    tail.move(dir, std::max(shift-shift_offset-1, 0));
                }
                moves.push_back(std::vector<int>({dir, std::max(shift-shift_offset-1, 0)}));
            }
        }
    } else if (cshift!=0) {
        // Moving vertical
        int shift_offset = cshift*rel.col + 1;
        if (rel.row ==0){
            // Tail in horizontal line
            if (update_visited){
                tail.move(dir, std::max(shift-shift_offset, 0), tail_visited);
            } else {
                tail.move(dir, std::max(shift-shift_offset, 0));
            }
            moves.push_back(std::vector<int>({dir, std::max(shift-shift_offset, 0)}));
        } else {
            if (shift - shift_offset > 0) {
                // Move diag
                int diag_dir = toDir(-rel.row, cshift);
                if (update_visited){
                    tail.move(diag_dir, 1, tail_visited);
                } else {
                    tail.move(diag_dir, 1);
                }
                moves.push_back(std::vector<int>({diag_dir, 1}));

                // Tail behind head
                if (update_visited){
                    tail.move(dir, std::max(shift-shift_offset-1, 0), tail_visited);
                } else {
                    tail.move(dir, std::max(shift-shift_offset-1, 0));
                }
                moves.push_back(std::vector<int>({dir, std::max(shift-shift_offset-1, 0)}));
            }
        }
    } else {
        std::cout << "Should not happen, help!" << std::endl;
        abort();
    }
    return moves;
}

void multi_step(
    std::vector<Position>& knots, 
    int const shift, int const dir, 
    std::vector<std::vector<bool>>& tail_visited)
{
    if (shift==0) {return;}
    int const rshift=row_shift[dir];
    int const cshift=col_shift[dir];

    std::vector<Position> rels;
    for (int i=0; i< knots.size()-1; i++){
        rels.push_back(knots[i+1].relativePosition(knots[i]));
    }


    Position prev_head_start(knots[0]);
    knots[0].move(dir, shift);
    std::vector<std::vector<int>> moves{{dir, shift}};
    for (int i=0; i< knots.size()-1; i++){
        Position next_head_start(knots[i+1]);

        std::vector<std::vector<int>> next_moves{};
        for (auto const& move : moves){
            std::vector<std::vector<int>> tmp_moves{{dir, shift}};
            prev_head_start.move(move[0], move[1]);
            tmp_moves= step(knots[i+1], rels[i], move[1], move[0], tail_visited, i==(knots.size()-2));
            rels[i] = knots[i+1].relativePosition(prev_head_start);

            next_moves.reserve(next_moves.size() + distance(tmp_moves.begin(),tmp_moves.end()));
            next_moves.insert(next_moves.end(),tmp_moves.begin(),tmp_moves.end());
        }
        prev_head_start = next_head_start;
        moves = next_moves;
    }
}

void task_1()
{
    std::vector<std::pair<int, int>> instructions;
    std::vector<std::vector<bool>> tail_visited;
    init(instructions, tail_visited);

    Position start = Position(tail_visited.size()/2, tail_visited[0].size()/2);
    std::vector<Position> knots;
    knots.push_back(Position(start));
    knots.push_back(Position(start));
    tail_visited[start.row][start.col] = true;

    int dir{0};
    int shift{0};
    for (auto const& instr_pair: instructions)
    {
        std::tie<int,int>(dir, shift) = instr_pair;
        
        multi_step (knots, shift, dir, tail_visited);

        printGrid(tail_visited, knots[1], knots[0]);
    }
    
    int total{0};
    for (auto const& row: tail_visited)
    {
        for (bool const val: row)
        {
            if (val) {total++;}
        }

    }
    std::cout << total << std::endl;

}

void task_2()
{
    std::vector<std::pair<int, int>> instructions;
    std::vector<std::vector<bool>> tail_visited;
    init(instructions, tail_visited);

    Position start = Position(tail_visited.size()/2, tail_visited[0].size()/2);
    std::vector<Position> knots;
    knots.push_back(Position(start));
    for(int i=0; i<9; i++){
        knots.push_back(Position(start));
    }
    tail_visited[start.row][start.col] = true;

    int dir{0};
    int shift{0};
    for (auto const& instr_pair: instructions)
    {
        std::tie<int,int>(dir, shift) = instr_pair;
        
        multi_step (knots, shift, dir, tail_visited);

        printGrid(tail_visited, knots);
    }
    
    int total{0};
    for (auto const& row: tail_visited)
    {
        for (bool const val: row)
        {
            if (val) {total++;}
        }

    }
    std::cout << total << std::endl;
}

int main()
{
    task_1();
    task_2();
}