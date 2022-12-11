#include "cpu.h"

void task_1()
{
    std::vector<int> print_ticks{20, 60, 100, 140, 180, 220};
    Cpu cpu{print_ticks};
    cpu.run();
}

void task_2()
{
    Cpu cpu{};
    cpu.run();
}

int main()
{
    task_1();
    task_2();
}