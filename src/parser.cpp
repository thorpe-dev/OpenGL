#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <fstream>

//include "cgRender.h"
#include "parser.h"


    using namespace std;
    using namespace boost;
    
    parser::parser(char* s, bool b)
    {
        if (b) 
        {
            vtk = parse_vtk_file(s);
            ppm = NULL;
        }
        else 
        {
            vtk = NULL;
            ppm = parse_ppm_file(s);
        }
    }
    
    ppm_file* parser::parse_ppm_file(char* s)
    {
        ppm_file* input = (ppm_file*)malloc(sizeof(struct ppm_file));
        
        ifstream file(s);
        
        // Drop the header
        char discard[20];
        file.getline(discard,20);
        
        file >> input->width;
        file >> input->height;
        file >> input->max;
        
        // Skip over line-feed
        file.get();
        
        input->bytecount = 3 * input->width * input->height;
        
        char* texture = new char[input->bytecount];
        
        file.read(texture,input->bytecount);
        
        input->texture = texture;
        
        return input;
    }
    
    string parser::read_file(char* fname)
    {
        ifstream t(fname);
        stringstream* buffer = new stringstream();
        *buffer << t.rdbuf();
        
        t.close();
            
        return buffer->str();
    }
    
    vtk_file* parser::parse_vtk_file(char* d)
    {
        // Allocate memory for vtk_file struct and vectors
        string s = read_file(d);
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

        beg++;
        
        // Get polygon data
        int polygon_count = lexical_cast<int>(*beg);
        beg++;
        beg++;

        // While the token is not point_data, add polygons to vector
        // We're going to drop every 4th element - the number of points making up the polygon
        
        cout << "Parsing polygon data" << endl;
        int count = 0;
        while (*beg != "POINT_DATA")
        {
            if ((count % 4) != 0)
            {
                polygons->push_back(lexical_cast<int>(*beg));
            }
            beg++;
            count++;
        }
        
        beg++;
        
        int texture_count = lexical_cast<int>(*beg++);

        // Move iterator into position
        beg++;
        beg++;
        beg++;
        beg++;
        
        //While the token is not at the end, add texture data to vector
        cout << "Parsing texture data" << endl;
        while (beg != tok.end())
        {
            texture->push_back(lexical_cast<float>(*beg++));
        }
        
        // Set up vtk struct
        input->point_count = point_count;
        input->polygon_count = polygon_count;
        input->texture_count = texture_count;

        input->points = points;
        input->polygons = polygons;
        input->textures = texture;
        
        cout << "Parsing complete" << endl;

        return input;
    }