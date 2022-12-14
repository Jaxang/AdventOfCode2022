#include "include/keep_away.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>

KeepAway::KeepAway(int n_rounds, int worry_level):
    n_rounds_{n_rounds},
    worry_level_{worry_level},
    gcd_{1},
    monkeys_{}
{
    load_monkeys(monkeys_);
    getGcd();
}

std::function<int64_t (int64_t, int64_t)> KeepAway::getOperation(char c)
{
    if (c=='*'){
        return [](int64_t a, int64_t b){return a*b;};
    } else if (c=='+'){
        return [](int64_t a, int64_t b){return a+b;};
    } else {
        std::cout << "Unrecognized operation: " << c << std::endl; 
        abort();
    }
}


void KeepAway::load_monkeys(std::vector<Monkey>& monkeys)
{
    std::ifstream file("inputs.txt");
    std::string str;

    while (std::getline(file, str)){
        if (str.empty() and str.size()==0) {continue;}

        if (str.substr(0, 6)=="Monkey"){
            std::vector<int64_t> starting_items{};
            {
                std::getline(file, str);
                std::stringstream sstr(str.substr(18, str.size()));
                std::string val_str;
                while (std::getline(sstr, val_str, ','))
                {
                    starting_items.push_back(stoi(val_str));
                }
            }

            std::function<int64_t (int64_t)> op;
            {
                std::getline(file, str);
                auto operation = getOperation(str[23]);
                std::string val = str.substr(25, str.size());
                
                op = [operation, val](int64_t old)
                {
                    return operation(old, (isdigit(val[0])) ? stoi(val) : old);
                };
            }

            std::array<int, 3> test_vals;
            {
                std::getline(file, str);
                test_vals[0] = stoi(str.substr(21, str.size()));

                std::getline(file, str);
                test_vals[1] = stoi(str.substr(29, str.size()));
                
                std::getline(file, str);
                test_vals[2] = stoi(str.substr(30, str.size()));
            }


            monkeys.push_back(Monkey{starting_items, op, test_vals, worry_level_});
        } else {
            std::cout << "Error" << std::endl;
            abort();
        }
    }
}

void KeepAway::round(std::vector<Monkey>& monkeys)
{
    for (auto& monkey: monkeys)
    {
        for (std::pair<int, int64_t> item_pair : monkey.takeTurn())
        {
            monkeys[item_pair.first].addItem(item_pair.second%gcd_);
        }
    }
}

void KeepAway::run()
{
    
    for(int i=0; i<n_rounds_; i++)
    {
        round(monkeys_);
    }

    std::priority_queue<int64_t> pqueue;
    for (auto const& monkey : monkeys_)
    {
        pqueue.push(monkey.getNInpected());
    }

    int n_most_active{2};
    int64_t monkey_business{1};
    for (int i=0; i<n_most_active; i++)
    {
        monkey_business *= pqueue.top();
        pqueue.pop();
    }
    std::cout << monkey_business << std::endl;
}

void KeepAway::getGcd()
{
    for (auto  monkey : monkeys_)
    {
        if (gcd(gcd_, monkey.getTestDivider()) == 1)
        {
            gcd_ *= monkey.getTestDivider();
        }
    }
}

int64_t KeepAway::gcd(int64_t a, int64_t b)
{
    if (a == 0)
    {
        return b;
    }
    return gcd(b % a, a);
}