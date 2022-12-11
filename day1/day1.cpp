#include <fstream>
#include <iostream>
#include <string>
#include <queue>

int simple()
{
    std::ifstream file("inputs.txt");
    std::string str; 
    int most{0};
    int tmp{0};
    while (std::getline(file, str))
    {
        if (!str.empty() and std::isdigit(str[0])){
            tmp += std::stoi(str);
        } else {
            if (tmp >  most){
                most = tmp;
            }
            tmp = 0;
        }
    }
    std::cout << most << std::endl;
}

int less_simple(int n_to_print)
{
    std::ifstream file("inputs.txt");
    std::string str; 
    std::priority_queue<int> pqueue;
    int tmp{0};
    while (std::getline(file, str))
    {
        if (!str.empty() and std::isdigit(str[0])){
            tmp += std::stoi(str);
        } else {
            pqueue.push(tmp);
            tmp = 0;
        }
    }

    int sum{0};
    for (int i=0; i<n_to_print; i++){
        sum += pqueue.top();
        std::cout << "Carrying: " << pqueue.top() << " Total: " << sum << std::endl;
        pqueue.pop();
        if (pqueue.empty()){
            break;
        }
    }
}


int main(){
    less_simple(3);
}
