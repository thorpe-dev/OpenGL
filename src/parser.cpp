#include <boost/tokenizer.hpp>
#include <iostream>
#include <vector>
#include <string>

namespace parser
{
        bool parse_vtk_file(std::iterator First, std::iterator Last)
        {
            return false;
        }
}

int main ()
{
    std::string str = "hello\n";

    if (parser::parse_vtk_file(str.begin(),str.end()))
        std::cout << "lol";
    else
        std::cout << "roflmao";
}
