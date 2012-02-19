#ifndef _SHADER_UTILS_H
#define _SHADER_UTILS_H

#include <GL/glew.h>
#include <string>
#include <stdlib.h>
#include <vector>


using namespace std;

void print_log(GLuint object);
string read_file(char* fname);
GLuint shader_create(char* fname, GLenum type);

template<typename T>
T * vector_to_array(vector<T>* v);

#endif