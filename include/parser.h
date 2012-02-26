#ifndef _PARSER_H
#define _PARSER_H

#include <string>
#include <vector>

using namespace std;

struct vtk_file
{
    int point_count;
    int polygon_no;
    int texture_count;

    vector<float>* points;
    vector<int>* polygons;
    vector<float>* point_data;
};

struct ppm_file
{
    int width;
    int height;
    int max;
    int bytecount;
    char* texture;
};

class parser
{
public:
    
    parser(char* s,bool b);
    
    vtk_file* get_vtk_file() {return vtk;}
    ppm_file* get_ppm_file() {return ppm;}
    
private:
    
    vtk_file* vtk;
    ppm_file* ppm;
    vtk_file* parse_vtk_file(char* s);
    ppm_file* parse_ppm_file(char* s);
    parser() {} // private constructor
};

#endif