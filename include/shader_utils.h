#ifndef _SHADER_UTILS_H
#define _SHADER_UTILS_H

#include <GL/glew.h>
#include <string>
#include <stdlib.h>

using namespace std;


void print_log(GLuint object);
string read_file(char* fname);
GLuint shader_create(const char* fname, GLenum type);

#endif