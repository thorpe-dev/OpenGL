#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include "cgRender.h"

namespace parser
{
    using namespace std;
    using namespace boost;
    
    vtk_file* parse_vtk_file(string s)
    {

        
        // Allocate memory for vtk_file struct and vectors
        
        vtk_file* input = (vtk_file*)malloc(sizeof(struct vtk_file));
        vector<float>* points = new vector<float>;
        vector<int>* polygons = new vector<int>;
        vector<float>* texture = new vector<float>;

        
        // Set up tokenizer
        char_separator<char> sep(" \n");
        tokenizer<char_separator<char> > tok(s,sep);
        tokenizer<char_separator<char> >::iterator beg=tok.begin();		

        // Move to start of point data
        while ((*beg) != "POINTS")
        {
            beg++;
        }


        beg++;


        // Get number of points
        int point_count = lexical_cast<int>(*beg++);

        beg++;

        // While the token is not polygons, add points to vector
        cout << "Parsing point data" << endl;
        while(*beg != "POLYGONS")
        {
            points->push_back(lexical_cast<float>(*beg++));
        }
        cout << points->size() << endl;

        beg++;
        
        // Get polygon data
        int polygon_no = lexical_cast<int>(*beg);
        beg++;
        int polygon_size = lexical_cast<int>(*beg);
        beg++;

        // While the token is not point_data, add polygons to vector
        cout << "Parsing polygon data" << endl;
        while (*beg != "POINT_DATA")
        {
            polygons->push_back(lexical_cast<int>(*beg++));
        }

        // Move iterator into position
        beg++;
        beg++;
        beg++;
        beg++;
        beg++;
        beg++;
        
        cout << "Parsing texture data" << endl;
        while (beg != tok.end())
        {
            texture->push_back(lexical_cast<float>(*beg++));
        }

        input->point_count = point_count;
        input->polygon_size = polygon_size;
        input->polygon_no = polygon_no;

        input->points = points;
        input->polygons = polygons;
        input->point_data = texture;
        
        cout << "Parsing complete" << endl;

        return input;
    }

}

int main (int argc, char** argv)
{
    using namespace std;
    // Check input arguments
    // and print a usage on failure
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << "and then a .vtk file, followed by 1 (meaning Gourand rendered "
            << "or 2 (meaning texture mapped render)" << endl;
        exit(EXIT_FAILURE);
    }

    //int length;
    ifstream inputFile(argv[1]);


    // get length of file:
    inputFile.seekg (0, std::ios::end);
    int length = inputFile.tellg();
    inputFile.seekg (0, std::ios::beg);

    // allocate memory for the string
    char* buffer = new char[length];
    
    //Read file into string
    inputFile.read (buffer,length);
    inputFile.close();
    std::string vtk_input = buffer;
    
    //Parse the file
    cout << "Parsing vtk file: " << argv[1] << endl;
    vtk_file* file = parser::parse_vtk_file((string)buffer);
}
