#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <fstream>

//include "cgRender.h"
#include "parser.h"


    using namespace std;
    using namespace boost;
    
    ppm_file* parse_ppm_file(char* s)
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
    
    string read_file(char* fname)
    {
        ifstream t(fname);
        stringstream* buffer = new stringstream();
        *buffer << t.rdbuf();
        
        t.close();
            
        return buffer->str();
    }
    
    void parse_vtk_file(char* d, vector<glm::vec3> &points, vector<glm::vec3> &normals, vector<glm::vec3> &polygons, vector<glm::vec2> &textures)
    {
        // Allocate memory for vtk_file struct and vectors
        string s = read_file(d);
   

        
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
        
        points = vector<glm::vec3>(point_count);
        normals = vector<glm::vec3>(point_count, glm::vec3(0.0,0.0,0.0));
        
        glm::vec3 point;
        glm::vec3 norm;

        beg++;
        
        int i = 0;

        // While the token is not polygons, add points to vector
        cout << "Parsing point data" << endl;
        while(*beg != "POLYGONS")
        {
            point = glm::vec3(
                lexical_cast<float>(*beg++),
                lexical_cast<float>(*beg++),
                lexical_cast<float>(*beg++));
            points[i] = point;
            norm = glm::vec3(0.0f,0.0f,0.0f);
            normals[i] = norm;
            i++;
        }

        beg++;
        
        // Get polygon data
        int polygon_count = lexical_cast<int>(*beg++);
        
        beg++;
        
        polygons = vector<glm::vec3>(polygon_count, glm::vec3(0.0,0.0,0.0));
        
        glm::vec3 poly;

        cout << "Parsing polygon data" << endl;

        int j = 0;
        while (*beg != "POINT_DATA")
        {
            beg++;
            poly = glm::vec3(
                lexical_cast<float>(*beg++),
                lexical_cast<float>(*beg++),
                lexical_cast<float>(*beg++)
            );
            polygons[j] = poly;
            j++;
        }
        
        beg++;
        
        int texture_count = lexical_cast<int>(*beg++);

        // Move iterator into position
        beg++;
        beg++;
        beg++;
        beg++;
        
        textures = vector<glm::vec2>(texture_count);
        
        cout << "Allocated" << endl;
        int k = 0;
        glm::vec2 t;
        
        //While the token is not at the end, add texture data to vector
        cout << "Parsing texture data" << endl;
        while (beg != tok.end())
        {
            t = glm::vec2(
                lexical_cast<float>(*beg++),
                lexical_cast<float>(*beg++)
            );
            textures[k] = t;
            k++;
            
        }        
        cout << "Parsing complete" << endl;
    }