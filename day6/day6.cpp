#include <fstream>
#include <iostream>
#include <string>

#include <array>

template <std::size_t N>
inline int find(std::array<char, N> const& buffer, int const start, int const size, char const c)
{
    for (int i=0; i<size; i++)
    {
        int const j{(start + i) % N};
        if (buffer[j]==c) {
            return i;
        }
    }
    return -1;
}

template <std::size_t N>
int solution(int idx=0)
{
    std::ifstream file("inputs.txt");
    std::string str; 
    std::getline(file, str);

    std::array<char, N> buffer{};
    int start{0};
    int size{0};
    while (size<N)
    {
        char const c = str[idx];
        int in_buffer{find(buffer, start, size, c)};

        buffer[(start+size)%N] = c;
        size -= in_buffer;
        start = (start + in_buffer + 1) % N;
        idx++;
    }
    std::cout << "First packet: '";
    for (int i=0; i<N; i++) {
        std::cout << buffer[(start+i)%N];
    }
    std::cout << "', after " << idx << " chars." << std::endl;
    return idx;
}

int task_1()
{
    return solution<4>();
}

int task_2(int start_idx)
{
    return solution<14>(start_idx);
}

int main()
{
    int final = task_2(task_1() - 4);
}