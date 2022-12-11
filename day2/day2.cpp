#include <fstream>
#include <iostream>
#include <string>

#include <unordered_map>

std::unordered_map<char,char> const char_to_int{
    {'A', 1}, {'B', 2}, {'C', 3},  // 3->0 in mod 3 
    {'X', 1}, {'Y', 2}, {'Z', 3}   // 3->0 in mod 3 
};

int calc_score(int opponent, int you)
{
    int score{you};
    if (opponent == you){
        // Tie
        score += 3;
    } else if ((opponent+1)%3 == you%3) {
        // Win
        score += 6;
    }
    return score;
}

void task_1(){
    std::ifstream file("inputs.txt");
    std::string str; 
    int score{0};
    while (std::getline(file, str))
    {
        int opponent = char_to_int.at(str[0]);
        int you = char_to_int.at(str[2]);

        score += calc_score(opponent, you);
    }
    std::cout << score << std::endl;
}

void task_2(){
    std::ifstream file("inputs.txt");
    std::string str; 
    int score{0};
    while (std::getline(file, str))
    {
        int opponent = char_to_int.at(str[0])%3;
        int outcome = char_to_int.at(str[2]) - 2;
        int you = (3 + opponent + outcome) % 3;  // -1%3=-1 use (3+(-1))%3=2 

        score += (you) ? you:3;
        score += (outcome+1) * 3;
    }
    std::cout << score << std::endl;
}

int main()
{
    task_2();
}