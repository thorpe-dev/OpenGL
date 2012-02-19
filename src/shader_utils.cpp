#include <shader_utils.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


template <typename T>
T * vector_to_array(vector<T>* v)
{
    int size = v->size();
    T* ret = new T[size];
    
    for (int i = 0; i < size / 3; i++)
    {
        ret[i] = (*v)[i];
    }
    
    return ret;
}

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
    ifstream t(fname);
    stringstream* buffer = new stringstream();
    *buffer << t.rdbuf();
        
    return buffer->str();
}

GLuint shader_create(char* fname, GLenum type)
{
    cout << "Making shader from file: " << fname << endl;
    const char* source = read_file(fname).c_str();
        
    if (source == NULL)
    {
        cout << "Error opening file: " << fname << endl;
        return 0;
    }
    
    
    GLuint result = glCreateShader(type);
    
    glShaderSource(result, 1, &source, NULL);
    
    print_log(result);
    //delete(source);
        
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