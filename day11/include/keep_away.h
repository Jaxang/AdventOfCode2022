#include "monkey.h"

#include <functional>
#include <vector>

class KeepAway final
{
public:
    explicit KeepAway(int n_rounds, int worry_level);

    void run();
    
private:
    int n_rounds_;
    int worry_level_;
    int64_t gcd_;
    std::vector<Monkey> monkeys_;

    std::function<int64_t (int64_t, int64_t)> getOperation(char c);

    void load_monkeys(std::vector<Monkey>& monkeys);

    void round(std::vector<Monkey>& monkeys);

    void getGcd();

    int64_t gcd(int64_t a, int64_t b);
};