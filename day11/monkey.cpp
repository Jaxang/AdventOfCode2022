#include "include/monkey.h"

#include <fstream>
#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include <unordered_map>

Monkey::Monkey(
        std::vector<int64_t>& starting_items, 
        std::function<int64_t (int64_t)>& op, 
        std::array<int, 3>& test_vals,
        int worry_factor):
    items_{std::move(starting_items)},
    op_{std::move(op)},
    test_vals_{std::move(test_vals)},
    n_inspected_{0},
    worry_factor_{worry_factor}
{}

std::vector<std::pair<int, int64_t>> Monkey::takeTurn()
{
    std::vector<std::pair<int, int64_t>> output{};
    for (auto const& item : items_)
    {
        int64_t new_value = inspect(item);
        int64_t reciever = test(new_value);
        output.push_back(std::make_pair(reciever, new_value));
    }
    items_.resize(0);
    return output;
}

void Monkey::print() const
{
    std::cout << "Monkey : ";
    for (auto const& item : items_)
    {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
}

void Monkey::addItem(int64_t new_item)
{
    // new_item.convert(test_vals_[0]);
    items_.push_back(new_item);
}


int64_t Monkey::getNInpected() const
{
    return n_inspected_;
}

int64_t Monkey::getTestDivider()
{
    return test_vals_[0];
}

int64_t Monkey::inspect(int64_t old)
{
    n_inspected_++;
    return op_(old) / worry_factor_;
}

int64_t Monkey::test(int64_t val) const
{
    return (val%test_vals_[0]==0) ? test_vals_[1] : test_vals_[2];
}