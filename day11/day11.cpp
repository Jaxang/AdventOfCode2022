#include "include/keep_away.h"

void task_1()
{
    KeepAway keep_away(20, 3);
    keep_away.run();
}

void task_2()
{
    KeepAway keep_away(10000, 1);
    keep_away.run();
}

int main()
{
    task_1();
    task_2();
}