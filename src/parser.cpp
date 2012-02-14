#include <boost/spirit/include/qi.hpp>
#include <iostream>
#include <vector>
#include <string>

namespace parser
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    template<typename Iterator>
        bool parse_vtk_file(Iterator First, Iterator Last)
        {
            using qi::double_;
            using qi::phrase_parse;
            using ascii::space;

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
