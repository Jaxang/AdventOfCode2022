#include <fstream>
#include <iostream>
#include <string>

#include <vector>

std::vector<int> read_input(std::string str)
{
    std::vector<int> vals = {0, 0, 0, 0};

    int start_idx = 0;
    int counter = 0;
    for (int i=0; i<str.size(); i++)
    {
        if (str[i]=='-' or str[i]==','){
            vals[counter] = stoi(str.substr(start_idx, i-start_idx));
            start_idx=i+1;
            counter ++;
        }
    }
    vals[counter] = stoi(str.substr(start_idx, str.size()-start_idx));

    return vals;
}

int encapsulates(int low1, int high1, int low2, int high2)
{
    if ((low1<=low2 and high1>=high2) or (low2<=low1 and high2>=high1))
    {
        return 1;
    } else {
        return 0;
    }
}

int overlaps(int low1, int high1, int low2, int high2)
{
    if (low1<low2) {
        return (high1>=low2) ? 1:0;
    } else if (low1<=high2) {
        return 1;
    } else {
        return 0;
    }
}

void task_1()
{
    std::ifstream file("inputs.txt");
    std::string str; 
    int score{0};

    while (std::getline(file, str))
    {
        std::vector<int> const vals = read_input(str);

        score += encapsulates(vals[0], vals[1], vals[2], vals[3]);
    }
    std::cout << score << std::endl;
}

void task_2()
{
    std::ifstream file("inputs.txt");
    std::string str; 
    int score{0};

    while (std::getline(file, str))
    {
        std::vector<int> const vals = read_input(str);

        score += overlaps(vals[0], vals[1], vals[2], vals[3]);
    }
    std::cout << score << std::endl;
}

int main()
{
    //task_1();
    task_2();
}