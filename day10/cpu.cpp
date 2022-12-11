#include "cpu.h"

#include <fstream>
#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include <unordered_map>

int opToTick(std::string op)
{
    std::unordered_map<std::string, int> const map{{"noop", 1}, {"addx", 2}};
    return map.at(op);
}

Cpu::Cpu():
    print_ticks_{},
    next_print_idx_{0},
    tick_{0},
    X_{1},
    total_signal_strength_{0},
    draw_mode_{true}
{}


Cpu::Cpu(std::vector<int>& print_ticks):
    print_ticks_{std::move(print_ticks)},
    next_print_idx_{0},
    tick_{0},
    X_{1},
    total_signal_strength_{0},
    draw_mode_{false}
{}

void Cpu::run()
{
    std::ifstream file("inputs.txt");
    std::string str;

    while (std::getline(file, str))
    {
        std::string op = str.substr(0,4); 

        int val{0};
        if (str.size() > 4){
            val = stoi(str.substr(5,str.size()));
        }

        auto cmd = getOp(op);
        if (!cmd(val))
        {
            std::cout << "Done: " << total_signal_strength_ << std::endl;
            return;
        }
    }
    std::cout << "Never reached end: " << total_signal_strength_ << std::endl;
}

bool Cpu::tick()
{
    tick_++;
    if (draw_mode_) {
        draw();
        return true;
    } else {
        if (tick_ == print_ticks_[next_print_idx_]) {
            int signal_strenght = tick_*X_;
            total_signal_strength_ += signal_strenght;
            std::cout << "During tick " << tick_ << 
                        ": X=" << X_ << 
                        " and the signal strength is " << signal_strenght << 
                        " with the current sum " << total_signal_strength_ <<  std::endl;
            next_print_idx_++;
        }
        return next_print_idx_!=print_ticks_.size();
    }
    
}

void Cpu::draw()
{
    int draw_idx = (tick_-1)%40;
    if (X_-1 <= draw_idx and draw_idx <= X_+1)
    {
        std::cout << "#";
    } else {
        std::cout << ".";
    }
    if (draw_idx == 39) {
        std::cout << std::endl;
    }
}

std::function<bool (int)> Cpu::getOp(std::string op)
{
    if (op=="noop") {
        return [=](int val){
            static_cast<void>(val);
            return tick();
        };
    } else if ("addx") {
        return [=](int val){
            if (!tick()){return false;}
            if (!tick()){return false;}
            X_+=val;
            return true;
        };
    } else {
        std::cout << "Unknown operation " << op <<std::endl;
        abort();
    }
}