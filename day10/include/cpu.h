#include <functional>
#include <vector>

class Cpu final
{
public:
    explicit Cpu();
    explicit Cpu(std::vector<int>& print_ticks);
    void run();

private:
    std::vector<int> const print_ticks_;
    int next_print_idx_;
    int tick_;
    int X_;
    int total_signal_strength_;
    bool draw_mode_;

    bool tick();

    void draw();

    std::function<bool (int)> getOp(std::string op);
};