#include <fstream>
#include <iostream>
#include <string>

#include <unordered_set>
#include <vector>

void task_1(){
    std::ifstream file("inputs.txt");
    std::string str; 
    int score{0};
    while (std::getline(file, str))
    {
        char shared;

        std::unordered_set<char> set_a;
        std::unordered_set<char> set_b;
        for (int i{0}; i < str.size()/2; i++){
            char a = str[i];
            char b = str[str.size()-1-i];
            
            set_a.emplace(a);
            set_b.emplace(b);
            if (set_a.find(b) != set_a.end()){
                shared = b;
                break;
            } else if (set_b.find(a) != set_b.end()) {
                shared = a;
                break;
            }

            
        }
        if (islower(shared)) {
            score += shared-96;
        } else {
            score += shared-38;
        }
    }
    std::cout << score << std::endl;
}

void task_2(){
    std::ifstream file("inputs.txt");
    std::string str;
    
    int score{0};
    int counter{-1};

    std::unordered_set<char> set_a;
    std::unordered_set<char> set_b;

    while (std::getline(file, str))
    {
        counter = (counter + 1) % 3;

        if (counter == 0) {
            for (char c : str){
                set_a.emplace(c);
            }
        } else if (counter == 1) {
            for (char c : str){
                if (set_a.find(c) != set_a.end()){
                    set_b.emplace(c);
                }
            }
        } else if (counter == 2){
            char common;
            for (char c : str){
                if (set_b.find(c) != set_b.end()){
                    common = c;
                    break;
                }
            }

            if (islower(common)) {
                score += common-96;
            } else {
                score += common-38;
            }
            set_a.clear();
            set_b.clear();
        }
    }
    std::cout << score << std::endl;
}

int main()
{
    task_2();
}