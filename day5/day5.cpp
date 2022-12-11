#include <fstream>
#include <iostream>
#include <string>

#include <vector>

using stack=std::vector<char>;

std::vector<stack> read_init(std::ifstream& file)
{
    std::vector<std::string> lines;
    std::string str;

    while (std::getline(file, str))
    {
        if (str.empty()) {
            break;
        }
        lines.push_back(str);
    }

    int max_height = lines.size()-1;
    int width = lines[max_height].size();
    int n_stacks = lines[max_height][width-2] - '0';
    std::vector<stack> stacks(n_stacks, stack());

    int idx = 1;
    for (int j=0; j<n_stacks; j++){
        int idx = 1 + 4*j;
        for (int i=max_height-1; i>=0; i--)
        {
            char c{lines[i][idx]};
            if (c==' '){
                break;
            } else {
                stacks[j].push_back(c);
            }
        }
    }

    return stacks;
}

std::vector<int> read_instructions(std::string str)
{
    std::vector<int> vals = {0, 0, 0};

    int start_idx = 5;
    int counter = 0;
    int spaces = 0;
    for (int i=start_idx+1; i<str.size(); i++)
    {
        char const c{str[i]};
        if (!std::isdigit(c)){
            vals[counter] = stoi(str.substr(start_idx, i-start_idx));
            start_idx=i+ ((counter) ? 4:6);
            i=start_idx;
            counter ++;
        }
    }
    vals[counter] = stoi(str.substr(start_idx, str.size()-start_idx));

    return vals;
}

void task_1()
{
    std::ifstream file("inputs.txt");
    std::string str; 

    std::vector<stack> stacks = read_init(file);

    while (std::getline(file, str))
    {
        std::vector<int> inst = read_instructions(str);

        stacks[inst[2]-1].reserve(stacks[inst[2]-1].size()+inst[0]);
        for (int i=0; i<inst[0]; i++)
        {
            stacks[inst[2]-1].push_back(stacks[inst[1]-1].back());
            stacks[inst[1]-1].pop_back();
        }
    }

    for (int i=0; i<stacks.size(); i++)
    {
        std::cout << stacks[i].back();
    }
    std::cout << std::endl;
}

void task_2()
{
    std::ifstream file("inputs.txt");
    std::string str; 

    std::vector<stack> stacks = read_init(file);

    while (std::getline(file, str))
    {
        std::vector<int> inst = read_instructions(str);

        stacks[inst[2]-1].reserve(stacks[inst[2]-1].size()+inst[0]);
        for (int i=stacks[inst[1]-1].size()-inst[0]; i<stacks[inst[1]-1].size(); i++)
        {
            stacks[inst[2]-1].push_back(stacks[inst[1]-1][i]);
        }
        stacks[inst[1]-1].resize(stacks[inst[1]-1].size()-inst[0]);
    }

    for (int i=0; i<stacks.size(); i++)
    {
        std::cout << stacks[i].back();
    }
    std::cout << std::endl;
}

int main()
{
    task_1();
    task_2();
}