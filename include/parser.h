#ifndef _PARSER_H
#define _PARSER_H

#include <string>
#include <vector>

using namespace std;

struct vtk_file
{
    int point_count;
    int polygon_size;
    int polygon_no;

    vector<float>* points;
    vector<short>* polygons;
    vector<float>* point_data;
};

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