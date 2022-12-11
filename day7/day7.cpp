#include <fstream>
#include <iostream>
#include <string>

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

struct File
{
    File(int size, std::string name): 
        size_{size}, 
        name_{name}
    {}

    int size_;
    std::string name_;
};

struct Dir
{

    Dir(std::string name): files_{}, dirs_{}, size_{-1}, init_{false}, name_{name}
    {}

    ~Dir() noexcept = default;

    std::string parse_ls(std::ifstream& file)
    {
        init_=true;
        std::string str;
        while (std::getline(file, str))
        {      
            if (str[0] == '$') {return str;}

            auto it = std::find(str.begin(), str.end(), ' ');

            std::string name(it+1, str.end());
            if (std::isdigit(str[0])){
                std::string file_size(str.begin(), it);
                
                files_.push_back(File{std::stoi(file_size), name});
            } else {
                dirs_[name] = std::make_shared<Dir>(name);
            }
        }
        return "";
    }

    int getSize()
    {
        if (!init_){
            return -1;
        } else if (size_>-1) {
            return size_;
        }

        int tmp_size{0};
        for (auto& dir_pair : dirs_){
            int const tmp = dir_pair.second->getSize();
            if (tmp == -1) {
                return -1;
            } else {
                tmp_size+= tmp;
            }
        }

        for (auto const& file: files_){
            tmp_size += file.size_;
        }
        size_=tmp_size;
        return tmp_size;
    }

    int getSizeBelow(int bellow){
        int this_size = getSize();

        int size_below{0};
        if (this_size <= bellow)
        {
            size_below += this_size;
        }

        for (auto const& dir_pair: dirs_)
        {
            size_below += dir_pair.second->getSizeBelow(bellow);
        }
        return size_below;
    }

    int smallestAbove(int needed_space){
        int this_size = getSize();

        int smallest  = 0;
        if (this_size >= needed_space){
            smallest = this_size;
            for (auto const& dir_pair: dirs_)
            {
                int tmp = dir_pair.second->smallestAbove(needed_space);
                if (tmp==0) {
                    continue;
                } else {
                    smallest = std::min(smallest, tmp);
                }
            }
        }
        
        return smallest;
    }

    std::vector<File> files_;
    std::unordered_map<std::string, std::shared_ptr<Dir>> dirs_;
    std::string name_;
    int size_;
    bool init_;

};

void parse_commands(std::ifstream& file, std::vector<std::shared_ptr<Dir>> trace)
{
    
    std::string command;
    std::getline(file, command);
    
    while(file) {
        if (command[0] != '$'){
            std::cout << "Not a command: " << command << std::endl;
            abort();
        }

        auto const& current = trace.back();
        if (command.substr(2, 2) == "cd") {
            if (command.size() ==7 and command.substr(5, 2) == ".."){
                trace.pop_back();
            } else {
                std::string new_dir = command.substr(5, command.size());
                trace.push_back(current->dirs_.at(new_dir));
            }
            std::getline(file, command);
        } else if (command.substr(2, 2) == "ls") {
            if (current->init_){
                std::getline(file, command);
            } else {
                command = current->parse_ls(file);
            }
        } else {
            std::cout << "Unknown command: " << command << std::endl;
            abort();
        }
    }
    
}


void task_1()
{
    std::ifstream file("inputs.txt");
    std::string str; 
    std::getline(file, str);
    if (str != "$ cd /") {
        std::cout << "unexpected first input: " << str << std::endl;
        abort();
    }
    std::shared_ptr<Dir> root= std::make_shared<Dir>("/");
    std::vector<std::shared_ptr<Dir>> trace{root};

    parse_commands(file, trace);

    std::cout << root->getSizeBelow(100000) << std::endl;

}

void task_2()
{
    std::ifstream file("inputs.txt");
    std::string str; 
    std::getline(file, str);
    if (str != "$ cd /") {
        std::cout << "unexpected first input: " << str << std::endl;
        abort();
    }
    std::shared_ptr<Dir> root= std::make_shared<Dir>("/");
    std::vector<std::shared_ptr<Dir>> trace{root};

    parse_commands(file, trace);

    int const max_size{70000000};
    int const free_space_needed{30000000};
    int const available_space{max_size-root->getSize()};
    int const remaining_freespace_needed{free_space_needed-available_space};

    std::cout << root->smallestAbove(remaining_freespace_needed) << std::endl;
}

int main()
{
    task_1();
    task_2();
}