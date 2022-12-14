#include <array>
#include <functional>
#include <vector>
#include <cmath>
/*
struct Item
{
public:
    Item(int const value, int const mod):
        whole_{value/mod},
        remainder_{value%mod},
        mod_{mod}
    {}

    void convert(int const new_mod)
    {
        int new_mod_d = static_cast<double>(new_mod);
        int tmp_whole = std::floor((whole_/new_mod_d)*mod_ + remainder_/ new_mod_d)
        int tmp_remainder = ((whole_%new_mod)*(mod_%new_mod)%new_mod + remainder_% new_mod_d)%new_mod;

        whole_ = tmp_whole;
        remainder_ = tmp_remainder;
        mod_ = new_mod;
    }

    Item& operator+=(const int rhs) // compound assignment (does not need to be a member,
    {                           // but often is, to modify the private members)
        int tmp = remainder_ + rhs;
        remainder_ = tmp%mod_;
        whole_+=tmp/mod_;
        return *this; // return the result by reference
    }

    // friends defined inside class body are inline and are hidden from non-ADL lookup
    friend Item operator+(Item lhs,        // passing lhs by value helps optimize chained a+b+c
                       const int rhs) // otherwise, both parameters may be const references
    {
        lhs += rhs; // reuse compound assignment
        return lhs; // return the result by value (uses move constructor)
    }

    Item& operator*=(const int rhs) // compound assignment (does not need to be a member,
    {                           // but often is, to modify the private members)
        int tmp = remainder_ * rhs;
        remainder_ = tmp%mod_;
        whole_+=tmp/mod_;
        return *this; // return the result by reference
    }

    // friends defined inside class body are inline and are hidden from non-ADL lookup
    friend Item operator*(Item lhs,        // passing lhs by value helps optimize chained a+b+c
                       const int rhs) // otherwise, both parameters may be const references
    {
        lhs += rhs; // reuse compound assignment
        return lhs; // return the result by value (uses move constructor)
    }

    Item& operator/=(const int rhs) // compound assignment (does not need to be a member,
    {                           // but often is, to modify the private members)
        double tmp_rhs = static_cast<double>(rhs);
        int tmp_remainder = remainder_ / rhs;
        int tmp_whole = whole_ / rhs;
        remainder_ = tmp%mod_;
        whole_+=tmp/mod_;
        return *this; // return the result by reference
    }

    // friends defined inside class body are inline and are hidden from non-ADL lookup
    friend Item operator/(Item lhs,        // passing lhs by value helps optimize chained a+b+c
                       const int rhs) // otherwise, both parameters may be const references
    {
        lhs += rhs; // reuse compound assignment
        return lhs; // return the result by value (uses move constructor)
    }

    int remainder() const
    {
        return remainder_;
    }

    int value() const
    {
        return whole_*mod_ + remainder_;
    }

private:
    int whole_;
    int remainder_;
    int mod_;

};
*/
class Monkey final
{
public:
    explicit Monkey(
        std::vector<int64_t>& starting_items, 
        std::function<int64_t (int64_t)>& op, 
        std::array<int, 3>& test_vals,
        int worry_factor);
    
    std::vector<std::pair<int, int64_t>> takeTurn();

    void addItem(int64_t new_item);

    int64_t getTestDivider();

    void print() const;

    int64_t getNInpected() const;

private:
    std::vector<int64_t> items_;
    std::function<int64_t (int64_t)> op_;
    std::array<int, 3> test_vals_;
    int worry_factor_;
    int n_inspected_;

    int64_t inspect(int64_t old);

    int64_t test(int64_t val) const;
};