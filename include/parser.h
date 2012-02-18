#ifndef _PARSER_H
#define _PARSER_H

#include "cgRender.h"
#include <string>

class parser
{
public:
    
    parser(std::string s);
    
    vtk_file* get_vtk_file() {return vtk;}
    
private:
    
    vtk_file* vtk;
    vtk_file* parse_vtk_file(std::string s);
    parser() {} // private constructor
};

#endif