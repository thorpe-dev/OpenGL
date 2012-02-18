#include <shader_utils.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Debug function
void print_log(GLuint object)
{
  GLint length = 0;
  
  if (glIsShader(object))
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
  
  else if (glIsProgram(object))
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
  
  else {
    cout << "printlog: Not a shader or a program" << endl;
    return;
  }
 
  char* log = (char*)malloc(length);
 
  if (glIsShader(object))
    glGetShaderInfoLog(object, length, NULL, log);
  else if (glIsProgram(object))
    glGetProgramInfoLog(object, length, NULL, log);
 
  cout << log << endl;
  free(log);
}

string read_file(char* fname)
{
    /*ifstream ifile(fname);
    string filetext;

    while( ifile.good() ) {
        std::string line;
        std::getline(ifile, line);
        filetext.append(line + "\n");
    }

    return filetext;*/
    
    ifstream t(fname);
    stringstream* buffer = new stringstream();
    *buffer << t.rdbuf();
        
    return buffer->str();
}

GLuint shader_create(char* fname, GLenum type)
{
    const char* source = read_file(fname).c_str();
        
    if (source == NULL)
    {
        cout << "Error opening file: " << fname << endl;
        return 0;
    }
    
    GLuint result = glCreateShader(type);
    
    glShaderSource(result, 2, &source, NULL);
    free((void*)source);
    
    glCompileShader(result);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(result, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE)
    {
        cout << "Error in file: " << fname << endl;
        print_log(result);
        glDeleteShader(result);
        return 0;
    }
    return result;
}